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
	d_word_length(word_length),
	d_iter(0)
    {
	set_samp_count(0);
	set_sym_count(0);
	set_start_bit(0);
	set_iter(0);
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

        // Do <+signal processing+>
		// Do <+signal processing+>
		int num_output_items = 0;
		//std::cout << "INPUT:: " << iter() << std::endl;
		//std::cout << start_bit() << std::endl;
		if (start_bit() == 0 & in[0] == 1){ 
			set_start_bit(1);
			std::cout << std::endl << "Rising edge detected." << std::endl << std::endl;
		}
		if (start_bit() == 1){
			//std::cout << "Start Bit Set" << std::endl;
			if (samp_count() == (samp_per_sym()-1)/2-1){				
				std::cout << "Symbol Count: " << sym_count()
					<< "  Sample Count: " << samp_count()					 
					<< "  Bit: " << in[0] << std::endl;
				out[0] = in[0];
				num_output_items = 1;
				set_sym_count(sym_count()+1);
			}
			if (sym_count() == word_length()){
				set_start_bit(0);
				set_samp_count(0);
				set_sym_count(0);
			}
			set_samp_count(samp_count()+1);
			if (samp_count() == samp_per_sym()){
				set_samp_count(0);
			}
		}
		set_iter(iter()+1);
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (1);
		//std::cout << std::endl;
        // Tell runtime system how many output items we produced.
        return num_output_items;
    }

  } /* namespace custom */
} /* namespace gr */

