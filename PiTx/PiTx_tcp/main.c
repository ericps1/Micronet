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
/*      4/18/2014 - TCP Set up                                      */
/*      4/19/2014 - TCP Configure                                   */
/*      4/22/2014 - Addressing                                      */
/*      4/28/2014 - Header && CRC                                   */
/*      5/1/2014 - Flags Control                                    */
/*																	*/
/*	README :													 	*/
/*		Compiled with g++ on Ubuntu 12.04       					*/
/*      "gcc main.c xmit.h xmit.c dma_xmit.h dma_xmit.c tcp_socket.c tcp_socket.h -o TCP_XMIT"    */
/*      "sudo ./TCP_XMIT -f 100"                                    */
/*																	*/
/*		Sometime you may need to use    							*/
/*	    gcc -c xmit.c -o xmit.o                                     */
/*	    gcc -c dma_xmit.c -o dma_xmit.o                             */
/*	    gcc -c tcp_socket.c -o tcp_socket.o                         */
/*	    gcc -c main.c -o main.o                                     */
/*      gcc main.o xmit.o dma_xmit.o tcp_socket.o -o TCP_XMIT       */
/*      "sudo ./TCP_XMIT -f 100"                                    */
/*																	*/
/********************************************************************/


#include "dma_xmit.h"
#include "xmit.h"
#include "file_handler.h"
#include "tcp_socket.h"


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
    printf("    a   :   Don't use addresses         [Default on] \n");
    printf("    A   :   Address                     ['A'] \n");
    printf("    H   :   Don't use a header          [Default on]  \n");
    printf("    N   :   Don't tx a file name        [Default on]  \n");
    printf("    D   :   Set DSA pasge to ON         [Default off] \n");
    printf("    X   :   Set GNU Hex display to ON   [Default off] \n");
    printf("    C   :   Set GNU Char display to OFF [Default on] \n");
    printf("    O   :   Set GNU output to off       [Default on]  \n");
    printf("    t   :   TX Type                     [DMA_FSK]\n\t:   0 = DMA_FSK   1 = DMA_OOK   2 = FSK   3 = OOK\n");
    printf("    e   :   Echo                        [TCP ECHO off]\n");
}

int main(int argc, char **argv)
{
    init_tcp(PORT);

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

    flags[FILE_TRANSMIT]    = OFF;
    flags[OUTPUT_DATA]      = ON;
    flags[COUT_DATA_HEX]    = OFF;
    flags[COUT_DATA_CHAR]   = ON;
    flags[DSA_PAGE]         = OFF;

    tx_type = 0;

    echo = 0;

    int option;
    while ((option = getopt(argc,argv,"qvhNPerHADXCOt:f:m:d:b:p:F:")) != EOF)
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
            case 'a':   addr            = 0;                break;
            case 'A':   address_char    = *optarg;          break;
            case 'H':   header          = 0;                break;
            case 'N':   tx_file_name    = OFF;              break;
            case 'D':   flags[DSA_PAGE]        = ON;        break;
            case 'X':   flags[COUT_DATA_HEX]   = ON;        break;
            case 'C':   flags[COUT_DATA_CHAR]  = OFF;       break;
            case 'O':   flags[OUTPUT_DATA]     = OFF;       break;
            case 't':   tx_type         = atof(optarg);     break;
            case 'e':   echo            = atof(optarg);     break;
            default:    usage();                            return 0;
        }
    }

    if ((listen(socket_fd, BACKLOG)) == -1)
        error("Listening Failure\n");
   

    /* ******************************** */
    /* ***  Main Lock Loop For TCP  *** */
    /* ******************************** */

    while(1) 
    {
        size = sizeof(struct sockaddr_in);

        if((client_fd = accept(socket_fd, (struct sockaddr *)&dest, &size))==-1 ) 
            error("accept");

        if(verbose)
            printf("Server got connection from client %s\n", inet_ntoa(dest.sin_addr));

        while(1) 
        {
            if((num = recv(client_fd, buffer, BUFFER_LEN,0))== -1) 
                error("recv");
            else if (num == 0) 
            {
                if(verbose)
                    printf("Connection closed\n");
                //So I can now wait for another client
                    break;
            }
            buffer[num] = '\0';
            if(verbose)
                printf("Server:Msg Received %s\n", buffer);
            char *config_check = strstr(buffer,"CONFIG");
            if(config_check != NULL)
            {
                configure(buffer);
                continue;
            }

            // use the num*8 to allocate memory
            // and then copy over the data
            // from the buffer for loop n
            // after converting to binary

            if(echo)
            {
                if ((send(client_fd,buffer, strlen(buffer),0))== -1) 
                {
                    if(verbose)
                        printf("Failure Sending Message\n");
                    close(client_fd);
                    break;
                }
            }

            printf("Server:Msg being sent: %s\nNumber of bytes sent: %d\n",buffer, strlen(buffer));

            /* TX SECTION */
            int *data = NULL;
            int num_samples = 16;

            if(strcmp(file_name,"No File"))
            {
                if(verbose)
                    printf("File Name : %s\n",file_name);

                num_samples = file_breakdown(file_name,&data);

                flags[FILE_TRANSMIT] = 1;

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

            if(strlen(buffer) > 0)
            {
                num_samples = strlen(buffer)*8;
                data = malloc(num_samples*sizeof(int));
                data = string_breakdown(buffer);
            }


            if(verbose)
                printf("TX TYPE = %i\n\n",tx_type);
            switch(tx_type)
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
                default:
                    break;
            }

            free(data);
            data = NULL;

            } //End of Inner While...
            //Close Connection Socket
            close(client_fd);

        } //Outer While

        close(socket_fd);


        error("terminate");
        return 0;
    }

