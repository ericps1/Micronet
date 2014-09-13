#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <iostream>
using namespace std;

int  mem_fd;

// I/O access
volatile unsigned *gpio;
volatile unsigned *allof7e;

// Macros for register operations
#define ACCESS(base) *(volatile int*)((int)allof7e+base-0x7e000000)
#define SETBIT(base, bit) ACCESS(base) |= 1<<bit
#define CLRBIT(base, bit) ACCESS(base) &= ~(1<<bit)

// Addresses for relevant registers
#define CM_GP0CTL (0x7e101070)
#define GPFSEL0 (0x7E200000)
#define CM_GP0DIV (0x7e101074)
#define CLKBASE (0x7E101000)
#define DMABASE (0x7E007000)
#define PWMBASE  (0x7e20C000) /* PWM controller */

// Struct for the Clock Manager General Purpose Control register
struct GPCTL {
    char SRC         : 4;
    char ENAB        : 1;
    char KILL        : 1;
    char             : 1;
    char BUSY        : 1;
    char FLIP        : 1;
    char MASH        : 2;
    unsigned int     : 13;
    char PASSWD      : 8;
};

// Enable GPIO 4 as a clock source and set center frequency
void carrier_setup(double center_freq)
{

    // open /dev/mem, which provides access to physical memory
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        printf("can't open /dev/mem \n");
        exit (-1);
    }
    
	// define a pointer to I/O peripherals in /dev/mem using mmap
    allof7e = (unsigned *)mmap(
                  NULL,
                  0x01000000,  //len
                  PROT_READ|PROT_WRITE,
                  MAP_SHARED,
                  mem_fd,
                  0x20000000  // (I/O physical memory)
              );

    if ((int)allof7e==-1) exit(-1);

	// Set the function of GPIO 4 to alternate function 0
    SETBIT(GPFSEL0 , 14);
    CLRBIT(GPFSEL0 , 13);
    CLRBIT(GPFSEL0 , 12);

	// Set value for Clock Manager General Purpose Control register
	// Reference is set to PLLd = 500 MHz
    struct GPCTL setupword = {6/*SRC*/, 1, 0, 0, 0, 1,0x5a};
    ACCESS(CM_GP0CTL) = *((int*)&setupword);
	
	// Define INT and FRAC values for PLL based on center frequency
	int INT = 500/center_freq;
	int FRAC = 4096*(500/center_freq-INT);

	// Set center frequency by modifying int and frac values of PLL
	ACCESS(CM_GP0DIV) = (0x5a << 24) + (INT << 12) + FRAC; // Modify the div value of the CM_GP0DIV register
}

// FM Modulation - Sets a new frequency for the GPIO CLK
void fm_modulate(double freq)
{
	int INT = 500/freq;
	int FRAC = 4096*(500/(freq)-INT);
    ACCESS(CM_GP0DIV) = (0x5a << 24) + (INT << 12) + FRAC; // Modify the div value of the CM_GP0DIV register
}

// FSK Modulation - Transmits a vector of data by updating the divider of the GPIO CLK PLL according to passed parameters
void fsk_modulate(int data[], int length, double center_freq, double freq_space, double rate)
{
	double tsleep = 1000000/rate;
	for(int i=0; i<length; i++){
		double freq = center_freq + ((double)data[i]-0.5)*freq_space/2;
		int INT = 500/freq;
		int FRAC = 4096*(500/(freq)-INT);
    		ACCESS(CM_GP0DIV) = (0x5a << 24) + (INT << 12) + FRAC; // Modify the div value of the CM_GP0DIV register
		usleep(tsleep);
	}
}

// OOK Modulation - Transmits a vector of data by enabling/disabling GPIO CLK according to passed parameters
void OOK_modulate(int data[],int length, double rate)
{
	double tsleep = 1000000/rate;
	for(int i=0;i<length; i++)
	{
		if(data[i] == 0){
			// We either need to clear the enable bit or set the kill bit to disable the CLK
			struct GPCTL setupword = {6/*SRC*/, 0, 0, 0, 0, 1,0x5a};
    			ACCESS(CM_GP0CTL) = *((int*)&setupword);
			//CLRBIT(CM_GP0CTL,4);
			//SETBIT(CM_GP0CTL,5);
		}
		else{
			// We either need to set the enable bit or clear the kill bit to disable the CLK
			struct GPCTL setupword = {6/*SRC*/, 1, 0, 0, 0, 1,0x5a};
    			ACCESS(CM_GP0CTL) = *((int*)&setupword);			
			//SETBIT(CM_GP0CTL,4);
			//CLRBIT(CM_GP0CTL,5);
		}
		usleep(tsleep);
	}
}

int main() {
	// Simple test of FM abilities. First we enable the GPIO clock and set its frequency
	// After ten seconds we modulate the frequency a single time. We should be able to
	// watch it move on an FFT.
	double center_freq = 120.0;		// Center frequency in MHz
	carrier_setup(center_freq);
	
	/////////////////////FM TEST///////////////////////
	//Loop through 1 MHz band
	/*double offset = -0.4;			// Offset frequency to modulate to in MHz
	for(int i=0; i<50; i++){
		cout << "Transmitting tone at " << center_freq+offset << "MHz" << endl << "Offset = " << offset << endl;
		usleep(1000000);
		fm_modulate(center_freq + offset);
		offset = offset + 0.1;
		if(offset == 0.5){
			offset = -0.4;
		}
	}*/
	
	int length = 8;
	int data[8] = {1,0,1,0,1,1,0,0}; // We'll want to look into being able to initialize variable length vectors
	double freq_space = 0.1;
	double rate = 1000; //symbol rate

	////////////////////FSK/OOK TEST///////////////////////
	for(int i=0; i<60*(rate/8); i++){
		//fsk_modulate(data, length, center_freq, freq_space, rate);
		OOK_modulate(data, length, rate);
	}

	// disable GPIO CLK
	struct GPCTL disableword = {6/*SRC*/, 0, 0, 0, 0, 1,0x5a};
    	//ACCESS(CM_GP0CTL) = *((int*)&disableword);

	return 0;
}



