#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Tue Mar 11 19:03:46 2014
##################################################

from gnuradio import analog
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import forms
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import osmosdr
import threading
import time
import wx
import math
import paramiko

class top_block(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Top Block")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.stop_freq = stop_freq = 106.1e6
        self.step = step = 200e3
        self.start_freq = start_freq = 104.1e6
        self.samp_rate = samp_rate = 1024e3
        self.mag_probe = mag_probe = 0
        self.decim = decim = 4
        self.center_freq = center_freq = 105.3e6
        self.adj_ch_weight = adj_ch_weight = 1
        self.Run_DSA = Run_DSA = 'Run'

        ##################################################
        # Blocks
        ##################################################
        self.mag = analog.probe_avg_mag_sqrd_c(0, 0.01)
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=1024,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.GridAdd(self.wxgui_fftsink2_0.win, 0, 0, 1, 5)
        self._stop_freq_text_box = forms.text_box(
        	parent=self.GetWin(),
        	value=self.stop_freq,
        	callback=self.set_stop_freq,
        	label="Frequency Stop",
        	converter=forms.float_converter(),
        )
        self.GridAdd(self._stop_freq_text_box, 1, 2, 1, 1)
        self._step_text_box = forms.text_box(
        	parent=self.GetWin(),
        	value=self.step,
        	callback=self.set_step,
        	label="Frequency Step",
        	converter=forms.float_converter(),
        )
        self.GridAdd(self._step_text_box, 1, 3, 1, 1)
        self._start_freq_text_box = forms.text_box(
        	parent=self.GetWin(),
        	value=self.start_freq,
        	callback=self.set_start_freq,
        	label="Frequency Start",
        	converter=forms.float_converter(),
        )
        self.GridAdd(self._start_freq_text_box, 1, 1, 1, 1)
        self.osmosdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + 'rtl=0' )
        self.osmosdr_source_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0.set_center_freq(center_freq, 0)
        self.osmosdr_source_0.set_freq_corr(0, 0)
        self.osmosdr_source_0.set_dc_offset_mode(0, 0)
        self.osmosdr_source_0.set_iq_balance_mode(0, 0)
        self.osmosdr_source_0.set_gain_mode(0, 0)
        self.osmosdr_source_0.set_gain(10, 0)
        self.osmosdr_source_0.set_if_gain(20, 0)
        self.osmosdr_source_0.set_bb_gain(20, 0)
        self.osmosdr_source_0.set_antenna("", 0)
        self.osmosdr_source_0.set_bandwidth(0, 0)
          
        def _mag_probe_probe():
        	while True:
        		val = self.mag.level()
        		try: self.set_mag_probe(val)
        		except AttributeError, e: pass
        		time.sleep(1.0/(10))
        _mag_probe_thread = threading.Thread(target=_mag_probe_probe)
        _mag_probe_thread.daemon = True
        _mag_probe_thread.start()
        self.low_pass_filter_0 = filter.fir_filter_ccf(decim, firdes.low_pass(
        	1, samp_rate, 256e3, 10e3, firdes.WIN_HAMMING, 6.76))
        self._adj_ch_weight_text_box = forms.text_box(
        	parent=self.GetWin(),
        	value=self.adj_ch_weight,
        	callback=self.set_adj_ch_weight,
        	label="Adjacent Channel Weight",
        	converter=forms.float_converter(),
        )
        self.GridAdd(self._adj_ch_weight_text_box, 1, 4, 1, 1)
        self._Run_DSA_chooser = forms.button(
        	parent=self.GetWin(),
        	value=self.Run_DSA,
        	callback=self.set_Run_DSA,
        	label="DSA",
        	choices=['Run'],
        	labels=[],
        )
        self.GridAdd(self._Run_DSA_chooser, 1, 0, 1, 1)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.osmosdr_source_0, 0), (self.low_pass_filter_0, 0))
        self.connect((self.low_pass_filter_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.low_pass_filter_0, 0), (self.mag, 0))


