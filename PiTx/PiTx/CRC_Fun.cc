/*

This file contains the functions needed to include a CRC in the header.

The function CRC is what adds the CRC to the header on the Tx side. It
takes a binary vector of length 24 and an integer that tells it the
length of the CRC. It then calculates the CRC and fills it in to the end
of the header.

The function CRC_Check is what will validate the CRC on the Rx side. It
takes the same arguments and calculates the CRC for the header in the same
way as the CRC function. It then compares this calculated value to the
actual value it received. It returns true if the CRC is valid and false
if the CRC is invalid.

*/

#include <iostream>

void CRC(int header[24], int crc_len){
	int num=0;
	int poly=0;
	
	// Create a polynomial of length crc_len with alternating 1's and 0's
	for(int i=0; i<crc_len; i++){
		if(i%2==0){poly += 1<<(crc_len-i-1);}
	}
	
	// Initialize the numerator of the polynomial division
	for(int i=0; i<crc_len; i++){
		num += header[i]<<i;
	}
	
	// Perform polynomial division
	for(int i=0; i<24-(2*crc_len); i++){
		if (num < poly){
			num = (num<<1) + header[i+crc_len];
		}
		else{
			num = (num-poly);
			if(i!= 24-crc_len){num=num<<1;}
		}
	}
	
	// Set the final designated CRC bits of the header
	for(int i=0; i<crc_len; i++){
		header[i+24-crc_len] = (num>>(crc_len-i-1)) & 1;
	}
}
int CRC_Check(int header[24], int crc_len){
	int num=0;
	int poly = 5; // 0b101
	
	// Initialize the numerator of the polynomial division
	for(int i=0; i<crc_len; i++){
		num += header[i]<<i;
	}
	
	// Perform polynomial division
	for(int i=0; i<24-(2*crc_len); i++){
		if (num < poly){
			num = (num<<1) + header[i+crc_len];
		}
		else{
			num = (num-poly);
			if(i!= 24-crc_len){num=num<<1;}
		}
	}
	
	// Compare calculated to received CRC
	int crc_rx=0;
	for(int i=0; i<crc_len; i++){
		crc_rx += header[24-crc_len+i]<<(crc_len-i-1);
	}
	return num==crc_rx;
}

int main(){
	// Test header
	int header[24] = {1,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,0,0,0};
	for(int i=0; i<24; i++){std::cout << header[i];}
	std::cout << std::endl;
	
	// Modify header to include CRC
	CRC(header, 3);
	for(int i=0; i<24; i++){std::cout << header[i];}
	
	// Check the CRC
	std::cout << std::endl << "CRC Check: " << CRC_Check(header, 3) << std::endl;
	return 0;
}
