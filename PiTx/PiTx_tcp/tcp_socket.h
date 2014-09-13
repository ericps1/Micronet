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
/*      4/18/2014 - TCP server    */
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

#define PORT 3490
#define BACKLOG 10
#define BUFFER_LEN 1024

/* Socket variables */
struct sockaddr_in server;
struct sockaddr_in dest;
int socket_fd, client_fd,num;
socklen_t size;

char buffer[BUFFER_LEN];

//  memset(buffer,0,sizeof(buffer));
int yes;
int echo;
/* Socket variables end */


/************************************************************************/
/***************************  Functions  *******************************/
/************************************************************************/

/* init functions */
void init_tcp(int port);

/* configure functions */
int configure(char *dat_in);

#endif
