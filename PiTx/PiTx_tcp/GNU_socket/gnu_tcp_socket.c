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
/*      4/24/2014 - TCP Configure Updated                           */
/*      5/3/2014 - Adapted to control Raspberry Pi from GNU Radio   */
/*																	*/
/*	README :													 	*/
/*		Compiled with g++ on Ubuntu 12.04       					*/
/*																	*/
/********************************************************************/

#include "gnu_tcp_socket.h"


/************************************************************************/
/***************************  Functions  *******************************/
/************************************************************************/


/********************************************************************/
/*																	*/
/*  Function : configure          		                            */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Configure the transmitter, format and pass the        */
/*	received configure packet                                       */
/*                                                                  */ 
/*  Example : configure();                                          */
/*																	*/
/********************************************************************/
void configure()
{
    char dat_in[BUFFER_LEN];
    strcpy(dat_in,config_string);

    tcp_transmit(dat_in, curr_char, CONFIG);
    clear_chars();
}

/********************************************************************/
/*																	*/
/*  Function : tcp_transmit          		                            */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Configure the transmitter, format and pass the        */
/*	received configure packet                                       */
/*                                                                  */ 
/*  Example : tcp_transmit("CONFIGURE -f 128",16);                                          */
/*																	*/
/********************************************************************/
void tcp_transmit(char *dat_in, int len, int config)
{
    /* Check to make sure that the data has configure in it
     * if not add it */
    char buffer[1024];
    dat_in[len] = '\0';
    if(config && strstr(dat_in,"CONFIGURE") == NULL)
    {
        buffer[0] = 'C';
        buffer[1] = 'O';
        buffer[2] = 'N';
        buffer[3] = 'F';
        buffer[4] = 'I';
        buffer[5] = 'G';
        buffer[6] = 'U';
        buffer[7] = 'R';
        buffer[8] = 'E';
        buffer[9] = ' ';

        printf("IN CONFIGURE\n");

        int byte;
        for(byte = 0; byte < len; byte++)
            buffer[10+byte] = dat_in[byte];

        buffer[len+10] = '\0';
    }
    else
    {
        int byte;
        for(byte = 0; byte < len; byte++)
            buffer[byte] = dat_in[byte];
        buffer[len] = '\0';
       // strcpy(buffer,dat_in);
    }


    struct sockaddr_in server_info;
    struct hostent *he;
    int socket_fd,num;

    //if ((he = gethostbyname("127.0.0.1"))==NULL) 
    if ((he = gethostbyname(IP))==NULL) 
    {
        fprintf(stderr, "Cannot get host name\n");
        exit(1);
    }

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Socket Failure!!\n");
        exit(1);
    }

    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);
    server_info.sin_addr = *((struct in_addr *)he->h_addr);
    if (connect(socket_fd, (struct sockaddr *)&server_info, sizeof(struct sockaddr))<0) {
        //fprintf(stderr, "Connection Failure\n");
        perror("connect");
        exit(1);
    }

    printf("freq to be sent \"%s\" of length %i\n",buffer,strlen(buffer));
    if ((send(socket_fd,&buffer, strlen(buffer),0))== -1) 
    {
        fprintf(stderr, "Failure Sending Message\n");
        close(socket_fd);
        exit(1);
    }
    close(socket_fd);
    curr_char = 0;
}

void set_freq(int freq)
{
    /* In MHZ */
    char conf[1024]; 
    sprintf(conf,"CONFIGURE -f %i",freq);
    printf("freq to be sent \"%s\" of length %i\n",conf,strlen(conf));
    
    tcp_transmit(conf, strlen(conf), CONFIG);
}
void set_mod_rate(int mod_rate)
{
    /* In MHZ */
    char conf[1024]; 
    sprintf(conf,"CONFIGURE -m %i",mod_rate);
    
    tcp_transmit(conf, strlen(conf), CONFIG);
}
void set_bps(int bps)
{
    /* In MHZ */
    char conf[1024]; 
    sprintf(conf,"CONFIGURE -b %i",bps);
    
    tcp_transmit(conf, strlen(conf), CONFIG);
}
void set_tx_type(int tx_type)
{
    /* In MHZ */
    char conf[1024]; 
    sprintf(conf,"CONFIGURE -t %i",tx_type);
    
    tcp_transmit(conf, strlen(conf), CONFIG);
}
void set_freq_deviation(int freq_dev)
{
    /* In MHZ */
    char conf[1024]; 
    sprintf(conf,"CONFIGURE -d %i",freq_dev);
    
    tcp_transmit(conf, strlen(conf), CONFIG);
}
void set_addr(char addr)
{
    /* In MHZ */
    char conf[1024]; 
    sprintf(conf,"CONFIGURE -a %c",addr);
    
    tcp_transmit(conf, strlen(conf), CONFIG);
}
void set_pkt_len(int pkt_len)
{
    /* In MHZ */
    char conf[1024]; 
    sprintf(conf,"CONFIGURE -p %i",pkt_len);
    
    tcp_transmit(conf, strlen(conf), CONFIG);
}
void set_DSA_page(int dsa_page)
{
    /* In MHZ */
    char conf[1024]; 
    if(dsa_page)
        sprintf(conf,"CONFIGURE -D on");
    else
        sprintf(conf,"CONFIGURE -D off");
    
    tcp_transmit(conf, strlen(conf), CONFIG);
}
void set_flags(int disp_char, int disp_hex, int out)
{
    /* In MHZ */
    char conf[1024]; 
    int flags = 0;
    flags = (disp_char? 1 : 0);
    flags = (flags<<1)+(disp_hex? 1 : 0);
    flags = (flags<<1)+(out? 1 : 0);
    sprintf(conf,"CONFIGURE -O %i",flags);
    
    tcp_transmit(conf, strlen(conf), CONFIG);
}

/********************************************************************/
/*																	*/
/*  Function : add_char          		                            */
/*																	*/
/*	Arguements :         					    					*/
/*		char new_char : next char to be added to the config string  */
/*																	*/
/*	Purpose : Configure the transmitter, remove config data         */
/*                                                                  */ 
/*  Example : add_char('-');                                        */
/*																	*/
/********************************************************************/
void add_char(char new_char)
{
    config_string[curr_char++] = new_char;
}

/********************************************************************/
/*																	*/
/*  Function : clear_chars        		                            */
/*																	*/
/*	Arguements : NONE   					    					*/
/*																	*/
/*	Purpose : Configure the transmitter, remove config data         */
/*                                                                  */ 
/*  Example : clear_chars();                                        */
/*																	*/
/********************************************************************/
void clear_chars()
{
    for(curr_char = 0; curr_char < BUFFER_LEN; curr_char++)
        config_string[curr_char] = '\0';
    curr_char = 0;
}


