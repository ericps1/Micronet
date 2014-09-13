/********************************************************************/
/*																	*/
/*  ECE 2574, Homework 3, Michael Botkin							*/
/*																	*/
/*	File Name : mapclass.h      								    */
/*																	*/
/*	Description : Function set for the 					            */
/*																	*/
/*	Date created : 3/18/2014										*/
/*																	*/
/*	Revisions :	        											*/
/*      3/18/2014 - pitx, header with defines, memory/sys functs    */
/*      3/23/2014 - statics removed, pitxing, increased modularity  */
/*      4/4/2014 - function unification                             */
/*      4/8/2014 - Packtizing, dma_ook, usage func, file handling   */
/*      4/10/2014 - file handling finished, no repeat, improving    */
/*      4/13/2014 - single transmit improved                        */
/*																	*/
/*	README :													 	*/
/*		Compiled with g++ on Ubuntu 12.04       					*/
/*      "g++ searchmap.cpp mapclass.cpp mapclass.h"                 */
/*      "./a.out input.txt output.txt && cat output.txt"            */
/*																	*/
/********************************************************************/


#ifndef pitx_H_
#define pitx_H_


/************************************************************************/
/***************************  Constants  ********************************/
/************************************************************************/



/************************************************************************/
/**********************  Variables and Structs  *************************/
/************************************************************************/



/************************************************************************/
/***************************  Functions  *******************************/
/************************************************************************/

/* init functions */
void init(void);
void carrier_setup(int mod_type);

/* tear down functions */
void carrier_disable(void);

/* Transmit Functions */
void fm_modulate(double freq);
void fsk_transmit(int *data, int len);
void ook_transmit(int *data, int len);


/************************************************************************/
/********************** System Control Fucntions ************************/
/************************************************************************/




/************************************************************************/
/************************* Memory Fucntions *****************************/
/************************************************************************/


// End header file 

#endif
