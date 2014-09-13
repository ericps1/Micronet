/*
 * DMA Rebuild Micahel Botkin
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

// The .wav file is mono at 22050Hz, which means we have a new sample every
// 45.4us.  We want to adjust the 100MHz core frequency at 10 times that so as
// to provide some level of subsampling to improve quality.  The basic idea is
// to maintain a buffer of 4000 values to write to the clock control register
// and then arrange for the DMA controller to write the values sequentially at
// 4.54us intervals.  The control code can then wake up every 10ms or so and
// populate the buffer with new samples.  At 4.54us per sample, a 4000 sample
// buffer will last a bit over 18ms, so waking every 10ms should be sufficient.
//
// Total memory needed is:
//
// The frequencies		4000 * 4
// CBs to set the frequency	4000 * 32
// CBs to cause delays		4000 * 32
//
// Process can wake every 10ms and update all samples based on where the DMA
// CB is pointed.

/************************************************************************/
/***************************  Constants  ********************************/
/************************************************************************/

#define NUM_SAMPLES	    16	
#define NUM_CBS			(NUM_SAMPLES * 2)

#define BCM2708_DMA_NO_WIDE_BURSTS	(1<<26)
#define BCM2708_DMA_WAIT_RESP		(1<<3)
#define BCM2708_DMA_D_DREQ		    (1<<6)
#define BCM2708_DMA_PER_MAP(x)		((x)<<16)
#define BCM2708_DMA_END			    (1<<1)
#define BCM2708_DMA_RESET	    	(1<<31)
#define BCM2708_DMA_INT		    	(1<<2)

#define DMA_CS			(0x00/4)
#define DMA_CONBLK_AD	(0x04/4)
#define DMA_DEBUG		(0x20/4)

/* bus addresses */
//#define CM_GP0CTL (0x7e101070)
//#define GPFSEL0 (0x7E200000)
#define CM_GP0DIV (0x7e101074)
//#define CLKBASE (0x7E101000)
//#define DMABASE (0x7E007000)
//#define PWMBASE  (0x7e20C000) /* PWM controller */
/* Using physical address instead of 
 * bus address at 0x7ennnnnn */
#define DMA_BASE		0x20007000
#define DMA_LEN			0x24
#define PWM_BASE		0x2020C000
#define PWM_LEN			0x28
#define CLK_BASE	    0x20101000
#define CLK_LEN			0xA8
#define GPIO_BASE		0x20200000
#define GPIO_LEN		0xB4

#define PWM_CTL			(0x00/4)
#define PWM_DMAC		(0x08/4)
#define PWM_RNG1		(0x10/4)
#define PWM_FIFO		(0x18/4)

#define PWMCLK_CNTL		40
#define PWMCLK_DIV		41

#define GPCLK_CNTL		(0x70/4)
#define GPCLK_DIV		(0x74/4)

#define PWMCTL_MODE1	(1<<1)
#define PWMCTL_PWEN1	(1<<0)
#define PWMCTL_CLRF		(1<<6)
#define PWMCTL_USEF1	(1<<5)

#define PWMDMAC_ENAB	(1<<31)
// I think this means it requests as soon as there is one free slot in the FIFO
// which is what we want as burst DMA would mess up our timing..
#define PWMDMAC_THRSHLD		((15<<8)|(15<<0))

#define GPFSEL0			(0x00/4)

#define PLLFREQ			500000000	// PLLD is running at 500MHz
#define CARRIERFREQ		100000000	// Carrier frequency is 100MHz
// The deviation specifies how wide the signal is. Use 25.0 for WBFM
// (broadcast radio) and about 3.5 for NBFM (walkie-talkie style radio)
#define DEVIATION		25.0

#define MOD_FREQ 10000

/************************************************************************/
/******************** Data Structs and Global Vars **********************/
/************************************************************************/

typedef struct 
{
    uint32_t info, src, dst, length,
             stride, next, pad[2];
} dma_cb_t;

typedef struct 
{
    uint8_t *virtaddr;
    uint32_t physaddr;
} page_map_t;

page_map_t *page_map;

static uint8_t *virtbase;

static volatile uint32_t *pwm_reg;
static volatile uint32_t *clk_reg;
static volatile uint32_t *dma_reg;
static volatile uint32_t *gpio_reg;

