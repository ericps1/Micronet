/********************************************************************/
/*																	*/
/*  Michael Botkin		                                           */
/*																	*/
/*	File Name : dma_pitx.h      								    */
/*																	*/
/*	Description : Function set for the 					            */
/*																	*/
/*	Date created : 3/18/2014										*/
/*																	*/
/*	Revisions :	        											*/
/*      3/18/2014 - pitx, header with defines, memory/sys functs    */
/*      3/23/2014 - statics removed, pitxing, increased modularity  */
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
/*      "g++ searchmap.cpp mapclass.cpp mapclass.h"                 */
/*      "./a.out input.txt output.txt && cat output.txt"            */
/*																	*/
/********************************************************************/


#ifndef DMA_pitx_H_
#define DMA_pitx_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


/************************************************************************/
/***************************  Constants  ********************************/
/************************************************************************/

/* Transmit Types */
#define FSK                         0
#define OOK                         1
#define DMA                         1
#define pitx                        0

/* Defaults */
#define PLLFREQ			            500000000	// PLLD is running at 500MHz
#define CARRIERFREQ		            175000000	// Carrier frequency is 100MHz
#define FREQ_SPACING                100         // In MHz
#define MOD_FREQ                    80000
#define BITS_PER_SYM                1
#define PKT_SIZE                    256 
#define MAX_PKT_SIZE                2048 
//#define PKT_SIZE                    32 
#define ADDR                        (char)'A'

/* Header Constants */
#define PKT_HEADER                  3 /* not in the head - start and stop bits */
#define ADDR_LEN                    8
#define HDR_LEN                     5
#define PKT_LEN                     3
#define NUM_OPTIONS                 5
#define HEADER_LEN                  (ADDR_LEN + HDR_LEN + PKT_LEN + NUM_OPTIONS) 
#define CRC_LEN                     3

/* Flags Bits */
#define FILE_TRANSMIT               0
#define OUTPUT_DATA                 1
#define COUT_DATA_HEX               2
#define COUT_DATA_CHAR              3
#define DSA_PAGE                    4

#define OFF                         0
#define ON                          1

/* File Constants */
#define FILE_SIZE                   64
#define FILE_DATA_BLOCK             1024 

/* Determines Pages Allocated */
#define NUM_SAMPLES	                ((MAX_PKT_SIZE+PKT_HEADER)*2)	
#define NUM_CBS			            (NUM_SAMPLES * 2)

#define BCM2708_DMA_NO_WIDE_BURSTS	(1<<26)
#define BCM2708_DMA_WAIT_RESP		(1<<3)
#define BCM2708_DMA_D_DREQ		    (1<<6)
#define BCM2708_DMA_PER_MAP(x)		((x)<<16)
#define BCM2708_DMA_END			    (1<<1)
#define BCM2708_DMA_RESET	    	(1<<31)
#define BCM2708_DMA_INT		    	(1<<2)

#define DMA_CS			            (0x00/4)
#define DMA_CONBLK_AD	            (0x04/4)
#define DMA_DEBUG		            (0x20/4)

/* bus addresses */
#define CM_GP0CTL                   (0x7e101070)
//#define GPFSEL0_DMA               (0x7E200000)
#define CM_GP0DIV                   (0x7e101074)
//#define CLKBASE                   (0x7E101000)
//#define DMABASE                   (0x7E007000)
//#define PWMBASE                   (0x7e20C000) /* PWM controller */

/* Using physical address instead of 
 * bus address at 0x7ennnnnn */
#define DMA_BASE		            0x20007000
#define DMA_LEN			            0x24
#define PWM_BASE		            0x2020C000
#define PWM_LEN			            0x28
#define CLK_BASE	                0x20101000
#define CLK_LEN			            0xA8
#define GPIO_BASE		            0x20200000
#define GPIO_LEN		            0xB4

#define PWM_CTL			            (0x00/4)
#define PWM_DMAC		            (0x08/4)
#define PWM_RNG1		            (0x10/4)

