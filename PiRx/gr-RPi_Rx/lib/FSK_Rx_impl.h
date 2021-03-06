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

#ifndef INCLUDED_RPI_RX_FSK_RX_IMPL_H
#define INCLUDED_RPI_RX_FSK_RX_IMPL_H

#include <RPi_Rx/FSK_Rx.h>

namespace gr {
  namespace RPi_Rx {

    class FSK_Rx_impl : public FSK_Rx
    {
     private:
      int d_samp_per_sym;
	  int d_samp_count;
	  int d_sym_count;
	  int d_start_bit;
	  int d_rising_edge;
	  int d_word_length;
	  int d_samp_point;
	  float d_start_1_count;
	  float d_start_2_count;
	  int d_packet_count;
	  int d_max;
	  int d_min;
	  int d_bits_per_sym;
	  float d_boundaries[7];

     public:
      FSK_Rx_impl(int bits_per_sym, int packet_length, int samp_per_sym);
      ~FSK_Rx_impl();
	
	  int samp_per_sym() const { return d_samp_per_sym; }
	  void set_samp_per_sym(unsigned samp_per_sym) { d_samp_per_sym = samp_per_sym; }
	  int samp_count() const { return d_samp_count; }
      void set_samp_count(unsigned count) { d_samp_count = count; }
	  int sym_count() const { return d_sym_count; }
      void set_sym_count(int count) { d_sym_count = count; }
	  int start_bits() const { return d_start_bit; }
      void set_start_bits(unsigned flag) { d_start_bit = flag; }
      int rising_edge() const { return d_rising_edge; }
      void set_rising_edge(unsigned re) { d_rising_edge = re; }
	  int word_length() const { return d_word_length; }
      void set_word_length(unsigned len) { d_word_length = len; }
      int samp_point() const {return d_samp_point;}
      void set_samp_point(unsigned sp) { d_samp_point = sp; }
      float start_1_count() const {return d_start_1_count;}
      void set_start_1_count(float sc) { d_start_1_count = sc; }
      float start_2_count() const {return d_start_2_count;}
      void set_start_2_count(float sc) { d_start_2_count = sc; }
      int packet_count() const {return d_packet_count;}
      void set_packet_count(unsigned pc) { d_packet_count = pc; }
	  int max() const { return d_max; }
      void set_max(unsigned max) { d_max = max; }
	  int min() const { return d_min; }
      void set_min(unsigned min) { d_min = min; }
	  void boundaries(float A[7]) { 
	  		for (int i=0; i<7; i++){
      			A[i] = d_boundaries[i];
      		}
	  }
      void set_boundaries(float b[7]) { 
      		for (int i=0; i<7; i++){
      			d_boundaries[i] = b[i];
      		}      		
      }
	  int bits_per_sym() const { return d_bits_per_sym; }
      void set_bits_per_sym(int b) { 
      			d_bits_per_sym = b;
      }
      
      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace RPi_Rx
} // namespace gr

#endif /* INCLUDED_RPI_RX_FSK_RX_IMPL_H */

