/********************************************************************/
/*																	*/
/*  Michael Botkin							*/
/*																	*/
/*	File Name : tcp_socket.h      								    */
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


#ifndef TCP_SOCKET_H_
#define TCP_SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>

#define PORT 3490
#define BACKLOG 10
#define BUFFER_LEN 1024

#define VERBOSE 1
#define CONFIG  1
#define IP      "192.168.10.2"
//#define IP      "127.0.0.1"

char config_string[BUFFER_LEN];
int curr_char;

//  memset(buffer,0,sizeof(buffer));
int yes;
int echo;
/* Socket variables end */


/************************************************************************/
/***************************  Functions  *******************************/
/************************************************************************/

/* init functions */
void init_tcp(int port);

void tcp_transmit(char *dat_in, int len, int config);

/* configure functions */
void configure(void);
void add_char(char new_char);
void clear_chars(void);

/* Individual Configure Functions */
void set_freq(int freq);
void set_mod_rate(int mod_rate);
void set_bps(int bps);
void set_tx_type(int tx_type);
void set_freq_deviation(int freq_dev);
void set_addr(char addr);
void set_pkt_len(int pkt_len);
void set_DSA_page(int dsa_page);
void set_flags(int disp_char, int disp_hex, int out);



/************************************************************************/
/********************** System Control Fucntions ************************/
/************************************************************************/

void error(char *fmt, ...);

#endif
