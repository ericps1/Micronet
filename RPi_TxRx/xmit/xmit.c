/********************************************************************/
/*																	*/
/*  Michael Botkin							                        */
/*																	*/
/*	File Name : xmit.cpp      								        */
/*																	*/
/*	Description : Function set for the 					            */
/*																	*/
/*	Date created : 3/18/2014										*/
/*																	*/
/*	Revisions :	        											*/
/*      3/18/2014 - xmit, header with defines, memory/sys functs    */
/*      3/23/2014 - statics removed, xmiting, increased modularity  */
/*      3/26/2014 - dma moved to DMA xmit, regular now in xmit      */
/*      4/3/2014 - mod rate input now accepted                      */
/*      4/4/2014 - function unification                             */
/*      4/8/2014 - Packtizing, dma_ook, usage func, file handling   */
/*      4/10/2014 - file handling finished, no repeat, improving    */
/*      4/13/2014 - single transmit improved                        */
/*																	*/
/*	README :													 	*/
/*		Compiled with g++ on Ubuntu 12.04       					*/
/*      "g++ main.cpp xmit.cpp xmit.h"                              */
/*      "./a.out <center freq>"                                     */
/*																	*/
/********************************************************************/


#include "xmit.h"
#include "dma_xmit.h"



/************************************************************************/
/***************************  Functions  ********************************/
/************************************************************************/


/********************************************************************/
/*																	*/
/*  Function : init          		                                */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Initilize the xmit files and set up signal handlers   */
/*                                                                  */ 
/*  Example : init(); // Sig handlers setup and atexit dma_disable  */
/*																	*/
/********************************************************************/
void init(void)
{
    init_dma();
}

/********************************************************************/
/*																	*/
/*  Function : carrier_disable     		                            */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : disconnect the clock form the pin                     */
/*                                                                  */ 
/*  Example : carrier_disable(); //free up the pin from the clock   */
/*																	*/
/********************************************************************/
void carrier_disable(void)
{
    clock_disconnect();
}

/********************************************************************/
/*																	*/
/*  Function : carrier_setup           		                        */
/*																	*/
/*	Arguements :        					    					*/
/*		int mod_type: 0 for fsk and 1 for ook						*/  
/*																	*/
/*	Purpose : Enable GPIO 4 as a clock source and set center freq   */
/*                                                                  */ 
/*  Example : carrier_setup();                                      */
/*																	*/
/********************************************************************/
void carrier_setup(int mod_type)
{
    /* set up the virtual and physical memory */
    /* map the virtual pages */
    if(verbose)
        printf("Allocating and mapping memory");
    map_memory(XMIT);

    if(verbose)
        printf("setting up the clock\n");
    clock_connect(mod_type);

    fm_modulate(center_freq);
}

/********************************************************************/
/*																	*/
/*  Function : fm_modulate           		                        */
/*																	*/
/*	Arguements :        					    					*/
/*		double freq: frequency to set the clock to					*/  
/*																	*/
/*	Purpose : FM Modulation - Sets a new frequency for the GPIO CLK */
/*    Set value for Clock Manager General Purpose Control register  */
/*    Reference is set to PLLd = 500 MHz                            */
/*                                                                  */ 
/*  Example : fm_modulate();                                        */
/*																	*/
/********************************************************************/
void fm_modulate(double freq)
{
	// Define INT and FRAC values for PLL based on center frequency
	int INT = 500/freq;
	int FRAC = 4096*(500/(freq)-INT);
	// Set center frequency by modifying int and frac values of PLL
    clk_reg[GPCLK_DIV] = (0x5a << 24) + (INT << 12) + FRAC; // Modify the div value of the CM_GP0DIV register
}



/************************************************************************/
/************************* transmit Fucntions ***************************/
/************************************************************************/


/********************************************************************/
/*																	*/
/*  Function : fsk_transmit        		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int *data: data array to be transmitted						*/
/*		int len: length of the data array    						*/
/*																	*/
/*	Purpose : Transmit the data in the int array using the cpu      */
/*	and frequency shift keying                                      */
/*                                                                  */ 
/*  Example : fsk_transmit({1,0,1,0},4);                            */
/*																	*/
/********************************************************************/
void fsk_transmit(int *data, int len)
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

    carrier_setup(FSK); 

    /* ************************************* */
    /* End of other initialization functions */
    /* ************************************* */

    if(verbose)
        printf("Enetering FSK Lock Loop\n");

    double tsleep = 1000000/mod_freq;
    for (;;) 
    {
        for(curr_packet = 0; curr_packet < num_packets; curr_packet++)
        {
            int i;
            for(i=0; i<len; i++)
            {
                double freq = center_freq + ((double)packetized_data[curr_packet][i]-0.5)*((double)freq_deviation/1000);
                fm_modulate(freq);
                usleep(tsleep);
            }
        }
        if(!repeat)
            break;
    }

    carrier_disable();
}

/********************************************************************/
/*																	*/
/*  Function : ook_transmit        		                            */
/*																	*/
/*	Arguements :        					    					*/
/*		int *data: data array to be transmitted						*/
/*		int len: length of the data array    						*/
/*																	*/
/*	Purpose : Transmit the data in the int array using the cpu      */
/*	and on off keying                                               */
/*                                                                  */ 
/*  Example : ook_transmit({1,0,1,0},4);                            */
/*																	*/
/********************************************************************/
void ook_transmit(int *data, int len)
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
        //if(len>NUM_SAMPLES)
        //    error("Data Length is too long to transmit");
        //else
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

    carrier_setup(OOK); 

    /* ************************************* */
    /* End of other initialization functions */
    /* ************************************* */

    if(verbose)
        printf("Enetering OOK Lock Loop\n");

    double tsleep = 1000000/mod_freq;
    for (;;) 
    {
        for(curr_packet = 0; curr_packet < num_packets; curr_packet++)
        {
            int i;
            for(i=0;i<len; i++)
            {
                if(packetized_data[curr_packet][i] == 0)
                    clk_reg[GPCLK_CNTL] = 0x5A << 24 | 1 << 9 | 0 << 4 | 6;
                else
                    clk_reg[GPCLK_CNTL] = 0x5A << 24 | 1 << 9 | 1 << 4 | 6;

                usleep(tsleep);
            }
        }
        if(!repeat)
            break;
    }

    carrier_disable();
}



/************************************************************************/
/********************** System Control Fucntions ************************/
/************************************************************************/



