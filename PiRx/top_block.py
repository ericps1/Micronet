#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Tue Jun 17 16:08:26 2014
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.wxgui import forms
from grc_gnuradio import blks2 as grc_blks2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import RPi_Rx
import osmosdr
import wx

class top_block(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Top Block")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.symbol_rate = symbol_rate = 60000
        self.symbol_period = symbol_period = pow(symbol_rate,-1)
        self.samp_rate = samp_rate = 300e3
        self.samp_per_sym = samp_per_sym = 5
        self.packet_len = packet_len = 256
        self.decimation = decimation = 1
        self.bits_per_sym = bits_per_sym = 1
        self.Sq_Thresh = Sq_Thresh = -30
        self.RF_Gain = RF_Gain = 20
        self.Fc = Fc = 175e6

        ##################################################
        # Blocks
        ##################################################
        _Sq_Thresh_sizer = wx.BoxSizer(wx.VERTICAL)
        self._Sq_Thresh_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_Sq_Thresh_sizer,
        	value=self.Sq_Thresh,
        	callback=self.set_Sq_Thresh,
        	label="Squelch Threshold",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._Sq_Thresh_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_Sq_Thresh_sizer,
        	value=self.Sq_Thresh,
        	callback=self.set_Sq_Thresh,
        	minimum=-50,
        	maximum=20,
        	num_steps=1000,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_Sq_Thresh_sizer, 0, 2, 1, 1)
        _RF_Gain_sizer = wx.BoxSizer(wx.VERTICAL)
        self._RF_Gain_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_RF_Gain_sizer,
        	value=self.RF_Gain,
        	callback=self.set_RF_Gain,
        	label="RF Gain",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._RF_Gain_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_RF_Gain_sizer,
        	value=self.RF_Gain,
        	callback=self.set_RF_Gain,
        	minimum=0,
        	maximum=50,
        	num_steps=1000,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_RF_Gain_sizer, 0, 0, 1, 1)
        _Fc_sizer = wx.BoxSizer(wx.VERTICAL)
        self._Fc_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_Fc_sizer,
        	value=self.Fc,
        	callback=self.set_Fc,
        	label="Center Frequency",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._Fc_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_Fc_sizer,
        	value=self.Fc,
        	callback=self.set_Fc,
        	minimum=50e6,
        	maximum=1700e6,
        	num_steps=1000,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.GridAdd(_Fc_sizer, 0, 1, 1, 1)
        self.osmosdr_source_0_0 = osmosdr.source( args="numchan=" + str(1) + " " + 'rtl_tcp=10.0.0.13:1234' )
        self.osmosdr_source_0_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0_0.set_center_freq(Fc+10e6, 0)
        self.osmosdr_source_0_0.set_freq_corr(0, 0)
        self.osmosdr_source_0_0.set_dc_offset_mode(0, 0)
        self.osmosdr_source_0_0.set_iq_balance_mode(0, 0)
        self.osmosdr_source_0_0.set_gain_mode(False, 0)
        self.osmosdr_source_0_0.set_gain(RF_Gain, 0)
        self.osmosdr_source_0_0.set_if_gain(0, 0)
        self.osmosdr_source_0_0.set_bb_gain(0, 0)
        self.osmosdr_source_0_0.set_antenna("", 0)
        self.osmosdr_source_0_0.set_bandwidth(0, 0)
          
        self.osmosdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + 'rtl_tcp=10.0.0.12:1234' )
        self.osmosdr_source_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0.set_center_freq(Fc, 0)
        self.osmosdr_source_0.set_freq_corr(0, 0)
        self.osmosdr_source_0.set_dc_offset_mode(0, 0)
        self.osmosdr_source_0.set_iq_balance_mode(0, 0)
        self.osmosdr_source_0.set_gain_mode(False, 0)
        self.osmosdr_source_0.set_gain(0, 0)
        self.osmosdr_source_0.set_if_gain(0, 0)
        self.osmosdr_source_0.set_bb_gain(0, 0)
        self.osmosdr_source_0.set_antenna("", 0)
        self.osmosdr_source_0.set_bandwidth(0, 0)
          
        self.blocks_skiphead_0_0 = blocks.skiphead(gr.sizeof_gr_complex*1, int(samp_rate)*5)
        self.blocks_skiphead_0 = blocks.skiphead(gr.sizeof_gr_complex*1, int(samp_rate)*5)
        self.blocks_pack_k_bits_bb_0_0 = blocks.pack_k_bits_bb(8)
        self.blocks_pack_k_bits_bb_0 = blocks.pack_k_bits_bb(8)
        self.blocks_null_sink_0_0 = blocks.null_sink(gr.sizeof_float*1)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_float*1)
        self.blocks_float_to_char_0_0 = blocks.float_to_char(1, 1)
        self.blocks_float_to_char_0 = blocks.float_to_char(1, 1)
        self.blks2_tcp_sink_0_0_0 = grc_blks2.tcp_sink(
        	itemsize=gr.sizeof_char*1,
        	addr="127.0.0.1",
        	port=3493,
        	server=True,
        )
        self.blks2_tcp_sink_0_0 = grc_blks2.tcp_sink(
        	itemsize=gr.sizeof_char*1,
        	addr="127.0.0.1",
        	port=3491,
        	server=True,
        )
        self.analog_pwr_squelch_xx_0_0 = analog.pwr_squelch_cc(Sq_Thresh, 0.01, 5, True)
        self.analog_pwr_squelch_xx_0 = analog.pwr_squelch_cc(-10, 0.01, 5, True)
        self.analog_fm_demod_cf_0_0 = analog.fm_demod_cf(
        	channel_rate=samp_rate,
        	audio_decim=decimation,
        	deviation=75e3,
        	audio_pass=120e3,
        	audio_stop=140e3,
        	gain=1.0,
        	tau=75e-6,
        )
        self.analog_fm_demod_cf_0 = analog.fm_demod_cf(
        	channel_rate=samp_rate,
        	audio_decim=decimation,
        	deviation=75e3,
        	audio_pass=120e3,
        	audio_stop=140e3,
        	gain=1.0,
        	tau=75e-6,
        )
        self.RPi_Rx_RPi_Rx_0_0 = RPi_Rx.RPi_Rx(bits_per_sym, samp_per_sym, packet_len, 65)
        self.RPi_Rx_RPi_Rx_0 = RPi_Rx.RPi_Rx(bits_per_sym, samp_per_sym, packet_len, 65)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_fm_demod_cf_0, 0), (self.RPi_Rx_RPi_Rx_0, 0))
        self.connect((self.RPi_Rx_RPi_Rx_0, 0), (self.blocks_null_sink_0, 0))
        self.connect((self.RPi_Rx_RPi_Rx_0, 0), (self.blocks_float_to_char_0, 0))
        self.connect((self.analog_pwr_squelch_xx_0, 0), (self.analog_fm_demod_cf_0, 0))
        self.connect((self.blocks_skiphead_0, 0), (self.analog_pwr_squelch_xx_0, 0))
        self.connect((self.osmosdr_source_0, 0), (self.blocks_skiphead_0, 0))
        self.connect((self.blocks_float_to_char_0, 0), (self.blocks_pack_k_bits_bb_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0, 0), (self.blks2_tcp_sink_0_0, 0))
        self.connect((self.blocks_float_to_char_0_0, 0), (self.blocks_pack_k_bits_bb_0_0, 0))
        self.connect((self.blocks_pack_k_bits_bb_0_0, 0), (self.blks2_tcp_sink_0_0_0, 0))
        self.connect((self.analog_fm_demod_cf_0_0, 0), (self.RPi_Rx_RPi_Rx_0_0, 0))
        self.connect((self.RPi_Rx_RPi_Rx_0_0, 0), (self.blocks_null_sink_0_0, 0))
        self.connect((self.RPi_Rx_RPi_Rx_0_0, 0), (self.blocks_float_to_char_0_0, 0))
        self.connect((self.analog_pwr_squelch_xx_0_0, 0), (self.analog_fm_demod_cf_0_0, 0))
        self.connect((self.blocks_skiphead_0_0, 0), (self.analog_pwr_squelch_xx_0_0, 0))
        self.connect((self.osmosdr_source_0_0, 0), (self.blocks_skiphead_0_0, 0))