# QT sink close method reimplementation

    def get_stop_freq(self):
        return self.stop_freq

    def set_stop_freq(self, stop_freq):
        self.stop_freq = stop_freq
        self._stop_freq_text_box.set_value(self.stop_freq)

    def get_step(self):
        return self.step

    def set_step(self, step):
        self.step = step
        self._step_text_box.set_value(self.step)

    def get_start_freq(self):
        return self.start_freq

    def set_start_freq(self, start_freq):
        self.start_freq = start_freq
        self._start_freq_text_box.set_value(self.start_freq)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.osmosdr_source_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)
        self.low_pass_filter_0.set_taps(firdes.low_pass(1, self.samp_rate, 256e3, 10e3, firdes.WIN_HAMMING, 6.76))

    def get_mag_probe(self):
        return self.mag_probe

    def set_mag_probe(self, mag_probe):
        self.mag_probe = mag_probe

    def get_decim(self):
        return self.decim

    def set_decim(self, decim):
        self.decim = decim

    def get_center_freq_0(self):
        return self.center_freq_0

    def set_center_freq_0(self, center_freq_0):
        self.center_freq_0 = center_freq_0

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.osmosdr_source_0.set_center_freq(self.center_freq, 0)
        
    def get_adj_ch_weight(self):
        return self.adj_ch_weight

    def set_adj_ch_weight(self, adj_ch_weight):
        self.adj_ch_weight = adj_ch_weight
        self._adj_ch_weight_text_box.set_value(self.adj_ch_weight)

    def get_Run_DSA(self):
        return self.Run_DSA
        
#################################################################################################
    def set_Run_DSA(self, Run_DSA):
        self.Run_DSA = Run_DSA
        self._Run_DSA_chooser.set_value(self.Run_DSA)
        
        # Setup SSH link
		RPi = paramiko.SSHClient()
		RPi.set_missing_host_key_policy(paramiko.AutoAddPolicy())
		RPi.connect('10.0.0.12', username='pi', password='raspberry')
    	
    	f_opt, f_opt_ch_pow = self.find_opt_f()
    	for i in range(50):    		
    		# transmit packets
    		if f_opt != 0:
    			print "Transmitting burst at: ", f_opt, " MHz\n"
    			# Run pifm_rebuild
    			stdin, stdout, stderr = ssh.exec_command("sudo ~/PiTx/pifm_rebuild")
    			stdin.write('raspberry\n')
				stdin.flush()
				data = stdout.read.splitlines()
				for line in data:
        			print line
    		else:
    			print "All channels are occupied\n"
    		time.sleep(1)
    		# check for empty channel
    		print "Rechecking vacancy of channel"
    		check = self.check_chan()
    		print check, f_opt_ch_pow	    		
    		if check > 2*f_opt_ch_pow:
    			f_opt, f_opt_ch_pow = self.find_opt_f()
    
    def find_opt_f(self):
    	print "Scanning allowable frequencies for optimal transmit frequency"
    	start_freq = self.get_start_freq()
    	stop_freq =self.get_stop_freq()
    	step = self.get_step()
    	iterations = int(math.ceil((stop_freq-start_freq)/step))
    	
    	# Create an array of measured power
    	powers = [0]*iterations
    	for i in range(iterations):
    		self.set_center_freq(start_freq+i*step)
    		time.sleep(1)
    		powers[i] = self.get_mag_probe()
    		print "Received signal power at center frequency ", start_freq+i*step, ":", powers[i]
 		
 		# Decide which frequency is optimal to transmit on by looking at the channel power of
 		# the channel itself as well as the two adjacent channels
 		adj_ch_weight = self.get_adj_ch_weight()
 		ch_pow = [0]*(iterations-2)
    	for j in range(iterations-2):
    		ch_pow[j] = (powers[j]+powers[j+2])*adj_ch_weight + powers[j+1]
    	
    	# Find min of ch_pow
    	k = ch_pow.index(min(ch_pow))
    	f_opt_ch_pow = ch_pow[k]
    	f_opt = start_freq + k*step
    	
    	# Determine if it is reasonably vacant based on power level
    	if f_opt_ch_pow > 1e-3:
    		f_opt = 0
    	
    	return (f_opt, f_opt_ch_pow)
    	
    def check_chan(self):
    	step = self.get_step()
    	center_freq = self.get_center_freq()
    	adj_ch_weight = self.get_adj_ch_weight()
    	ch_pow = self.get_mag_probe()
    	
    	self.set_center_freq(center_freq-step)
    	ch_pow = ch_pow + adj_ch_weight*self.get_mag_probe()
    	self.set_center_freq(center_freq+step)
    	ch_pow = ch_pow + adj_ch_weight*self.get_mag_probe()
    	
    	return ch_pow
    	
################################################################################################### 

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
    tb = top_block()
    tb.Start(True)
    tb.Wait()
