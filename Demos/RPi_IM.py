#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Rpi Im
# Generated: Fri May  9 13:53:56 2014
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from grc_gnuradio import blks2 as grc_blks2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import RPi_Rx
import osmosdr
import wx

class RPi_IM(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Rpi Im")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.symbol_rate = symbol_rate = 80000
        self.vec_len = vec_len = 256
        self.symbol_period = symbol_period = pow(symbol_rate,-1)
        self.samp_rate = samp_rate = 2.4e6
        self.samp_per_sym = samp_per_sym = 15
        self.decim = decim = 2
        self.bits_per_sym = bits_per_sym = 1

        ##################################################
        # Blocks
        ##################################################
        self.osmosdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + "rtl=0" )
        self.osmosdr_source_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0.set_center_freq(175e6, 0)
        self.osmosdr_source_0.set_freq_corr(0, 0)
        self.osmosdr_source_0.set_dc_offset_mode(0, 0)
        self.osmosdr_source_0.set_iq_balance_mode(0, 0)
        self.osmosdr_source_0.set_gain_mode(0, 0)
        self.osmosdr_source_0.set_gain(15, 0)
        self.osmosdr_source_0.set_if_gain(0, 0)
        self.osmosdr_source_0.set_bb_gain(0, 0)
        self.osmosdr_source_0.set_antenna("", 0)
        self.osmosdr_source_0.set_bandwidth(0, 0)
          
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(8)
        self.blocks_float_to_char_0 = blocks.float_to_char(1, 1)
        self.blks2_tcp_sink_0_0 = grc_blks2.tcp_sink(
        	itemsize=gr.sizeof_char*1,
        	addr="127.0.0.1",
        	port=3491,
        	server=True,
        )
        self.analog_pwr_squelch_xx_0 = analog.pwr_squelch_cc(-20, 0.01, 10, True)
        self.analog_fm_demod_cf_0 = analog.fm_demod_cf(
        	channel_rate=samp_rate,
        	audio_decim=decim,
        	deviation=75e3,
        	audio_pass=400e3,
        	audio_stop=500e3,
        	gain=1.0,
        	tau=75e-6,
        )
        self.RPi_Rx_RPi_Rx_0 = RPi_Rx.RPi_Rx(bits_per_sym, samp_per_sym, vec_len, 65)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.osmosdr_source_0, 0), (self.analog_pwr_squelch_xx_0, 0))
        self.connect((self.analog_pwr_squelch_xx_0, 0), (self.analog_fm_demod_cf_0, 0))
        self.connect((self.analog_fm_demod_cf_0, 0), (self.RPi_Rx_RPi_Rx_0, 0))
        self.connect((self.RPi_Rx_RPi_Rx_0, 0), (self.blocks_float_to_char_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.blks2_tcp_sink_0_0, 0))
        self.connect((self.blocks_float_to_char_0, 0), (self.blocks_pack_k_bits_bb_0, 0))


# QT sink close method reimplementation

    def get_symbol_rate(self):
        return self.symbol_rate

    def set_symbol_rate(self, symbol_rate):
        self.symbol_rate = symbol_rate
        self.set_symbol_period(pow(self.symbol_rate,-1))

    def get_vec_len(self):
        return self.vec_len

    def set_vec_len(self, vec_len):
        self.vec_len = vec_len

    def get_symbol_period(self):
        return self.symbol_period

    def set_symbol_period(self, symbol_period):
        self.symbol_period = symbol_period

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.osmosdr_source_0.set_sample_rate(self.samp_rate)

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym

    def get_decim(self):
        return self.decim

    def set_decim(self, decim):
        self.decim = decim

    def get_bits_per_sym(self):
        return self.bits_per_sym

    def set_bits_per_sym(self, bits_per_sym):
        self.bits_per_sym = bits_per_sym

if __name__ == '__main__':
    import ctypes
    import os
    if os.name == 'posix':
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = RPi_IM()
    tb.Start(True)
    tb.Wait()

