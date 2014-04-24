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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "UART_impl.h"
#include <volk/volk.h>
#include <stdexcept>
#include <iostream>
#include <cmath>

namespace gr {
  namespace custom {

    UART::sptr
    UART::make(int samp_per_sym, int word_length)
    {
      return gnuradio::get_initial_sptr
        (new UART_impl(samp_per_sym, word_length));
    }

    /*
     * The private constructor
     */
    UART_impl::UART_impl(int samp_per_sym, int word_length)
      : gr::block("UART",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float))),
	d_samp_per_sym(samp_per_sym),
	d_samp_count(0),
	d_sym_count(0),
	d_start_bit(0),
	d_rising_edge(0),
	d_start_count(0),
	d_packet_count(0),
	d_word_length(word_length),
	file_name((char*)"/home/user/Desktop/Rx_Test.txt")
    {
		if(samp_per_sym%2 == 1){
			set_samp_point((samp_per_sym-1)/2);
		}
		else{
			set_samp_point(samp_per_sym/2-1);
		}
	}

    /*
     * Our virtual destructor.
     */
    UART_impl::~UART_impl()
    {}

    void
    UART_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = samp_per_sym();
    }		
	
    int
    UART_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];

        // Expect there to be no output unless determined below
		int num_output_items = 0;
		//int levels = pow(2,bits_per_sym());
		
		// Look for a rising edge and verify the start bit
		if (start_bit()==0){
			if (rising_edge()==1){
				set_samp_count(samp_count()+1);
				set_start_count(start_count()+in[0]);
			}
			/*if (rising_edge()==1 && samp_count()==samp_point() && in[0]==0){
				//std::cout << "False start bit." << std::endl;
				//set_rising_edge(0);
				//set_samp_count(0);				
			}*/
			if (rising_edge() == 0 && in[0] == 1){ 
				set_rising_edge(1);
				std::cout << std::endl << std::endl << "Rising edge detected." << std::endl;
			}
			if (rising_edge()==1 && samp_count()==samp_per_sym()-1){
				if(start_count() > samp_point()+1){
					std::cout << "Start bit successfully detected. Packet number: " << packet_count() << std::endl;
					set_start_bit(1);
					set_samp_count(0);
					set_rising_edge(0);
					set_start_count(0);
					set_packet_count(packet_count()+1);
				}
				else{
					std::cout << "False start bit." << std::endl;
					set_rising_edge(0);
					set_samp_count(0);
				}
			}
		}
		
		// Once a start has been detected, sample the given number of symbols at the center
		if (start_bit() == 1 && sym_count() >= 0){

			// Determine the symbols value by looking at the expected center sample
			if (samp_count() == samp_point()){								

				std::cout << in[0];
				num_output_items = 1;				
				out[0] = in[0];
				if (sym_count()>0){				
					set_character(in[0], sym_count()%8-1);
				}
				if (sym_count()%8==0){
					int C[8];
					character(C);
					char_write(C);					
				}			
			}

			// Always increment the sample counter during a word decoding
			set_samp_count(samp_count()+1);

			// Reset the sample counter and iterate the symbol counter each time the sample counter
			// reaches the number of samples per symbol
			if (samp_count() == samp_per_sym()){
				set_samp_count(0);
				set_sym_count(sym_count()+1);
			}
			
			// Reset UART once entire word has been read and the stop bit has been received					
			if (sym_count() >= word_length() && in[0] == 0){
				set_rising_edge(0);
				set_start_bit(0);
				set_samp_count(0);
				set_sym_count(0);
			}
			//std::cout << "Symbol Count: " << sym_count()
			//	<< "  Sample Count: " << samp_count() << std::endl;					 
		}

        consume_each (1);
        return num_output_items;
    }

  } /* namespace custom */
} /* namespace gr */

