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


#ifndef INCLUDED_RPI_RX_RPI_RX_H
#define INCLUDED_RPI_RX_RPI_RX_H

#include <RPi_Rx/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace RPi_Rx {

    /*!
     * \brief <+description of block+>
     * \ingroup RPi_Rx
     *
     */
    class RPI_RX_API RPi_Rx : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<RPi_Rx> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of RPi_Rx::RPi_Rx.
       *
       * To avoid accidental use of raw pointers, RPi_Rx::RPi_Rx's
       * constructor is in a private implementation
       * class. RPi_Rx::RPi_Rx::make is the public interface for
       * creating new instances.
       */
      static sptr make(int bits_per_sym, int samp_per_sym, int packet_length, int address);
    };

  } // namespace RPi_Rx
} // namespace gr

#endif /* INCLUDED_RPI_RX_RPI_RX_H */

