/********************************************************************/
/*																	*/
/*  Michael Botkin		                                           */
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
/*      4/28/2014 - Header && CRC                                   */
/*      5/1/2014 - Flags Control                                    */
/*																	*/
/*	README :													 	*/
/*		Compiled with g++ on Ubuntu 12.04       					*/
/*      "gcc main.c xmit.h xmit.c dma_xmit.h dma_xmit.c -o XMIT"    */
/*      "sudo ./XMIT -f 100"                                        */
/*																	*/
/*		Sometime you may need to use    							*/
/*	    gcc -c xmit.c -o xmit.o                                     */
/*	    gcc -c dma_xmit.c -o dma_xmit.o                             */
/*	    gcc -c main.c -o main.o                                     */
/*      gcc main.o xmit.o dma_xmit.o -o XMIT                       */
/*      "sudo ./XMIT -f 100"                                        */
/*																	*/
/********************************************************************/


#include "dma_xmit.h"
#include "xmit.h"
#include "file_handler.h"

// add new updates to TCP
// Make this more readable (Usage)

void usage()
{
    printf("Xmit -- Transmit using the Raspberry Pi\n");
    printf("Must run as sudo : sudo ./XMIT -f 100\n\n");
    printf("    h   :   Usage/Help\n");
    printf("    f   :   Center Frequency (MHz)      [%i MHz] \n",CARRIERFREQ/1000000);
    printf("    m   :   Modulation Rate (SPS)       [%i Symbols per sec] \n",MOD_FREQ);
    printf("    d   :   Frequency Deviation (kHZ)   [%i kHz]\n",FREQ_SPACING);
    printf("    b   :   Bits per symbol             [%i bps]\n",BITS_PER_SYM);
    printf("    p   :   Packet Size                 [%i bits]\n",PKT_SIZE);
    printf("    P   :   Don't Packetize             [Packetize On]\n");
    printf("    r   :   Repeat                      [Repeat Off]\n");
    printf("    v   :   Verbose\n");
    printf("    q   :   Quiet                       [Default]\n");
    printf("    F   :   Text File to transmit (file name 64 char limit)\n");
    printf("    A   :   Don't use addresses         [Default on] \n");
    printf("    a   :   Address                     ['A'] \n");
    printf("    H   :   Don't use a header          [Default on]  \n");
    printf("    N   :   Don't tx a file name        [Default on]  \n");
    printf("    D   :   Set DSA pasge to ON         [Default off] \n");
    printf("    X   :   Set GNU Hex display to ON   [Default off] \n");
    printf("    C   :   Set GNU Char display to OFF [Default on] \n");
    printf("    O   :   Set GNU output to off       [Default on]  \n");
    printf("    0   :   Override flags for testing  [Default off]  \n");
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
    packetize = ON;
    strcpy(file_name,"No File");
    repeat = OFF;


    addr = ON;
    address_char = ADDR;
    header = ON;
    tx_file_name = ON;
    int override = 0;

    flags[FILE_TRANSMIT]    = OFF;
    flags[OUTPUT_DATA]      = ON;
    flags[COUT_DATA_HEX]    = OFF;
    flags[COUT_DATA_CHAR]   = ON;
    flags[DSA_PAGE]         = OFF;

    int option;
    while ((option = getopt(argc,argv,"qvhHNPrADXCOof:m:d:b:p:F:a:")) != EOF)
    {
        switch (option) 
        {
            case 'h':   usage();                            return 0;
            case 'q':   verbose         = OFF;              break;
            case 'v':   verbose         = ON;               break;
            case 'f':   center_freq     = atof(optarg);     break;
            case 'm':   mod_freq        = atof(optarg);     break;
            case 'd':   freq_deviation  = atof(optarg);     break;
            case 'b':   bits_per_sym    = atof(optarg);     break;
            case 'p':   pkt_size        = atof(optarg);     break;
            case 'P':   packetize       = OFF;              break;
            case 'r':   repeat          = ON;               break;
            case 'F':   strcpy(file_name,strdup(optarg));   break;
            case 'A':   addr            = OFF;              break;
            case 'a':   address_char    = *optarg;          break;
            case 'H':   header          = OFF;              break;
            case 'N':   tx_file_name    = OFF;              break;
            case 'D':   flags[DSA_PAGE]        = ON;        break;
            case 'X':   flags[COUT_DATA_HEX]   = ON;        break;
            case 'C':   flags[COUT_DATA_CHAR]  = OFF;       break;
            case 'O':   flags[OUTPUT_DATA]     = OFF;       break;
            case 'o':   override                = 1;        break;
            default:    usage();                            return 0;
        }
    }
        
    int *data;
    int num_samples = 16;
    int file_curr_block = 0;

    if(strcmp(file_name,"No File") && !override)
    {
        if(verbose)
            printf("File Name : %s\n",file_name);

        num_samples = file_breakdown(file_name,&data,file_curr_block);
        file_curr_block++;

        flags[FILE_TRANSMIT] = ON;

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
    
    if(override)
    {
        flags[FILE_TRANSMIT]    = OFF;
        flags[OUTPUT_DATA]      = OFF;
        flags[COUT_DATA_HEX]    = OFF;
        flags[COUT_DATA_CHAR]   = OFF;
        flags[DSA_PAGE]         = OFF;

        num_samples = binary_file_breakdown(file_name,&data,file_curr_block);
    }

    
    printf("\nRun which test?");
    printf("\n[0] dma fsk");
    printf("\n[1] dma ook");
    printf("\n[2] fsk");
    printf("\n[3] ook\n");

    int selection;
    while(scanf("%i",&selection) != 1);

    while(num_samples)
    {
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

        num_samples = 0;

        if(flags[FILE_TRANSMIT])
        {
            num_samples = file_breakdown(file_name,&data,file_curr_block);
            file_curr_block++;
            tx_file_name = 0;
        }
    }

    error("terminate");
    return 0;
}

