#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import custom_swig as custom

class qa_RPi_Rx (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        src_data = [0]*3 + [1]*14 + [0]*17 + [0.33]*13 + [0.67]*16 + [1]*15 + [0.33]*13 + [0]*30
        expected_result = src_data #(1.0,2.0,3.0,1.0)
        src = blocks.vector_source_f(src_data)
        fsk = custom.RPi_Rx(2,15,4)
        dst = blocks.vector_sink_f()
        self.tb.connect(src, fsk)
        self.tb.connect(fsk, dst)
        self.tb.run()
        result_data = dst.data()
        self.assertFloatTuplesAlmostEqual(expected_result, result_data, 6)


if __name__ == '__main__':
    gr_unittest.run(qa_RPi_Rx, "qa_RPi_Rx.xml")
