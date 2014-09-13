/********************************************************************/
/*																	*/
/*  Michael Botkin							                        */
/*																	*/
/*	File Name : pitx.cpp      								        */
/*																	*/
/*	Description : Function set for the 					            */
/*																	*/
/*	Date created : 3/18/2014										*/
/*																	*/
/*	Revisions :	        											*/
/*      3/18/2014 - pitx, header with defines, memory/sys functs    */
/*      3/23/2014 - statics removed, pitxing, increased modularity  */
/*      4/3/2014 - mod rate input now accepted                      */
/*      4/4/2014 - function unification                             */
/*      4/8/2014 - Packtizing, dma_ook, usage func, file handling   */
/*      4/10/2014 - file handling finished, no repeat, improving    */
/*      4/13/2014 - single transmit improved                        */
/*      4/14/2014 - single transmit improved                        */
/*      4/16/2014 - Repeat defualt off                              */
/*      4/20/2014 - BPS packet                                      */
/*      4/22/2014 - Addressing                                      */
/*      4/28/2014 - Header improved and modularized                 */
/*																	*/
/*	README :													 	*/
/*		Compiled with g++ on Ubuntu 12.04       					*/
/*      "g++ main.cpp pitx.cpp pitx.h ... "                         */
/*      "sudo ./a.out -f 128"                                       */
/*																	*/
/********************************************************************/


#include "dma_pitx.h"
#include "pitx.h"
#include "file_handler.h"



/************************************************************************/
/***************************  Functions  ********************************/
/************************************************************************/


/********************************************************************/
/*																	*/
/*  Function : init_dma          		                            */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Initilize the pitx files and set up signal handlers   */
/*                                                                  */ 
/*  Example : init_dma(); // Sig handlers setup                     */
/*																	*/
/********************************************************************/
void init_dma(void)
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    signal(SIGHUP, sig_handler);
    signal(SIGQUIT, sig_handler);

    initialized = 1;

    memset(flags,0,NUM_OPTIONS);
}

/********************************************************************/
/*																	*/
/*  Function : dma_pitx_init       		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int mod_type: 0 for fsk and 1 for ook						*/  
/*																	*/
/*	Purpose : Initilize the pitx files and set up signal handlers   */
/*                                                                  */ 
/*  Example : dma_pitx_init(OOK);                      */
/*																	*/
/********************************************************************/
void dma_pitx_init(int mod_type)
{
    if(verbose)
        printf("\n DMA pitx INITIALIZE \n");

    if(verbose)
        printf("\nnum pages: %i\n",NUM_PAGES);

    /* set up the virtual and physical memory */
    /* map the virtual pages */
    if(verbose)
        printf("Allocating and mapping memory\n");
    map_memory(DMA);

    if(verbose)
        printf("setting up the clock\n");
    clock_connect(mod_type);

    if(verbose)
        printf("setting up the dma ctl CB : NumSamples = %i\n",NUM_SAMPLES);
    dma_cb_setup(mod_type);

    if(verbose)
        printf("Initializing up the PWM\n");
    pwm_init();

    if(verbose)
        printf("Initializing up the DMA\n");
    dma_init();

    if(verbose)
        printf("\n DMA pitx INITIALIZE END \n");
}

/********************************************************************/
/*																	*/
/*  Function : map_memory         		                            */
/*																	*/
/*	Arguements :           					    					*/
/*		int dma : tells the function if the DMA needs to be mapped	*/
/*																	*/
/*	Purpose : Map virtual and physical memory                       */
/*      Allocating and mapping the memory to be use                 */ 
/*                                                                  */ 
/*  Example : map_memory(1);                                        */
/*																	*/
/********************************************************************/
void map_memory(int dma)
{
    /* Map virtual and physical memory */
    if(dma)
    {
        dma_reg = map_peripheral(DMA_BASE, DMA_LEN);
        pwm_reg = map_peripheral(PWM_BASE, PWM_LEN);
    }

    clk_reg = map_peripheral(CLK_BASE, CLK_LEN);
    gpio_reg = map_peripheral(GPIO_BASE, GPIO_LEN);

    /* allocating and mapping memory to be used */
    /* set up the virtual base */
    virtbase = mmap(NULL, NUM_PAGES * PAGE_SIZE, PROT_READ|PROT_WRITE,
            MAP_SHARED|MAP_ANONYMOUS|MAP_NORESERVE|MAP_LOCKED, -1, 0);
    /* Check if virtbase setup worked */
    if (virtbase == MAP_FAILED)
        error("Failed to mmap physical pages: %m\n");
    if ((unsigned long)virtbase & (PAGE_SIZE-1))
        error("Virtual address is not page aligned\n");

    if(verbose)
        printf("Virtual memory mapped at %p\n", virtbase);


    /* Allocate the number of pages needed */
    page_map = malloc(NUM_PAGES * sizeof(*page_map));
    /* Check if memory existed */
    if (page_map == 0)
        error("Failed to malloc page_map: %m\n");

    /* get the current process ID to read the pagemap */
    /* store the pagemap location in pagemap_fn */

    /* Stores the location for the mapped pages */
    char pagemap_fn[64];

    pid = getpid();
    sprintf(pagemap_fn, "/proc/%d/pagemap", pid);
    fd = open(pagemap_fn, O_RDONLY);

    if (fd < 0)
        error("Failed to open %s: %m\n", pagemap_fn);

    if (lseek(fd, (unsigned long)virtbase >> 9, SEEK_SET) != (unsigned long)virtbase >> 9)
        error("Failed to seek on %s: %m\n", pagemap_fn);

    if(verbose)
        printf("Page map\n");

    /* map the pages to the virtual addresses */
    for (i = 0; i < NUM_PAGES; i++) 
    {
        uint64_t pfn;
        page_map[i].virtaddr = virtbase + i * PAGE_SIZE;
        // Following line forces page to be allocated
        page_map[i].virtaddr[0] = 0;
        if (read(fd, &pfn, sizeof(pfn)) != sizeof(pfn))
            error("Failed to read %s: %m\n", pagemap_fn);
        if (((pfn >> 55)&0xfbf) != 0x10c)  // pagemap bits: https://www.kernel.org/doc/Documentation/vm/pagemap.txt
            error("Page %d not present (pfn 0x%016llx)\n", i, pfn);
        page_map[i].physaddr = (uint32_t)pfn << PAGE_SHIFT | 0x40000000;
        //		printf("  %2d: %8p ==> 0x%08x [0x%016llx]\n", i, page_map[i].virtaddr, page_map[i].physaddr, pfn);
    }
}

