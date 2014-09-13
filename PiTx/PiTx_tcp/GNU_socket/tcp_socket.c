/********************************************************************/
/*																	*/
/*  Michael Botkin							*/
/*																	*/
/*	File Name : tcp_socket.c      								    */
/*																	*/
/*	Description : Function set for the 					            */
/*																	*/
/*	Date created : 4/18/2014										*/
/*																	*/
/*	Revisions :	        											*/
/*      4/18/2014 - TCP server                                      */
/*      4/19/2014 - TCP Configure                                   */
/*																	*/
/*	README :													 	*/
/*		Compiled with g++ on Ubuntu 12.04       					*/
/*																	*/
/********************************************************************/

#include "tcp_socket.h"
#include "dma_xmit.h"


/************************************************************************/
/***************************  Functions  *******************************/
/************************************************************************/


/********************************************************************/
/*																	*/
/*  Function : init_tcp          		                            */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Initilize the tcp files    */
/*                                                                  */ 
/*  Example : init_tcp(); // Sig handlers setup                     */
/*																	*/
/********************************************************************/
void init_tcp(int port)
{
    /* Socket intialize */
    yes = 1;

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) 
        error("Socket failure!!\n");

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
        error("setsockopt");

    memset(&server, 0, sizeof(server));
    memset(&dest,0,sizeof(dest));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY; 

    if ((bind(socket_fd, (struct sockaddr *)&server, sizeof(struct sockaddr )))== -1)    
        error("Binding Failure\n");
    /* Socket intialize End*/
}

/********************************************************************/
/*																	*/
/*  Function : configure          		                            */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Configure the transmitter, remove config data         */
/*                                                                  */ 
/*  Example : configure();                                          */
/*																	*/
/********************************************************************/
int configure(char *in)
{
    char dat_in[BUFFER_LEN];
    strcpy(dat_in,in);

    char *ptr_end;
    int tmp;

    char *token;
    token = strtok(dat_in,"-");
    while(token != NULL)
    {
        token = strtok(NULL,"-");
        if(token != NULL)
        {
            if(verbose)
                printf("Option to change : %c : ",token[0]);
            switch(token[0])
            {
                case 'f': 
                    tmp = strtol(++token,&ptr_end,0);
                    if(ptr_end != NULL && tmp > 0)
                    {
                        if(verbose)
                            printf("Center Freq set to %i\n",tmp);
                        center_freq = tmp;
                    }
                    break;
                case 'm': 
                    tmp = strtol(++token,&ptr_end,0);
                    if(ptr_end != NULL && tmp > 0)
                    {
                        if(verbose)
                            printf("Mod Freq set to %i\n",tmp);
                        mod_freq = tmp;
                    }
                    break;
                case 'd': 
                    tmp = strtol(++token,&ptr_end,0);
                    if(ptr_end != NULL && tmp > 0)
                    {
                        if(verbose)
                            printf("Freq Deviation set to %i\n",tmp);
                        freq_deviation = tmp;
                    }
                    break;
                case 'b': 
                    tmp = strtol(++token,&ptr_end,0);
                    if(ptr_end != NULL && tmp > 0)
                    {
                        if(verbose)
                            printf("Bits per Symbol set to %i\n",tmp);
                        bits_per_sym = tmp;
                    }
                    break;
                case 'p': 
                    tmp = strtol(++token,&ptr_end,0);
                    if(ptr_end != NULL && tmp > 0)
                    {
                        if(verbose)
                            printf("Pkt Size set to %i\n",tmp);
                        pkt_size = tmp;
                    }
                    break;
                case 'P': 
                    if(strstr(token, "on") != NULL)
                    {
                        if(verbose)
                            printf("Packetize set to on\n");
                        packetize = 1;
                    }
                    else
                    {
                        if(verbose)
                            printf("Packetize set to off\n");
                        packetize = 0;
                    }
                    break;
                case 'r': 
                    if(strstr(token, "on") != NULL)
                    {
                        if(verbose)
                            printf("Repeat set to on\n");
                        repeat = 1;
                    }
                    else
                    {
                        if(verbose)
                            printf("Repeat set to off\n");
                        repeat = 0;
                    }
                    break;
                case 'a': 
                    if(token+2 != NULL)
                    {
                        if(verbose)
                            printf("Address set to %c\n",*(token+2));
                        address_char = *(token+2);
                    }
                    break;
                case 'A': 
                    if(strstr(token, "on") != NULL)
                    {
                        if(verbose)
                            printf("Addressing set to on\n");
                        addr = 1;
                    }
                    else
                    {
                        if(verbose)
                            printf("Addressing set to off\n");
                        addr = 0;
                    }
                    break;
                case 'H': 
                    if(strstr(token, "on") != NULL)
                    {
                        if(verbose)
                            printf("Header set to on\n");
                        header = 1;
                    }
                    else
                    {
                        if(verbose)
                            printf("Header set to off\n");
                        header = 0;
                    }
                    break;
                case 't':
                    tmp = strtol(++token,&ptr_end,0);
                    if(ptr_end != NULL)
                    {
                        if(verbose)
                            printf("TX type set to %i\n",tmp);
                        tx_type = tmp;
                    }
                    break;
                case 'e': 
                    if(strstr(token, "on") != NULL)
                    {
                        if(verbose)
                            printf("Echo set to on\n");
                        repeat = 1;
                    }
                    else
                    {
                        if(verbose)
                            printf("Echo set to off\n");
                        repeat = 0;
                    }
                    break;
                default:
                    if(verbose)
                        printf("\n");
                    break;
            }
        }
    }

    if(verbose)
        printf("\n");

    return 1;
}