# QT sink close method reimplementation

    def get_symbol_rate(self):
        return self.symbol_rate

    def set_symbol_rate(self, symbol_rate):
        self.symbol_rate = symbol_rate
        self.set_symbol_period(pow(self.symbol_rate,-1))

    def get_symbol_period(self):
        return self.symbol_period

    def set_symbol_period(self, symbol_period):
        self.symbol_period = symbol_period

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.osmosdr_source_0.set_sample_rate(self.samp_rate)
        self.osmosdr_source_0_0.set_sample_rate(self.samp_rate)

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym

    def get_packet_len(self):
        return self.packet_len

    def set_packet_len(self, packet_len):
        self.packet_len = packet_len

    def get_decimation(self):
        return self.decimation

    def set_decimation(self, decimation):
        self.decimation = decimation

    def get_bits_per_sym(self):
        return self.bits_per_sym

    def set_bits_per_sym(self, bits_per_sym):
        self.bits_per_sym = bits_per_sym

    def get_Sq_Thresh(self):
        return self.Sq_Thresh

    def set_Sq_Thresh(self, Sq_Thresh):
        self.Sq_Thresh = Sq_Thresh
        self._Sq_Thresh_slider.set_value(self.Sq_Thresh)
        self._Sq_Thresh_text_box.set_value(self.Sq_Thresh)
        self.analog_pwr_squelch_xx_0_0.set_threshold(self.Sq_Thresh)

    def get_RF_Gain(self):
        return self.RF_Gain

    def set_RF_Gain(self, RF_Gain):
        self.RF_Gain = RF_Gain
        self._RF_Gain_slider.set_value(self.RF_Gain)
        self._RF_Gain_text_box.set_value(self.RF_Gain)
        self.osmosdr_source_0_0.set_gain(self.RF_Gain, 0)

    def get_Fc(self):
        return self.Fc

    def set_Fc(self, Fc):
        self.Fc = Fc
        self._Fc_slider.set_value(self.Fc)
        self._Fc_text_box.set_value(self.Fc)
        self.osmosdr_source_0.set_center_freq(self.Fc, 0)
        self.osmosdr_source_0_0.set_center_freq(self.Fc+10e6, 0)

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = top_block()
    tb.Start(True)
    tb.Wait()

