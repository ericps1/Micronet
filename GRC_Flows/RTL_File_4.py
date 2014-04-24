#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Rtl File 4
# Generated: Tue Mar  4 22:07:13 2014
##################################################

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import osmosdr

class RTL_File_4(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Rtl File 4")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 256e3*8

        ##################################################
        # Blocks
        ##################################################
        self.osmosdr_source_0_1_0 = osmosdr.source( args="nchan=" + str(1) + " " + 'rtl_tcp=10.0.0.12:1234' )
        self.osmosdr_source_0_1_0.set_sample_rate(1.792e6)
        self.osmosdr_source_0_1_0.set_center_freq(105.3e6, 0)
        self.osmosdr_source_0_1_0.set_freq_corr(0, 0)
        self.osmosdr_source_0_1_0.set_dc_offset_mode(2, 0)
        self.osmosdr_source_0_1_0.set_iq_balance_mode(2, 0)
        self.osmosdr_source_0_1_0.set_gain_mode(1, 0)
        self.osmosdr_source_0_1_0.set_gain(40, 0)
        self.osmosdr_source_0_1_0.set_if_gain(30, 0)
        self.osmosdr_source_0_1_0.set_bb_gain(20, 0)
        self.osmosdr_source_0_1_0.set_antenna("", 0)
        self.osmosdr_source_0_1_0.set_bandwidth(0, 0)
          
        self.osmosdr_source_0_1 = osmosdr.source( args="nchan=" + str(1) + " " + 'rtl=2' )
        self.osmosdr_source_0_1.set_sample_rate(samp_rate)
        self.osmosdr_source_0_1.set_center_freq(92.3e6, 0)
        self.osmosdr_source_0_1.set_freq_corr(0, 0)
        self.osmosdr_source_0_1.set_dc_offset_mode(2, 0)
        self.osmosdr_source_0_1.set_iq_balance_mode(2, 0)
        self.osmosdr_source_0_1.set_gain_mode(1, 0)
        self.osmosdr_source_0_1.set_gain(40, 0)
        self.osmosdr_source_0_1.set_if_gain(30, 0)
        self.osmosdr_source_0_1.set_bb_gain(20, 0)
        self.osmosdr_source_0_1.set_antenna("", 0)
        self.osmosdr_source_0_1.set_bandwidth(0, 0)
          
        self.osmosdr_source_0_0 = osmosdr.source( args="nchan=" + str(1) + " " + 'rtl=1' )
        self.osmosdr_source_0_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0_0.set_center_freq(90.7e6, 0)
        self.osmosdr_source_0_0.set_freq_corr(0, 0)
        self.osmosdr_source_0_0.set_dc_offset_mode(2, 0)
        self.osmosdr_source_0_0.set_iq_balance_mode(2, 0)
        self.osmosdr_source_0_0.set_gain_mode(1, 0)
        self.osmosdr_source_0_0.set_gain(40, 0)
        self.osmosdr_source_0_0.set_if_gain(30, 0)
        self.osmosdr_source_0_0.set_bb_gain(20, 0)
        self.osmosdr_source_0_0.set_antenna("", 0)
        self.osmosdr_source_0_0.set_bandwidth(0, 0)
          
        self.osmosdr_source_0 = osmosdr.source( args="nchan=" + str(1) + " " + 'rtl=0' )
        self.osmosdr_source_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0.set_center_freq(105.3e6, 0)
        self.osmosdr_source_0.set_freq_corr(0, 0)
        self.osmosdr_source_0.set_dc_offset_mode(2, 0)
        self.osmosdr_source_0.set_iq_balance_mode(2, 0)
        self.osmosdr_source_0.set_gain_mode(1, 0)
        self.osmosdr_source_0.set_gain(40, 0)
        self.osmosdr_source_0.set_if_gain(30, 0)
        self.osmosdr_source_0.set_bb_gain(20, 0)
        self.osmosdr_source_0.set_antenna("", 0)
        self.osmosdr_source_0.set_bandwidth(0, 0)
          
        self.blocks_file_sink_0_1_0 = blocks.file_sink(gr.sizeof_gr_complex*1, "/home/micronet/Desktop/RTL_3_Capture.bin")
        self.blocks_file_sink_0_1_0.set_unbuffered(False)
        self.blocks_file_sink_0_1 = blocks.file_sink(gr.sizeof_gr_complex*1, "/home/micronet/Desktop/RTL_2_Capture.bin")
        self.blocks_file_sink_0_1.set_unbuffered(False)
        self.blocks_file_sink_0_0 = blocks.file_sink(gr.sizeof_gr_complex*1, "/home/micronet/Desktop/RTL_1_Capture.bin")
        self.blocks_file_sink_0_0.set_unbuffered(False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_gr_complex*1, "/home/micronet/Desktop/RTL_0_Capture.bin")
        self.blocks_file_sink_0.set_unbuffered(False)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.osmosdr_source_0, 0), (self.blocks_file_sink_0, 0))
        self.connect((self.osmosdr_source_0_0, 0), (self.blocks_file_sink_0_0, 0))
        self.connect((self.osmosdr_source_0_1_0, 0), (self.blocks_file_sink_0_1_0, 0))
        self.connect((self.osmosdr_source_0_1, 0), (self.blocks_file_sink_0_1, 0))


# QT sink close method reimplementation

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.osmosdr_source_0.set_sample_rate(self.samp_rate)
        self.osmosdr_source_0_0.set_sample_rate(self.samp_rate)
        self.osmosdr_source_0_1.set_sample_rate(self.samp_rate)

if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = RTL_File_4()
    tb.start()
    raw_input('Press Enter to quit: ')
    tb.stop()
    tb.wait()