/********************************************************************/
/*																	*/
/*  Function : pwm_init         		                            */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Initialize the PWM for thr 100MHz clock               */
/*                                                                  */ 
/*  Example : pwm_init();                                           */
/*																	*/
/********************************************************************/
void pwm_init(void)
{
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
    pwm_reg[PWM_RNG1] = (100*pow(10,6))/mod_freq; /* 1000 times a second */
    /* in .001 micro sec inc */
    /* PWM_RNG1/PWMCLK = 454/100MHz = 0.00000454 or 4.54 uHz */
    udelay(10);
    pwm_reg[PWM_DMAC] = PWMDMAC_ENAB | PWMDMAC_THRSHLD;
    udelay(10);
    pwm_reg[PWM_CTL] = PWMCTL_CLRF;
    udelay(10);
    pwm_reg[PWM_CTL] = PWMCTL_USEF1 | PWMCTL_PWEN1;
    udelay(10);

}

/********************************************************************/
/*																	*/
/*  Function : dma_init         		                            */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Initialize the DMA for thr 100MHz clock               */
/*                                                                  */ 
/*  Example : dma_init();                                           */
/*																	*/
/********************************************************************/
void dma_init(void)
{
    // Initialise the DMA
    dma_reg[DMA_CS] = BCM2708_DMA_RESET;
    udelay(10);
    dma_reg[DMA_CS] = BCM2708_DMA_INT | BCM2708_DMA_END;
    dma_reg[DMA_CONBLK_AD] = mem_virt_to_phys(ctl->cb);
    dma_reg[DMA_DEBUG] = 7; // clear debug error flags
    dma_reg[DMA_CS] = 0x10880001;	// go, mid priority, wait for outstanding writes

}

/********************************************************************/
/*																	*/
/*  Function : clock_connect      		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int mod_type: 0 for fsk and 1 for ook						*/  
/*																	*/
/*	Purpose : Initialize the DMA for thr 100MHz clock               */
/*                                                                  */ 
/*  Example : clock_connect();                                      */
/*																	*/
/********************************************************************/
void clock_connect(int mod_type)
{
    // GPIO4 needs to be ALT FUNC 0 to otuput the clock
    gpio_reg[GPFSEL0_DMA] = (gpio_reg[GPFSEL0_DMA] & ~(7 << 12)) | (4 << 12);

    // Program GPCLK to use MASH setting 1, so fractional dividers work
    clk_reg[GPCLK_CNTL] = 0x5A << 24 | 6;
    udelay(100);

    if(mod_type != OOK)
        clk_reg[GPCLK_CNTL] = 0x5A << 24 | 1 << 9 | 1 << 4 | 6;
    else
        clk_reg[GPCLK_CNTL] = 0x5A << 24 | 1 << 9 | 0 << 4 | 6;


    /* Set up the control frequency */
    // bps = 1
    // 2^1 -> 2 levels -0.5 and 0.5
    double freq;
    if(mod_type == FSK)
        freq = center_freq + (0.5-pow(2,bits_per_sym)/2)*((double)freq_deviation/1000);
    else if(mod_type == OOK)
        freq = center_freq;
    else
        error("Not a valid modulation type");

    int INT = 500/freq;
    int FRAC = 4096*(500/(freq)-INT);
    clk_reg[GPCLK_DIV] = (0x5a << 24) + (INT << 12) + FRAC; // Modify the div value of the CM_GP0DIV register

    usleep(1000000/mod_freq);
}

