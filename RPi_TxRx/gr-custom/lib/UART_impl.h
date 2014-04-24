/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_CUSTOM_UART_IMPL_H
#define INCLUDED_CUSTOM_UART_IMPL_H

#include <custom/UART.h>
#include <vector>
#include <stdio.h>
using namespace std;

namespace gr {
  namespace custom {

    class UART_impl : public UART
    {
     private:
      int d_samp_per_sym;
	  int d_samp_count;
	  int d_sym_count;
	  int d_start_bit;
	  int d_rising_edge;
	  int d_word_length;
	  int d_samp_point;
	  int d_start_count;
	  int d_packet_count;
	  int d_character[8];
	  char *file_name;
	  

     public:
      UART_impl(int samp_per_sym, int word_length);
      ~UART_impl();

	  int samp_per_sym() const { return d_samp_per_sym; }
	  void set_samp_per_sym(unsigned samp_per_sym) { d_samp_per_sym = samp_per_sym; }
	  int samp_count() const { return d_samp_count; }
      void set_samp_count(unsigned count) { d_samp_count = count; }
	  int sym_count() const { return d_sym_count; }
      void set_sym_count(int count) { d_sym_count = count; }
	  int start_bit() const { return d_start_bit; }
      void set_start_bit(unsigned flag) { d_start_bit = flag; }
      int rising_edge() const { return d_rising_edge; }
      void set_rising_edge(unsigned re) { d_rising_edge = re; }
	  int word_length() const { return d_word_length; }
      void set_word_length(unsigned len) { d_word_length = len; }
      int samp_point() const {return d_samp_point;}
      void set_samp_point(unsigned sp) { d_samp_point = sp; }
      int start_count() const {return d_start_count;}
      void set_start_count(unsigned sc) { d_start_count = sc; }
      int packet_count() const {return d_packet_count;}
      void set_packet_count(unsigned pc) { d_packet_count = pc; }
      
      void character(int C[8]) { 
	  		for (int i=0; i<8; i++){
      			C[i] = d_character[i];
      		}
	  }
	  void set_character(int sym, int ind){
	  		d_character[ind] = sym;
	  }
	  
	  void char_write(int C[8])
		{
			unsigned char int_char = 0;
	
			int bit_pos;
			for(bit_pos = 7; bit_pos >= 0; bit_pos--){
				int_char |= (*C++ == 1 ? 1 << bit_pos : 0);
			}
			FILE* output_file;
			output_file = fopen(file_name,"a+");
			fprintf(output_file,"%c", *C);
			fclose(output_file);
		}

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace custom
} // namespace gr

#endif /* INCLUDED_CUSTOM_UART_IMPL_H */