/* 2 times the amount of cbs used
 * 2 per sample
 * one for sample
 * one for delay */
struct control_data_s 
{
    dma_cb_t cb[NUM_CBS];
    uint32_t sample[NUM_SAMPLES];
};

#define PAGE_SIZE	4096
#define PAGE_SHIFT	12
#define NUM_PAGES	((sizeof(struct control_data_s) + PAGE_SIZE - 1) >> PAGE_SHIFT)

static struct control_data_s *ctl;

/************************************************************************/
/********************** System Control Fucntions ************************/
/************************************************************************/

static void udelay(int us)
{
    struct timespec ts = { 0, us * 1000 };

    nanosleep(&ts, NULL);
}

void dma_disable()
{
    printf("\nexiting\n");

    gpio_reg[GPFSEL0] = (gpio_reg[GPFSEL0] & ~(7 << 12));
    //clk_reg[PWMCLK_CNTL] = 0x5A100016;              // Source=PLLD and disable
    udelay(10);

    // struct GPCTL setupword = {6/*SRC*/, 0, 0, 0, 0, 1,0x5a};
    // ACCESS(CM_GP0CTL) = *((int*)&setupword);

    // struct DMAregs* DMA0 = (struct DMAregs*)&(ACCESS(DMABASE));
    // DMA0->CS =1<<31;  // reset dma controller

    /* compare these two methods */
    if (dma_reg) {
        dma_reg[DMA_CS] = BCM2708_DMA_RESET;
        udelay(10);
        printf("\nDMA Disabled\n");
    }
    exit(0);
}

static void error(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(0);
}


void sig_handler() 
{
    exit(0);
}

/************************************************************************/
/************************* Memory Fucntions *****************************/
/************************************************************************/

static uint32_t mem_virt_to_phys(void *virt)
{
    uint32_t offset = (uint8_t *)virt - virtbase;
    return page_map[offset >> PAGE_SHIFT].physaddr + (offset % PAGE_SIZE);
}

static uint32_t mem_phys_to_virt(uint32_t phys)
{
    uint32_t pg_offset = phys & (PAGE_SIZE - 1);
    uint32_t pg_addr = phys - pg_offset;
    int i;

    for (i = 0; i < NUM_PAGES; i++) {
        if (page_map[i].physaddr == pg_addr) {
            return (uint32_t)virtbase + i * PAGE_SIZE + pg_offset;
        }
    }
    error("Failed to reverse map phys addr %08x\n", phys);
    return 0;
}

static void* map_peripheral(uint32_t base, uint32_t len)
{
    int fd = open("/dev/mem", O_RDWR);
    void * vaddr;

    if (fd < 0)
        error("Failed to open /dev/mem: %m\n");
    vaddr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, base);
    if (vaddr == MAP_FAILED)
        error("Failed to map peripheral at 0x%08x: %m\n", base);
    close(fd);

    return vaddr;
}

/************************************************************************/
/**********************   Main    ***************************************/
/************************************************************************/

