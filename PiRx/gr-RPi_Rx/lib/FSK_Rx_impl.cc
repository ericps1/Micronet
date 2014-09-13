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
#include "FSK_Rx_impl.h"

namespace gr {
  namespace RPi_Rx {

    FSK_Rx::sptr
    FSK_Rx::make(int bits_per_sym, int packet_length, int samp_per_sym)
    {
      return gnuradio::get_initial_sptr
        (new FSK_Rx_impl(bits_per_sym, packet_length, samp_per_sym));
    }

    /*
     * The private constructor
     */
    FSK_Rx_impl::FSK_Rx_impl(int bits_per_sym, int packet_length, int samp_per_sym)
      : gr::block("FSK_Rx",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float))),
    d_samp_per_sym(samp_per_sym),
	d_samp_count(0),
	d_sym_count(0),
	d_start_bit(0),
	d_rising_edge(0),
	d_start_1_count(0.0),
	d_start_2_count(0.0),
	d_packet_count(0),
	d_word_length(packet_length),
	d_bits_per_sym(bits_per_sym)
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
    FSK_Rx_impl::~FSK_Rx_impl()
    {
    }

    void
    FSK_Rx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }

    int
    FSK_Rx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];

        // Expect there to be no output unless determined below
		int num_output_items = 0;
		int levels = pow(2,bits_per_sym());

		// Look for a rising edge and verify the start bit
		if (start_bits()==0){
			if (rising_edge() == 0 && in[0] > 0.0){ 
				set_rising_edge(1);
				std::cout << std::endl << std::endl << "Rising edge detected." << std::endl;
			}
			if (rising_edge()==1){
				set_samp_count(samp_count()+1);
				if (samp_count()<=samp_per_sym()){
					std::cout << in[0] << " ";			
					set_start_1_count(start_1_count()+in[0]);
				}
				if (samp_count()>samp_per_sym()){
					std::cout << in[0] << " ";
					set_start_2_count(start_2_count()+in[0]);
				}
				if (samp_count()==2*samp_per_sym()){
					if(start_1_count()>0.0 && start_2_count()<0.0){
						std::cout << "Start bits successfully detected. Packet number: " << packet_count() << std::endl;
						float max = (float)start_1_count()/(float)samp_per_sym();
						float min = (float)start_2_count()/(float)samp_per_sym();
						std::cout << "Max = " << max << ", " << "Min = " << min << std::endl << "Symbol boundaries = ";
							
						float bounds[7];
						for (int i=0; i<levels-1; i++){
							std::cout << min + (max-min)*(float)(2*i+1)/(2.0f*((float)levels-1.0f));
							if (i<levels-2) std::cout << ", ";
							bounds[i] = min + (max-min)*(float)(2*i+1)/(2.0f*((float)levels-1.0f));
						}						
						for (int i=levels-1; i<7; i++){
							bounds[i] = 0.0;
						}
						std::cout << std::endl;
						
						set_boundaries(bounds);
						set_start_1_count(0.0);
						set_start_2_count(0.0);
						set_start_bits(1);
						set_samp_count(0);
						set_rising_edge(0);
						set_packet_count(packet_count()+1);						
												
					}
					else{
						std::cout << std::endl << "False start bit." << std::endl;
						std::cout << "Count 1: " << start_1_count() << " Count 2: " << start_2_count() << std::endl;
						set_rising_edge(0);
						set_samp_count(0);
						set_start_1_count(0.0);
						set_start_2_count(0.0);
					}
				}
			}
		}
		// Once the start bits have been detected, sample the given number of symbols at the center
		if (start_bits() == 1){

			// Determine the symbols value by looking at the expected center sample
			if (samp_count() == (samp_per_sym()-1)/2){								
				float bounds[7];
				boundaries(bounds);
				float sym;
				num_output_items = 1;
				
				// Determine the range the symbol lies in				
				for(int i=0; i<pow(2,bits_per_sym())-1; i++){
					if(i==0 && in[0] < bounds[0]){sym=0.0f;}
					else{
						if(i==(int)(pow(2,bits_per_sym())-2) && in[0] > bounds[i]){sym=(float)i+1;}
						else{
							if(in[0] > bounds[i-1] && in[0] < bounds[i]){sym=(float)i;}
						}
					}
				}
				//std::cout << "Symbol = " << sym << " = ";
				for(int i=bits_per_sym(); i>0; i--){
					std::cout << (bool)(((int)sym) & ((int)(pow(2,i-1)))) << " ";
				}
				//std::cout << std::endl;
				out[0] = sym;
			}

			// Always increment the sample counter during a word decoding
			set_samp_count(samp_count()+1);
			
			// Reset UART once entire word has been read and the stop bit has been received					
			if (sym_count() == word_length() && in[0]<0.0){
				set_rising_edge(0);
				set_start_bits(0);
				set_samp_count(0);
				set_sym_count(0);
			}

			// Reset the sample counter and iterate the symbol counter each time the sample counter
			// reaches the number of samples per symbol
			if (samp_count() == samp_per_sym()){
				set_samp_count(0);
				set_sym_count(sym_count()+1);
			}
		}

        consume_each (1);
        return num_output_items;
    }

  } /* namespace RPi_Rx */
} /* namespace gr */

