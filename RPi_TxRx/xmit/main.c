/********************************************************************/
/*																	*/
/*  Botkin							                                */
/*																	*/
/*	File Name : main 									            */
/*																	*/
/*	Description : main function used to 			                */
/*																	*/
/*	Date created : 3/18/2014										*/
/*																	*/
/*	Revisions :														*/
/*      3/18/2014 - main set up                                     */
/*      3/23/2014 - statics removed, xmiting, increased modularity  */
/*      4/4/2014 - function unification                             */
/*      4/8/2014 - Packtizing, dma_ook, usage func, file handling   */
/*      4/10/2014 - file handling finished, no repeat, improving    */
/*      4/13/2014 - single transmit improved                        */
/*      4/15/2014 - RC CAR function                                 */
/*      4/16/2014 - Repeat defualt off                              */
/*      4/22/2014 - Addressing                                      */
/*																	*/
/*	README :													 	*/
/*		Compiled with g++ on Ubuntu 12.04       					*/
/*      "gcc main.c xmit.h xmit.c dma_xmit.h dma_xmit.c -o XMIT"    */
/*      "./a.out <center freq>"                                     */
/*																	*/
/*		Sometime you may need to use    							*/
/*	    gcc -c xmit.c -o xmit.o                                     */
/*	    gcc -c main.c -o main.o                                     */
/*      gcc main.o xmit.o -o XMIT                                   */
/*																	*/
/********************************************************************/


#include "dma_xmit.h"
#include "xmit.h"
#include "file_handler.h"


void usage()
{
    printf("Xmit -- Transmit using the Raspberry Pi\n");
    printf("Must run as sudo : sudo ./XMIT -f 100\n\n");
    printf("    h   :   Usage/Help\n");
    printf("    f   :   Center Frequency (MHz)      [%i MHz] \n",CARRIERFREQ/100000000);
    printf("    m   :   Modulation Rate (SPS)       [%i Symbols per sec] \n",MOD_FREQ);
    printf("    d   :   Frequency Deviation (kHZ)   [%i kHz]\n",FREQ_SPACING);
    printf("    b   :   Bits per symbol             [%i bps]\n",BITS_PER_SYM);
    printf("    p   :   Packet Size                 [%i bits]\n",PKT_SIZE);
    printf("    P   :   Don't Packetize             [Packetize On]\n");
    printf("    r   :   Repeat                      [Repeat Off]\n");
    printf("    v   :   Verbose\n");
    printf("    q   :   Quiet                       [Default]\n");
    printf("    F   :   Text File to transmit (file name 64 char limit)\n");
    printf("    c   :   command for the RC team \n");
    printf("    a   :   Don't use addresses         [Default on] \n");
    printf("    A   :   Address                     ['A'] \n");
}

int main(int argc, char **argv)
{
    verbose = 0;

    /* Initialize the xmit files */
    /* init calls init_dma as well */
    init();


    /* DEFAULTS */
    center_freq = CARRIERFREQ/pow(10,6);
    mod_freq = MOD_FREQ;
    bits_per_sym = BITS_PER_SYM;
    pkt_size = PKT_SIZE;
    freq_deviation = FREQ_SPACING;
    packetize = 1;
    strcpy(file_name,"No File");
    repeat = 0;
    file = 0;
    int command = 0;
    addr = 1;
    address_char = ADDR;

    int option;
    while ((option = getopt(argc,argv,"qvhPrac:f:m:d:b:p:F:A:")) != EOF)
    {
        switch (option) 
        {
            case 'h':   usage();                            return 0;
            case 'q':   verbose         = 0;                break;
            case 'v':   verbose         = 1;                break;
            case 'f':   center_freq     = atof(optarg);     break;
            case 'm':   mod_freq        = atof(optarg);     break;
            case 'd':   freq_deviation  = atof(optarg);     break;
            case 'b':   bits_per_sym    = atof(optarg);     break;
            case 'p':   pkt_size        = atof(optarg);     break;
            case 'P':   packetize       = 0;                break;
            case 'r':   repeat          = 1;                break;
            case 'F':   strcpy(file_name,strdup(optarg));   break;
            case 'c':   command         = atof(optarg);     break;
            case 'a':    addr           = 0;                break;
            case 'A':    address_char   = *optarg;          break;
            default:    usage();                            return 0;
        }
    }
        
    int *data;
    int num_samples = 16;

    if(strcmp(file_name,"No File"))
    {
        if(verbose)
            printf("File Name : %s\n",file_name);
        file = 1;
        num_samples = file_breakdown(file_name,&data);

        //error("\nFILE CHECK COMPLETE");
    }
    else
    {
        num_samples = 16;
        data = malloc(num_samples*sizeof(int));
	    int tmp_data[16] = {1,0,1,0,1,1,1,1,0,0,0,0,1,1,0,1}; 
        int i;
        for(i = 0; i < num_samples; i++)
            data[i] = tmp_data[i];
    }

    if(command)
    {
        mod_freq = (int)(1/0.00057);
        center_freq = 49.8;
        packetize = 0;

        /* Forward */
        if(command == 1)
        {
            num_samples = 4*4+10*2; // 36
            data = malloc(num_samples*(sizeof(int)));
            int tmp_data[36] = {1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0}; 

            int i;
            for(i = 0; i < num_samples; i++)
                data[i] = tmp_data[i];
        }
        /* Backwards */
        if(command == 2)
        {
            num_samples = 4*4+40*2; // 36
            data = malloc(num_samples*(sizeof(int)));
            int tmp_data[96] = {1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0}; 

            int i;
            for(i = 0; i < num_samples; i++)
                data[i] = tmp_data[i];
        }
        /* Forward Right */
        if(command == 3)
        {
            num_samples = 4*4+28*2; // 36
            data = malloc(num_samples*(sizeof(int)));
            int tmp_data[72] = {1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0}; 

            int i;
            for(i = 0; i < num_samples; i++)
                data[i] = tmp_data[i];
        }
        /* Forward Left */
        if(command == 4)
        {
            num_samples = 4*4+34*2; // 36
            data = malloc(num_samples*(sizeof(int)));
            int tmp_data[84] = {1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0}; 

            int i;
            for(i = 0; i < num_samples; i++)
                data[i] = tmp_data[i];
        }
        if(command == 5)
        {
            num_samples = 4*4+10*2; // 36
            data = malloc(num_samples*(sizeof(int)));
            int tmp_data[36] = {1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0}; 

            int i;
            for(i = 0; i < num_samples; i++)
                data[i] = tmp_data[i];
        }
        if(command == 6)
        {
            num_samples = 4*4+10*2; // 36
            data = malloc(num_samples*(sizeof(int)));
            int tmp_data[36] = {1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0}; 

            int i;
            for(i = 0; i < num_samples; i++)
                data[i] = tmp_data[i];
        }
    }

    
    printf("\nRun which test?");
    printf("\n[0] dma fsk");
    printf("\n[1] dma ook");
    printf("\n[2] fsk");
    printf("\n[3] ook\n");

    int selection;
    while(scanf("%i",&selection) != 1);

    switch(selection)
    {
        case 0:
            /***************** DMA_FSK TEST ********************/
            dma_fsk_transmit(data,num_samples);
            break;
        case 1:
            /***************** DMA_OOK TEST ********************/
            dma_ook_transmit(data,num_samples);
            break;
        case 2:
            /******************* FSK TEST **********************/
            fsk_transmit(data, num_samples);
            break;
        case 3:
            /******************* OOK TEST **********************/
            ook_transmit(data, num_samples);
            break;
    }

    error("terminate");
    return 0;
}