/********************************************************************/
/*																	*/
/*  Function : dma_cb_setup      		                            */
/*																	*/
/*	Arguements :           					    					*/
/*		int mod_type: 0 for fsk and 1 for ook						*/  
/*																	*/
/*	Purpose : set up the control blocks for the dma                 */
/*	every other cb goes to the pwm for timing                       */
/*	destination reg depends on mod_type                             */
/*                                                                  */ 
/*  Example : dma_cb_setup(0);                                      */
/*																	*/
/********************************************************************/
void dma_cb_setup(int mod_type)
{
    /* BPS ranges */
    /* 1 bps = 2 levels -> -0.5 is min
     * 2 bps = 4 levels -> -1.5 is min
     * 3 bps = 8 levels -> -3.5 is min */
    /* ((0.5-pow(2,bps))*freq_deviation */

    /* Set up the control frequency */
    double freq = center_freq + (0.5-pow(2,bits_per_sym)/2)*((double)freq_deviation/1000);
    int INT = 500/freq;
    int FRAC = 4096*(500/(freq)-INT);

    ctl = (struct control_data_s *)virtbase;
    dma_cb_t *cbp = ctl->cb;

    uint32_t phys_sample_dst;

    /* Type determins the destination
     * 1 for clock enable disable (OOK)
     * 0 for clock divisor reg    (FSK) */
    if(mod_type == FSK)
        phys_sample_dst = CM_GP0DIV;
    else if(mod_type == OOK)
        phys_sample_dst = CM_GP0CTL;
    else
        error("Not a valid modulation type");

    uint32_t phys_pwm_fifo_addr = 0x7e20c000 + 0x18;

    for (i = 0; i < NUM_SAMPLES; i++) 
    {
        if(mod_type == OOK)
            ctl->sample[i] = 0x5A << 24 | 1 << 9 | 0 << 4 | 6;
        else
            ctl->sample[i] = (0x5a << 24) + (INT << 12) + FRAC; // Modify the div value of the CM_GP0DIV register

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
}



/************************************************************************/
/************************* transmit Fucntions ***************************/
/************************************************************************/


/********************************************************************/
/*																	*/
/*  Function : dma_fsk_transmit   		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int *data: data array to be transmitted						*/
/*		int len: length of the data array    						*/
/*																	*/
/*	Purpose : Transmit the data in the int array using the dma      */
/*	and frequency shift keying                                      */
/*                                                                  */ 
/*  Example : dma_transmit({1,0,1,0},4);                            */
/*																	*/
/********************************************************************/
void dma_fsk_transmit(int *data, int len)
{
    int num_packets = 1;
    int pkt_per_block = 1;
    /* Used to access the **data array */
    int curr_packet = 0;
    int **packetized_data;

    if(packetize)
    {
        packetized_data = packetizer(data,&len,&num_packets);
        pkt_per_block = NUM_SAMPLES/len;

        if(verbose)
            printf("packets per DMA block = %i\n",pkt_per_block);
    }
    else
    {
        if(len>NUM_SAMPLES)
            error("Data Length is too long to transmit");
        else
            packetized_data = &data;
    }

    /* Display data */
    if(verbose)
    {
        printf("Length of data : %i\n",len);
        int i;
        int pkt;
        //int **tmp_data = (int **)data;
        FILE* output_file;
        output_file = fopen("/home/pi/measurements/test_.txt","w");
        for(pkt = 0; pkt < num_packets; pkt++)
        {
            if(flags[FILE_TRANSMIT])
            {
                printf("\nPacket %i : \n",pkt);
                char tmp;
                for(i = HEADER_LEN+CRC_LEN+PKT_HEADER-1; i < len; i++)
                {
                    if((i-(HEADER_LEN+CRC_LEN+PKT_HEADER-1))%8 == 0 && (i-(HEADER_LEN+CRC_LEN+PKT_HEADER-1)))
                        printf("%c",tmp);

                    tmp = (tmp << 1) + packetized_data[pkt][i];

                    //printf("pkt = %i \tData[%i] \t= %i\n",pkt,i,packetized_data[pkt][i]);
                    if(output_file != NULL)
                        fprintf(output_file,"%i\n",packetized_data[pkt][i]);
                }
            }
            else
            {
                //for(i = HEADER_LEN+CRC_LEN+PKT_HEADER-1; i < len; i++)
                for(i = 0; i < len; i++)
                {
                    //printf("pkt = %i \tData[%i] \t= %i\n",pkt,i,packetized_data[pkt][i]);
                    printf("%i",packetized_data[pkt][i]);
                    if(output_file != NULL)
                        fprintf(output_file,"%i\n",packetized_data[pkt][i]);
                }
            }
        }
        fclose(output_file);
    }

    /* ******************************************* */
    /* Call all the other initialization functions */
    /* ******************************************* */

    /* must be in main call dma_diable if main exits */
    atexit(dma_disable);

    dma_pitx_init(FSK);

    /* ************************************* */
    /* End of other initialization functions */
    /* ************************************* */


    uint32_t last_cb = (uint32_t)ctl->cb;


    if(verbose)
        printf("Enetering DMA_FSK Lock Loop\n");

    /* conblk - control block */
    /* get current controlblock */
    uint32_t cur_cb;
    int last_sample;
    int this_sample;
    int free_slots = 0;

    /* wait until enough slots are open */
    while(free_slots < (len - 2) && free_slots < NUM_SAMPLES-1)
    {
        cur_cb = mem_phys_to_virt(dma_reg[DMA_CONBLK_AD]);
        last_sample = (last_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
        this_sample = (cur_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
        free_slots = this_sample - last_sample;

        if (free_slots < 0)
            free_slots += NUM_SAMPLES;

        if (last_sample >= NUM_SAMPLES)
            last_sample = 0;
    }

    int dma_block_pkt_count = 0;
    for (;;) 
    {
        int data_index = 0;

        if (free_slots < 0)
            free_slots += NUM_SAMPLES;

        //for(curr_packet = 0; curr_packet < 2; curr_packet++)
        for(curr_packet = 0; curr_packet < num_packets; curr_packet++)
        {
            if(verbose && !repeat)
                printf("Current Packet = %i\n",curr_packet);

            last_cb = (uint32_t)virtbase + last_sample * sizeof(dma_cb_t) * 2;
            while(free_slots < len)
            {
                /* conblk - control block */
                /* get current controlblock */
                cur_cb = mem_phys_to_virt(dma_reg[DMA_CONBLK_AD]);
                this_sample = (cur_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
                free_slots = this_sample - last_sample;

                if (free_slots < 0)
                    free_slots += NUM_SAMPLES;
            }
            //if(verbose)
            //    printf("pkt = %i \tData[%i] \t= %i\n",curr_packet,data_index,packetized_data[curr_packet][data_index]);

            for(data_index = 0; data_index < len; data_index++)
            {
                /* Depending on the bit, set the frequency */
                double freq = center_freq + (0.5-pow(2,bits_per_sym)/2+(double)packetized_data[curr_packet][data_index])*((double)freq_deviation/1000);
                int INT = 500/freq;
                int FRAC = 4096*(500/(freq)-INT);

                ctl->sample[last_sample++] = (0x5a << 24) + (INT << 12) + FRAC; // Modify the div value of the CM_GP0DIV register

                if (last_sample == NUM_SAMPLES)
                    last_sample = 0;

                free_slots--;
            }
        }
        /* Write packets of zeros */
        int clear_packet;
        for(clear_packet = 0; clear_packet < NUM_SAMPLES/len; clear_packet++)
        {
            last_cb = (uint32_t)virtbase + last_sample * sizeof(dma_cb_t) * 2;
            while(free_slots < len)
            {
                /* conblk - control block */
                /* get current controlblock */
                cur_cb = mem_phys_to_virt(dma_reg[DMA_CONBLK_AD]);
                this_sample = (cur_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
                free_slots = this_sample - last_sample;

                if (free_slots < 0)
                    free_slots += NUM_SAMPLES;
            }
            for(data_index = 0; data_index < len; data_index++)
            {
                /* Depending on the bit, set the frequency */
                double freq = center_freq + (0.5-pow(2,bits_per_sym)/2+0)*((double)freq_deviation/1000);
                int INT = 500/freq;
                int FRAC = 4096*(500/(freq)-INT);

                ctl->sample[last_sample++] = (0x5a << 24) + (INT << 12) + FRAC; // Modify the div value of the CM_GP0DIV register

                if (last_sample == NUM_SAMPLES)
                    last_sample = 0;

                free_slots--;
            }
        }
        dma_block_pkt_count++;


        curr_packet++;

        //if(!repeat && curr_packet >= 2)
        if(!repeat && curr_packet >= num_packets)
        {

            last_cb = (uint32_t)virtbase + last_sample * sizeof(dma_cb_t) * 2;
            usleep(1000000*len*num_packets/mod_freq);

            //while(this_sample != last_sample)
            while(cur_cb != last_cb)
            {
                cur_cb = mem_phys_to_virt(dma_reg[DMA_CONBLK_AD]);
                last_sample = (last_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
                this_sample = (cur_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
                free_slots = this_sample - last_sample;

                if (free_slots < 0)
                    free_slots += NUM_SAMPLES;

                if (last_sample >= NUM_SAMPLES)
                    last_sample = 0;
            }
            usleep(1000000/mod_freq);
             clock_disconnect();

            /* if breaking out of this - DMA is disabled -> need to make sure it is done first */
            break;
        }

        if(num_packets == curr_packet)
            curr_packet = 0;

        /* with 1000 times a second, each bit takes 0.001 seconds 
         * 10 bits = 0.01  which is 10000 uS*/

        /* pkt_per_block = 2
         * num_packets = 4
         * pkt0 -> 1 skip
         * pkt1 -> 2 sleep
         * pkt2 -> 3 skip
         * pkt4 -> 4 sleep
         * pkt5 -> 1 skip */
        if(!(dma_block_pkt_count%pkt_per_block))
        {
            last_cb = (uint32_t)virtbase + last_sample * sizeof(dma_cb_t) * 2;
            usleep(1000000/mod_freq);

            while(this_sample != last_sample)
            {
                cur_cb = mem_phys_to_virt(dma_reg[DMA_CONBLK_AD]);
                last_sample = (last_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
                this_sample = (cur_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
            }

            usleep(1000000/mod_freq);
            dma_block_pkt_count = 0;
            /* Need to clear the DMA here !!! */
        }

        if(curr_packet >= num_packets)
            curr_packet = 0;
    }
}

/********************************************************************/
/*																	*/
/*  Function : dma_ook_transmit                                     */
/*																	*/
/*	Arguements :        					    					*/
/*		int *data: data array to be transmitted						*/
/*		int len: length of the data array    						*/
/*																	*/
/*	Purpose : Transmit the data in the int array using the dma      */
/*	and on off keying                                               */
/*                                                                  */ 
/*  Example : dma_transmit({1,0,1,0},4);                            */
/*																	*/
/********************************************************************/
void dma_ook_transmit(int *data, int len)
{
    int num_packets = 1;
    int pkt_per_block = 1;
    /* Used to access the **data array */
    int curr_packet = 0;
    int **packetized_data;

    if(packetize)
    {
        packetized_data = packetizer(data,&len,&num_packets);
        pkt_per_block = NUM_SAMPLES/len;

        if(verbose)
            printf("packets per DMA block = %i\n",pkt_per_block);
    }
    else
    {
        if(len>NUM_SAMPLES)
            error("Data Length is too long to transmit");
        else
            packetized_data = &data;
    }

    /* Display data */
    if(verbose)
    {
        printf("Length of data : %i\n",len);
        int i;
        int pkt;
        //int **tmp_data = (int **)data;

        for(pkt = 0; pkt < num_packets; pkt++)
            for(i = 0; i < len; i++)
                printf("pkt = %i \tData[%i] \t= %i\n",pkt,i,packetized_data[pkt][i]);

        //if(num_packets > 1)
        //    error("data check");
    }


    /* ******************************************* */
    /* Call all the other initialization functions */
    /* ******************************************* */

    /* must be in main call dma_diable if main exits */
    atexit(dma_disable);

    dma_pitx_init(OOK);

    /* ************************************* */
    /* End of other initialization functions */
    /* ************************************* */


    uint32_t last_cb = (uint32_t)ctl->cb;


    if(verbose)
        printf("Enetering DMA_OOK Lock Loop\n");
    
    /* conblk - control block */
    /* get current controlblock */
    uint32_t cur_cb;
    int last_sample;
    int this_sample;
    int free_slots = 0;

    /* wait until enough slots are open */
    while(free_slots < len-2)
    {
        cur_cb = mem_phys_to_virt(dma_reg[DMA_CONBLK_AD]);
        last_sample = (last_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
        this_sample = (cur_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
        free_slots = this_sample - last_sample;

        if (free_slots < 0)
            free_slots += NUM_SAMPLES;

        if (last_sample >= NUM_SAMPLES)
            last_sample = 0;
    }

    int dma_block_pkt_count = 0;
    for (;;) 
    {
        int data_index = 0;

        if (free_slots < 0)
            free_slots += NUM_SAMPLES;

        while (data_index < len) 
        {
            //if(verbose)
            //    printf("pkt = %i \tData[%i] \t= %i\n",curr_packet,data_index,packetized_data[curr_packet][data_index]);

            /* Depending on the bit, turn on or off the clock */
            //if((data+curr_packet*(sizeof(int*)))[data_index] == 0)
            if(packetized_data[curr_packet][data_index] == 0)
                ctl->sample[last_sample++] = 0x5A << 24 | 1 << 9 | 0 << 4 | 6;
            else
                ctl->sample[last_sample++] = 0x5A << 24 | 1 << 9 | 1 << 4 | 6;

            if (last_sample == NUM_SAMPLES)
                last_sample = 0;

            data_index++;
            free_slots--;

            if (free_slots < 0)
            {
                last_cb = (uint32_t)virtbase + last_sample * sizeof(dma_cb_t) * 2;
                /* wait until enough slots are open */
                while(free_slots < len)
                {
                    /* conblk - control block */
                    /* get current controlblock */
                    cur_cb = mem_phys_to_virt(dma_reg[DMA_CONBLK_AD]);
                    last_sample = (last_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
                    this_sample = (cur_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
                    free_slots = this_sample - last_sample;

                    if (free_slots < 0)
                        free_slots += NUM_SAMPLES;
                }
            }
        }
        //num_packets += pkt_per_block;
        dma_block_pkt_count++;

        if(verbose && !repeat)
            printf("Current Packet = %i\n",curr_packet);

        curr_packet++;

        if(!repeat && curr_packet >= num_packets)
        {
            //ctl->sample[this_sample++] = 0x5A << 24 | 1 << 9 | 0 << 4 | 6;
            //if (this_sample == NUM_SAMPLES)
            //    this_sample = 0;
            last_cb = (uint32_t)virtbase + last_sample * sizeof(dma_cb_t) * 2;
            usleep(1000000*(PKT_HEADER)/mod_freq);

            do{
                cur_cb = mem_phys_to_virt(dma_reg[DMA_CONBLK_AD]);
                last_sample = (last_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
                this_sample = (cur_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
            }while(this_sample != last_sample);

            usleep(1000000/mod_freq);
            clock_disconnect();


            /* if breaking out of this - DMA is disabled -> need to make sure it is done first */
            break;
        }

        if(num_packets == curr_packet)
            curr_packet = 0;

        /* with 1000 times a second, each bit takes 0.001 seconds 
         * 10 bits = 0.01  which is 10000 uS*/

        /* pkt_per_block = 2
         * num_packets = 4
         * pkt0 -> 1 skip
         * pkt1 -> 2 sleep
         * pkt2 -> 3 skip
         * pkt4 -> 4 sleep
         * pkt5 -> 1 skip */
        if(!(dma_block_pkt_count%pkt_per_block))
        {
            last_cb = (uint32_t)virtbase + last_sample * sizeof(dma_cb_t) * 2;
            usleep(1000000/mod_freq);

            while(this_sample != last_sample)
            {
                cur_cb = mem_phys_to_virt(dma_reg[DMA_CONBLK_AD]);
                last_sample = (last_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
                this_sample = (cur_cb - (uint32_t)virtbase) / (sizeof(dma_cb_t) * 2);
            }
            //usleep(1000000/mod_freq);

            //usleep(((1000000*len*pkt_per_block)/mod_freq));
            dma_block_pkt_count = 0;
            /* Need to clear the DMA here !!! */
        }

        if(curr_packet >= num_packets)
            curr_packet = 0;
    }
}


/************************************************************************/
/************************** Transmit Helpers ****************************/
/************************************************************************/


/********************************************************************/
/*																	*/
/*  Function : packetizer         		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int *data: data array pointer to be modified				*/
/*		int *len: length of the data array, modified to new len  	*/
/*		int *packet_count: returns the number of packets made		*/
/*																	*/
/*	Purpose : Packetize the data, modify and return                 */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
int **packetizer(int *data, int *len, int *packet_count)
{
    if(header && pkt_size < HEADER_LEN+CRC_LEN)
        error("Packet Size too short");
    if(verbose)
        printf("\n IN PACKETIZER \n");

    if(flags[FILE_TRANSMIT] && tx_file_name)
        file_name_set(&data, len);
        /* SET THE FILE NAME IN FRONT OF THE DATA */

    if(bits_per_sym > 1)
        (*len) = bps_packer(&data,(*len));

    /* make a function that does the entire header, determins the len and 
     * returns it */

    /* Figure out how many packets needed for the data */
    int num_packets = 1;

    /* Calculate how many packets are needed for data + headers */
    int header_len = 0;
    while(1)
    {
        /* ACCOUNT FOR BPS PACKING */
        if(header)
            header_len = num_packets/bits_per_sym*(HEADER_LEN+CRC_LEN);

        if((header_len+(*len)) <= pkt_size*num_packets)
            break;
        else
            num_packets++;
    }

    if(verbose)
        printf("Number of packets = %i\nfor data len = %i\n",num_packets,*len);
    
    // check in begining to see if len is less than packet size 

    int **new_data;
    new_data = (int **)malloc((num_packets)*sizeof(int));

    int pkt;
    for(pkt = 0; pkt < num_packets; pkt++)
        new_data[pkt] = (int *)malloc((pkt_size+PKT_HEADER)*sizeof(int));

    int curr_pos = 0;
    int curr_packet;
    for(curr_packet = 0; curr_packet < num_packets; curr_packet++)
    {
        /* Clear the current packet */
        int bit_clear;
        for(bit_clear = 0; bit_clear < pkt_size+PKT_HEADER; bit_clear++)
            new_data[curr_packet][bit_clear] = 0;

        new_data[curr_packet][0] = (pow(2,bits_per_sym)-1);
        new_data[curr_packet][1] = 0;
        new_data[curr_packet][pkt_size+2] = 0;

        if(header)
        {
            int header[HEADER_LEN+CRC_LEN];
            header_set(header, flags, curr_packet);
    //if(verbose)
    //{
    //    printf(" -- HEADER -- \n");
    //    int bit;
    //    for(bit = 0; bit < HEADER_LEN+CRC_LEN; bit++)
    //        printf("%i",header[bit]);
    //    printf("\n");
    //}
            int bit;
            for(bit = 0; bit < HEADER_LEN+CRC_LEN; bit++)
                new_data[curr_packet][bit+(PKT_HEADER-1)] = header[bit];
    //if(verbose)
    //{
    //    int bit;
    //    for(bit = 0; bit < HEADER_LEN+CRC_LEN+PKT_LEN-1; bit++)
    //        printf("%i",new_data[curr_packet][bit]);
    //    printf("\n -- HEADER -- \n");
    //}
        }

        /* Set in header here */
        // CRC accept char * header, int header_len,
        // return crc
        
        int bit;
        for(bit = 0; curr_pos < *len; bit++)
        {
            if(header)
            {
                if(bit+HEADER_LEN+CRC_LEN == pkt_size)
                    break;
                new_data[curr_packet][bit+(PKT_HEADER-1)+HEADER_LEN+CRC_LEN] = (data)[curr_pos];
            }
            else
            {
                if(bit == pkt_size)
                    break;
                new_data[curr_packet][bit+(PKT_HEADER-1)] = (data)[curr_pos];
            }
            curr_pos++;
        }
    //if(verbose)
    //{
    //    int bit;
    //    for(bit = 0; bit < PKT_SIZE+PKT_LEN-1; bit++)
    //        printf("%i",new_data[curr_packet][bit]);
    //    printf("\n -- HEADER -- \n");
    //}

        /* Copy the first part over !!! alternate -> send zeros */
        if(repeat)
        {
            if((*len) < pkt_size)
                for(i = (*len)+2; i < pkt_size+2; i++)
                    new_data[curr_packet][i] = new_data[curr_packet][i-*len];
        }
        else
        {
            //if(header)
            //{
            //    if(bit+HEADER_LEN+CRC_LEN < pkt_size)
            //        for(i = bit+HEADER_LEN+CRC_LEN+(PKT_HEADER-1); i < pkt_size+2; i++)
            //            new_data[curr_packet][i] = 0;
            //}
            //else
            //{
            //    if(bit < pkt_size)
            //        for(i = bit; i < pkt_size+2; i++)
            //            new_data[curr_packet][i] = 0;
            //}
        }
    }

    /* need to try and get this free to work */
    //free(*data);
    //(data) = new_data;
    *len = pkt_size+PKT_HEADER;
    *packet_count = num_packets;

    if(verbose)
        printf(" IN PACKETIZER END \n\n");

    return new_data;
}

/********************************************************************/
/*																	*/
/*  Function : bps_packer         		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int **data: data array pointer to be modified				*/
/*		int len: length of the data array    						*/
/*																	*/
/*	Return :        				    	    					*/
/*		int : length of the new data array    						*/
/*                                                                  */ 
/*	Purpose : Packetize the data, modify and return                 */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
int bps_packer(int **data, int len)
{
    if(verbose)
        printf("In BPS packer\n");

    int new_len;
    if(len%bits_per_sym)
        new_len = len/bits_per_sym + 1;
    else
        new_len = len/bits_per_sym;

    int* tmp_data;
    tmp_data = malloc(new_len * sizeof(int));

    int i = 0;
    for(i = 0; i < new_len; i++)
        tmp_data[i] = 0;

    i = 0;
    while(i < len) 
    {
        int curr_char;
        for(curr_char = 0; i < len && curr_char < bits_per_sym; curr_char++, i++)
            tmp_data[i/bits_per_sym] += (*data)[i] << (int)(bits_per_sym-1-curr_char);
        for(; curr_char < bits_per_sym; curr_char++)
            tmp_data[i/bits_per_sym] += 0 << (int)(bits_per_sym-1-i);
    }

    free((*data));
    (*data) = NULL;
    (*data) = tmp_data;

    if(verbose)
    {
        printf("BPS Mod data : \n");
        for(i = 0; i < new_len; i++)
            printf("data[%i] = %i\n",i,(*data)[i]);
    }

    if(verbose)
        printf("End BPS packer\n");

    return new_len;
}

/********************************************************************/
/*																	*/
/*  Function : header_set         		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int **data: data array pointer to be modified				*/
/*		int len: length of the data array    						*/
/*																	*/
/*	Return :        				    	    					*/
/*		int : length of the header          						*/
/*                                                                  */ 
/*	Purpose : Packetize the data, modify and return                 */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
int header_set(int header[HEADER_LEN+CRC_LEN], int flags[NUM_OPTIONS], int packet)
{
    if((HEADER_LEN+CRC_LEN)%bits_per_sym)
        error("Too many bits per symbol for desired header len");

    int address[ADDR_LEN];

    if(addr)
        addr_set(address);
    else
        memset(address,0,ADDR_LEN);

    int pkt_num = packet%((int)pow(2,PKT_HEADER)-1);
    
    char* file_name_alone;
    int file_name_len = 0;
    if(flags[FILE_TRANSMIT] && packet == 0 && tx_file_name)
    {
        char *tmp = strrchr(file_name, '/');
        if(tmp == NULL)
            file_name_alone = file_name;
        else
            file_name_alone = tmp + 1;

        file_name_len = strlen(file_name_alone);

        if(verbose)
            printf("File : %s \t name_len : %i\n",file_name_alone,file_name_len);

        if(file_name_len > pow(2,HDR_LEN))
            error("File Name too long");
    }

    int header_bit = 0;
    int bit;

    //flags[FILE_TRANSMIT] = 0; //!!!!!!!!!
    
        /* Set the Address */
        for(bit = 0; bit < ADDR_LEN; bit++, header_bit++)
            header[header_bit] = address[bit]; 
        /* Set the packet number */
        for(bit = 0; bit < PKT_LEN; bit++, header_bit++)
            header[header_bit] = ((pkt_num & 1<<((PKT_LEN-bit-1))) ? 1: 0); 
        /* Set the Header length */
        for(bit = 0; bit < HDR_LEN; bit++, header_bit++)
            header[header_bit] = ((file_name_len & 1<<((HDR_LEN-bit-1))) ? 1: 0); 
        /* Set the flags */
        for(bit = 0; bit < NUM_OPTIONS; bit++, header_bit++)
            header[header_bit] = (flags[bit] ? 1 : 0); 

   
        
    /* CRC Header Function */
    crc_set(header,CRC_LEN);

    //if(verbose)
    //{
    //    printf(" -- HEADER -- \n");
    //    for(bit = 0; bit < HEADER_LEN+CRC_LEN; bit++)
    //        printf("%i",header[bit]);
    //    printf("\n -- HEADER -- \n");
    //}

    return HEADER_LEN+CRC_LEN;
}

/********************************************************************/
/*																	*/
/*  Function : addr_set         		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int **data: data array pointer to be modified				*/
/*		int len: length of the data array    						*/
/*																	*/
/*	Return :        				    	    					*/
/*		int : length of the new data array    						*/
/*                                                                  */ 
/*	Purpose : Packetize the data, modify and return                 */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
void addr_set(int address[ADDR_LEN])
{
    if(verbose)
        printf("In Addr Setter\n");

    unsigned short address_bin[8];

    char2bin_int(address_bin,address_char);

    for(i = 0; i < ADDR_LEN; i++)
        address[i] = (int)address_bin[i];

//    if(verbose)
//        printf("End Addr Setter\n");
}

/********************************************************************/
/*																	*/
/*  Function : crc_set           		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int header: apply a crc to the first 21 bits of the header	*/
/*		int crc_len: length of the crc      						*/
/*																	*/
/*	Purpose : The function CRC is what adds the CRC to the header   */
/*	on the Tx side. It takes a binary vector of length 24 and an    */
/*	integer that tells it the length of the CRC. It then calculates */
/*	the CRC and fills it in to the end of the header.               */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
void crc_set(int header[HEADER_LEN+CRC_LEN], int crc_len)
{
	int num=0;
	int poly=0;
	
	// Create a polynomial of length crc_len with alternating 1's and 0's
    int i;
	for(i=0; i<crc_len; i++)
		if(i%2==0)
            poly += 1<<(crc_len-i-1);
	
	// Initialize the numerator of the polynomial division
	for(i=0; i<crc_len; i++)
		num += header[i]<<i;
	
	// Perform polynomial division
	for(i=0; i<24-(2*crc_len); i++)
    {
		if (num < poly)
			num = (num<<1) + header[i+crc_len];
		else
        {
			num = (num-poly);
			if(i!= HEADER_LEN){num=num<<1;}
		}
	}
	
	// Set the final designated CRC bits of the header
	for(i=0; i<crc_len; i++)
		header[i+HEADER_LEN] = (num>>(crc_len-i-1)) & 1;
}

/********************************************************************/
/*																	*/
/*  Function : crc_check           		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int header: header with CRC to check                    	*/
/*		int crc_len: length of the crc      						*/
/*																	*/
/*	Purpose : The function CRC_Check is what will validate the CRC  */
/*	on the Rx side. It takes the same arguments and calculates the  */
/*	CRC for the header in the same way as the CRC function. It      */
/*	then compares this calculated value to the actual value it      */
/*	received. It returns true if the CRC is valid and false if the  */
/*	CRC is invalid.                                                 */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
int crc_check(int header[HEADER_LEN+CRC_LEN], int crc_len)
{
	int num=0;
	int poly = 5; // 0b101
	
	// Initialize the numerator of the polynomial division
    int i;
	for(i=0; i<crc_len; i++)
		num += header[i]<<i;
	
	// Perform polynomial division
	for(i=0; i<24-(2*crc_len); i++)
    {
		if (num < poly)
			num = (num<<1) + header[i+crc_len];
		else
        {
			num = (num-poly);
			if(i!= 24-crc_len){num=num<<1;}
		}
	}
	
	// Compare calculated to received CRC
	int crc_rx=0;
	for(i=0; i<crc_len; i++)
		crc_rx += header[24-crc_len+i]<<(crc_len-i-1);

	return (num == crc_rx);
}

/********************************************************************/
/*																	*/
/*  Function : file_name_set      		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int **data: data array pointer to be modified				*/
/*		int *len: length of the data array    						*/
/*																	*/
/*	Purpose : Set the file name in the data, modify and return      */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
void file_name_set(int **data, int *len)
{
    if(verbose)
        printf("In File Name Setter\n");
    
    char* file_name_alone;
    int file_name_len = 0;

    char *tmp = strrchr(file_name, '/');
    if(tmp == NULL)
        file_name_alone = file_name;
    else
        file_name_alone = tmp + 1;

    file_name_len = strlen(file_name_alone);

    if(file_name_len > pow(2,HDR_LEN))
        error("File Name too long");

    int new_len = *len + file_name_len*8;
    int* tmp_data;
    tmp_data = malloc(new_len * sizeof(int));

    if(verbose)
        printf("SET IN DATA - File : %s \t name_len : %i \t bytes : %i \t new total bytes : %i\n",file_name_alone,file_name_len, file_name_len*8, new_len);

    int i = 0;
    int bit = 0;
    for(i = 0; i < file_name_len; i++)
    {
        unsigned short bin[8];
        char2bin_int(bin,file_name_alone[i]);

        for(bit = 0; bit < 8; bit++)
            tmp_data[i*8+bit] = bin[bit];
    }

    for(bit = 0; bit < *len; bit++)
        tmp_data[bit+file_name_len*8] = (*data)[bit];

    free((*data));
    (*data) = NULL;
    (*data) = tmp_data;

    *len = new_len;
}


/************************************************************************/
/********************** System Control Fucntions ************************/
/************************************************************************/


/********************************************************************/
/*																	*/
/*  Function : udelay       		                                */
/*																	*/
/*	Arguements : 							    					*/
/*	    int us: microseconds use in nanosleep   					*/
/*																	*/
/*	Purpose : Sleep while CPU does not need to be used              */
/*                                                                  */ 
/*  Example : udelay(50); // sleep for 50 microseconds               */
/*																	*/
/********************************************************************/
void udelay(int us)
{
    struct timespec ts = { 0, us * 1000 };

    nanosleep(&ts, NULL);
}

/********************************************************************/
/*																	*/
/*  Function : dma_disable      		                            */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Disable the DMA to prevent any error after the        */
/*	program closes                                                  */
/*                                                                  */ 
/*  Example : dma_disable(); // DMA turned off                    */
/*																	*/
/********************************************************************/
void dma_disable(void)
{
    if(verbose)
        printf("\n\n**************\n");

    clock_disconnect();

    if (dma_reg) 
    {
        dma_reg[DMA_CS] = BCM2708_DMA_RESET;
        udelay(10);
        if(verbose)
            printf("DMA Disabled\n");
    }

    if(verbose)
        printf("**************\n");
}

/********************************************************************/
/*																	*/
/*  Function : clock_disconnect      		                        */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Disable the DMA to prevent any error after the        */
/*                                                                  */ 
/*  Example : clock_disconnect(); // clock disconnected from GPIO   */
/*																	*/
/********************************************************************/
void clock_disconnect(void)
{
    gpio_reg[GPFSEL0_DMA] = (gpio_reg[GPFSEL0_DMA] & ~(7 << 12));
    udelay(10);
}

/********************************************************************/
/*																	*/
/*  Function : error              		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		char* fmt: used for fmt in printf       					*/
/*		... : rest of arguements    								*/
/*																	*/
/*	Purpose : Used to display any error and exit the program        */
/*                                                                  */ 
/*  Example : error("Exit Now"); // prints exit and exits           */
/*																	*/
/********************************************************************/
void error(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr,"\n");
    exit(0);
}

/********************************************************************/
/*																	*/
/*  Function : sig_handler       		                            */
/*																	*/
/*	Arguements : NONE     					    					*/
/*																	*/
/*	Purpose : Used to catch any signals from the OS (ex. EXIT)      */
/*                                                                  */ 
/********************************************************************/
void sig_handler() 
{
    if(verbose)
        printf("\nIn SigHandler");
    clock_disconnect();
    carrier_disable();
    dma_disable();
    exit(0);
}


/************************************************************************/
/************************* Memory Fucntions *****************************/
/************************************************************************/

/********************************************************************/
/*																	*/
/*  Function : mem_virt_to_phys              		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		uint32_t *virt:    								*/
/*																	*/
/*	Purpose : */
/*                                                                  */ 
/*  Example : */
/*																	*/
/********************************************************************/
uint32_t mem_virt_to_phys(void *virt)
{
    uint32_t offset = (uint8_t *)virt - virtbase;
    return page_map[offset >> PAGE_SHIFT].physaddr + (offset % PAGE_SIZE);
}

/********************************************************************/
/*																	*/
/*  Function : mem_phys_to_virt              		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		uint32_t phys:    								*/
/*																	*/
/*	Purpose : */
/*                                                                  */ 
/*  Example : */
/*																	*/
/********************************************************************/
uint32_t mem_phys_to_virt(uint32_t phys)
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

/********************************************************************/
/*																	*/
/*  Function : map_peripheral              		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		uint32_t base:       					*/
/*		uint32_t len:    								*/
/*																	*/
/*	Purpose : */
/*                                                                  */ 
/*  Example : */
/*																	*/
/********************************************************************/
void* map_peripheral(uint32_t base, uint32_t len)
{
    int fd = open("/dev/mem", O_RDWR|O_SYNC);
    void * vaddr;

    if (fd < 0)
        error("Failed to open /dev/mem: %m\n");
    vaddr = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, base);
    if (vaddr == MAP_FAILED)
        error("Failed to map peripheral at 0x%08x: %m\n", base);
    close(fd);

    return vaddr;
}


    /* need to set up center freq and other stuff */
    /* call an init function for DMA and PWM setup */