#define PWMCLK_CNTL		            40
#define PWMCLK_DIV		            41

#define GPCLK_CNTL		            (0x70/4)
#define GPCLK_DIV		            (0x74/4)

#define PWMCTL_MODE1	            (1<<1)
#define PWMCTL_PWEN1	            (1<<0)
#define PWMCTL_CLRF		            (1<<6)
#define PWMCTL_USEF1	            (1<<5)

#define PWMDMAC_ENAB	            (1<<31)
#define PWMDMAC_THRSHLD		        ((15<<8)|(15<<0))

#define GPFSEL0_DMA			        (0x00/4)

#define PAGE_SIZE	                4096
#define PAGE_SHIFT	                12
#define NUM_PAGES	                ((sizeof(struct control_data_s) + PAGE_SIZE - 1) >> PAGE_SHIFT)


/************************************************************************/
/**********************  Variables and Structs  *************************/
/************************************************************************/

/* Control Block Struct for the DMA */ 
typedef struct 
{
    uint32_t info, src, dst, length,
             stride, next, pad[2];
} dma_cb_t;

/* Struct used to allocate pages of memory */ 
/* and connect virtual and physical memory */
typedef struct 
{
    uint8_t *virtaddr;
    uint32_t physaddr;
} page_map_t;

/* Struct combining DMA CB and Data */ 
struct control_data_s 
{
    dma_cb_t cb[NUM_CBS];
    uint32_t sample[NUM_SAMPLES];
};

/* Struct used to allocate pages of memory */ 
/* and connect virtual and physical memory */
page_map_t *page_map;
/* control struct for virtual CB and data */
struct control_data_s *ctl;

/* virtually allocated memory pages */ 
uint8_t *virtbase;

/* access to physical registers */ 
volatile uint32_t *pwm_reg;
volatile uint32_t *clk_reg;
volatile uint32_t *dma_reg;
volatile uint32_t *gpio_reg;


int i, fd, pid;
int initialized;
int center_freq;    /* in MHz */
int mod_freq;       /* in MHz */
int pkt_size;       /* in bits */
int freq_deviation; /* in KHz */
int bits_per_sym;   
int packetize;
int repeat;
int addr;
char address_char;
int header;
int tx_file_name;

char file_name[FILE_SIZE+1]; /* 64 char limit on file name */
int flags[NUM_OPTIONS];

int verbose;


/************************************************************************/
/***************************  Functions  *******************************/
/************************************************************************/

/* init functions */
void init_dma(void);
void map_memory(int dma);
void pwm_init(void);
void dma_init(void);
void clock_connect(int mod_type);
void dma_cb_setup(int mod_type);

/* tear down functions */
void dma_disable(void);
void pwm_disable(void);
void clock_disconnect(void);

/* Transmit Functions */
void dma_fsk_transmit(int *data, int len);
void dma_ook_transmit(int *data, int len);

/* Transmit Helpers and Packet Functions */
int **packetizer(int *data, int *len, int *packet_count);
int bps_packer(int **data, int len);
int header_set(int header[HEADER_LEN+CRC_LEN], int flags[NUM_OPTIONS], int packet);
void addr_set(int address[ADDR_LEN]);
void crc_set(int header[HEADER_LEN+CRC_LEN], int crc_len); 
int crc_check(int header[HEADER_LEN+CRC_LEN], int crc_len);
void file_name_set(int **data, int *len);


/************************************************************************/
/********************** System Control Fucntions ************************/
/************************************************************************/

void udelay(int us);
void dma_disable(void);
void error(char *fmt, ...);

/* Signal Catcher */
void sig_handler();


/************************************************************************/
/************************* Memory Fucntions *****************************/
/************************************************************************/

uint32_t mem_virt_to_phys(void *virt);
uint32_t mem_phys_to_virt(uint32_t phys);
void* map_peripheral(uint32_t base, uint32_t len);

// End header file 

#endif