int main(int argc, char **argv)
{
    atexit(dma_disable);
    signal (SIGINT, sig_handler);
    signal (SIGTERM, sig_handler);
    signal (SIGHUP, sig_handler);
    signal (SIGQUIT, sig_handler);

    int i, fd, pid, freq_ctl;
    char pagemap_fn[64];


    // Calculate the frequency control word
    // The fractional part is stored in the lower 12 bits
    /* argv will hold the frequency, 100M will be the default */
    if(argc == 1)
        freq_ctl = ((float)(PLLFREQ / CARRIERFREQ)) * ( 1 << 12 );
    else if(argc == 2)
    {
        int center_freq = strtod(argv[1],NULL);
        if(center_freq)
            freq_ctl = ((float)(PLLFREQ / center_freq)) * ( 1 << 12 );
        else
            error("Invalid Center Freq\n");
    }
    else
        error("Invalid number of arguments\n");

    /* Map virtual and digital memory */
    dma_reg = map_peripheral(DMA_BASE, DMA_LEN);
    pwm_reg = map_peripheral(PWM_BASE, PWM_LEN);
    clk_reg = map_peripheral(CLK_BASE, CLK_LEN);
    gpio_reg = map_peripheral(GPIO_BASE, GPIO_LEN);

    printf("\nnum pages: %i\n",NUM_PAGES);

    //
    //error("terminate");
    //
    /* mapping memorry to be used */

    virtbase = mmap(NULL, NUM_PAGES * PAGE_SIZE, PROT_READ|PROT_WRITE,
            MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE|MAP_LOCKED, -1, 0);
    if (virtbase == MAP_FAILED)
        error("Failed to mmap physical pages: %m\n");
    if ((unsigned long)virtbase & (PAGE_SIZE-1))
        error("Virtual address is not page aligned\n");
    printf("Virtual memory mapped at %p\n", virtbase);
    page_map = malloc(NUM_PAGES * sizeof(*page_map));
    if (page_map == 0)
        error("Failed to malloc page_map: %m\n");

    /* get the current process ID to read the page map */
    pid = getpid();
    sprintf(pagemap_fn, "/proc/%d/pagemap", pid);
    fd = open(pagemap_fn, O_RDONLY);

    if (fd < 0)
        error("Failed to open %s: %m\n", pagemap_fn);

    if (lseek(fd, (unsigned long)virtbase >> 9, SEEK_SET) != (unsigned long)virtbase >> 9)
        error("Failed to seek on %s: %m\n", pagemap_fn);

    //	printf("Page map:\n");
    
    for (i = 0; i < NUM_PAGES; i++) 
    {
        uint64_t pfn;
        page_map[i].virtaddr = virtbase + i * PAGE_SIZE;
        // Following line forces page to be allocated
        page_map[i].virtaddr[0] = 0;
        if (read(fd, &pfn, sizeof(pfn)) != sizeof(pfn))
            error("Failed to read %s: %m\n", pagemap_fn);
        if ((pfn >> 55)&0xfbf != 0x10c)  // pagemap bits: https://www.kernel.org/doc/Documentation/vm/pagemap.txt
            error("Page %d not present (pfn 0x%016llx)\n", i, pfn);
        page_map[i].physaddr = (uint32_t)pfn << PAGE_SHIFT | 0x40000000;
        //		printf("  %2d: %8p ==> 0x%08x [0x%016llx]\n", i, page_map[i].virtaddr, page_map[i].physaddr, pfn);
    }

    // GPIO4 needs to be ALT FUNC 0 to otuput the clock
    gpio_reg[GPFSEL0] = (gpio_reg[GPFSEL0] & ~(7 << 12)) | (4 << 12);

    // Program GPCLK to use MASH setting 1, so fractional dividers work
    clk_reg[GPCLK_CNTL] = 0x5A << 24 | 6;
    udelay(100);
    clk_reg[GPCLK_CNTL] = 0x5A << 24 | 1 << 9 | 1 << 4 | 6;

    ctl = (struct control_data_s *)virtbase;
    dma_cb_t *cbp = ctl->cb;

    uint32_t phys_sample_dst = CM_GP0DIV;
    uint32_t phys_pwm_fifo_addr = 0x7e20c000 + 0x18;

    for (i = 0; i < NUM_SAMPLES; i++) 
    {
        ctl->sample[i] = 0x5a << 24 | freq_ctl;	// Silence
        // Write a frequency sample
        cbp->info = BCM2708_DMA_NO_WIDE_BURSTS | BCM2708_DMA_WAIT_RESP;
        cbp->src = mem_virt_to_phys(ctl->sample + i);
        cbp->dst = phys_sample_dst;
        cbp->length = 4;
        cbp->stride = 0;
        cbp->next = mem_virt_to_phys(cbp + 1);
        cbp++;
        // pwm is used between each data sample to delay it
        // Delay
        /* wait_resp is used instead of interrupts, the DMA waits unti the AXI has written it, PWM (i assume) 
         * wont accept new write until it completed its write */
        cbp->info = BCM2708_DMA_NO_WIDE_BURSTS | BCM2708_DMA_WAIT_RESP | BCM2708_DMA_D_DREQ | BCM2708_DMA_PER_MAP(5);
        cbp->src = mem_virt_to_phys(virtbase);
        cbp->dst = phys_pwm_fifo_addr;
        cbp->length = 4;
        cbp->stride = 0;
        cbp->next = mem_virt_to_phys(cbp + 1);
        cbp++;
    }
    cbp--;
    /* set the base so it is a circular list */
    cbp->next = mem_virt_to_phys(virtbase);

    // Initialise PWM to use a 100MHz clock too, and set the range to
    // 454 bits, which is 4.54us, the rate at which we want to update
    // the GPCLK control register.
    pwm_reg[PWM_CTL] = 0;
    udelay(10);
    clk_reg[PWMCLK_CNTL] = 0x5A000006;              // Source=PLLD and disable
    udelay(100);
    clk_reg[PWMCLK_DIV] = 0x5A000000 | (5<<12);    // set pwm div to 5, for 100MHz
    udelay(100);
    clk_reg[PWMCLK_CNTL] = 0x5A000016;              // Source=PLLD and enable
    udelay(100);
    pwm_reg[PWM_RNG1] = CARRIERFREQ/MOD_FREQ; /* 1000 times a second */
    /* in .001 micro sec inc */
    /* PWM_RNG1/PWMCLK = 454/100MHz = 0.00000454 or 4.54 uHz */
    udelay(10);
    pwm_reg[PWM_DMAC] = PWMDMAC_ENAB | PWMDMAC_THRSHLD;
    udelay(10);
    pwm_reg[PWM_CTL] = PWMCTL_CLRF;
    udelay(10);
    pwm_reg[PWM_CTL] = PWMCTL_USEF1 | PWMCTL_PWEN1;
    udelay(10);

    // Initialise the DMA
    dma_reg[DMA_CS] = BCM2708_DMA_RESET;
    udelay(10);
    dma_reg[DMA_CS] = BCM2708_DMA_INT | BCM2708_DMA_END;
    dma_reg[DMA_CONBLK_AD] = mem_virt_to_phys(ctl->cb);
    dma_reg[DMA_DEBUG] = 7; // clear debug error flags
    dma_reg[DMA_CS] = 0x10880001;	// go, mid priority, wait for outstanding writes

    uint32_t last_cb = (uint32_t)ctl->cb;

	int data[NUM_SAMPLES] = {1,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0}; // We'll want to look into being able to initialize variable length vectors

    for (;;) 
    {
        /* this is why 4 * numsamples is used for control block pages
         * to prevent data being transmitted from being over writted */
        /* with 1000 times a second, each bit takes 0.001 seconds 
         * 10 bits = 0.01  which is 10000 uS*/

        usleep(((10^6*NUM_SAMPLES)/MOD_FREQ));

        /* conblk - control block */
        /* get current controlblock */
        uint32_t cur_cb = mem_phys_to_virt(dma_reg[DMA_CONBLK_AD]);
        int last_sample = (last_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
        int this_sample = (cur_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
        int free_slots = this_sample - last_sample;

        if (free_slots < 0)
            free_slots += NUM_SAMPLES;

        while (free_slots >= 1) 
        {
                /* this sets up the frequency to be transmitted for this sample 
                 * 10 repeatedvalues (i.e. sub samples) */
                /* 10 samples were used, 
                 * frac (if .1 become 1) says how many freq below
                 * so 1 intval, 9 intval + 1
                 * */

                /* instead of center frequency use the freq_ctl and unshift it 12 places, somehow get the center freq from it
                 * and adjust it */

                //double tsleep = 1000000/rate;
                //int i;
                //for(i=0; i<length; i++){
                //    double freq = center_freq + ((double)data[i]-0.5)*freq_space/2;
                //    int INT = 500/freq;
                //    int FRAC = 4096*(500/(freq)-INT);
                //        ACCESS(CM_GP0DIV) = (0x5a << 24) + (INT << 12) + FRAC; // Modify the div value of the CM_GP0DIV register
                //    usleep(tsleep);
                //}

                double center_freq = 100;		// Center frequency in MHz
                double freq_space = 0.1;
                double freq = center_freq + ((double)data[last_sample]-0.5)*freq_space;
                int INT = 500/freq;
                int FRAC = 4096*(500/(freq)-INT);

                ctl->sample[last_sample++] = (0x5a << 24) + (INT << 12) + FRAC; // Modify the div value of the CM_GP0DIV register

            if (last_sample == NUM_SAMPLES)
                last_sample = 0;
            free_slots--;
        }
        last_cb = (uint32_t)virtbase + last_sample * sizeof(dma_cb_t) * 2;
    }
    error("terminate");
    return 0;
}

