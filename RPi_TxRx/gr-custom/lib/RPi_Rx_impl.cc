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
#include "RPi_Rx_impl.h"

namespace gr {
  namespace custom {

    RPi_Rx::sptr
    RPi_Rx::make(int bits_per_sym, int samp_per_sym, int word_length)
    {
      return gnuradio::get_initial_sptr
        (new RPi_Rx_impl(bits_per_sym, samp_per_sym, word_length));
    }

    /*
     * The private constructor
     */
    RPi_Rx_impl::RPi_Rx_impl(int bits_per_sym, int samp_per_sym, int word_length)
      : gr::block("RPi_Rx",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(float)))
    {}

    /*
     * Our virtual destructor.
     */
    RPi_Rx_impl::~RPi_Rx_impl()
    {
    }

    void
    RPi_Rx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;//samp_per_sym();
    }

    int
    RPi_Rx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];
        
		std::cout << in[0] << std::endl;
		
        out[0] = in[0];
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace custom */
} /* namespace gr */

