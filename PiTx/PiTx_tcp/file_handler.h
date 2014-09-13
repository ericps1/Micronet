/********************************************************************/
/*																	*/
/*  Michael Botkin							*/
/*																	*/
/*	File Name : file_handler.h      								    */
/*																	*/
/*	Description : Function set for the 					            */
/*																	*/
/*	Date created : 4/8/2014										*/
/*																	*/
/*	Revisions :	        											*/
/*      4/8/2014 - Packtizing, dma_ook, usage func, file handling   */
/*																	*/
/*	README :													 	*/
/*		Compiled with g++ on Ubuntu 12.04       					*/
/*      "g++ searchmap.cpp mapclass.cpp mapclass.h"                 */
/*      "./a.out input.txt output.txt && cat output.txt"            */
/*																	*/
/********************************************************************/


#ifndef FILE_HANDLER_H_
#define FILE_HANDLER_H_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define FILE_SIZE                   64
#define FILE_DATA_BLOCK             1024 

#define CHAR_LEN 8

extern int verbose;
extern void error(char *fmt, ...);

char *char2bin_ch(char dst[8], unsigned char value);

/* Uses ints instead of chars */
unsigned short *char2bin_int(unsigned short dst[8], unsigned char value);

/* Uses ints instead of chars */
char bin2char(unsigned char bit[8]);

/* break the file down to binary */
int file_breakdown(char* file_name, int** data);
/* break the string down to binary */
int* string_breakdown(char* in);

#endif
