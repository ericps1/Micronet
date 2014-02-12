from rtlsdr import *
from pylab import *
from operator import add
from scipy import signal
import numpy as np
import matplotlib.ticker as tckr
import matplotlib.pyplot as plt

# Interactive matplotlib and device definition
plt.ion()
sdr0 = RtlSdr()

# Settings
# avg_method should be 'running' or 'chunk'. Running will be more computationally expensive.
f_min = 850e6
f_max = 900e6
f_cal = 200e6
gain = 25
sample_rate = 3000e3
total_bins = 16384
avg = 5
avg_method = 'chunk' 			
cal = 'on'
cal_iterations = 100

# Other variables
bw = f_max-f_min
max_fft_length = 16384
delete = 0
iterations = int(bw/sample_rate)
if iterations == 0:
	iterations = 1

# Set FFT length to achieve closest possible number of total bins
if bw < sample_rate:
	fft_length = int(total_bins * 2**(round(np.log2(sample_rate/bw))))
else:
	fft_length = total_bins/iterations
	fft_length = int(2**round(np.log2(fft_length)))

# Limit FFT size to 256 < N < max_fft_length
if fft_length > max_fft_length:
	fft_length = max_fft_length
if fft_length < 256:
	decim = 256/fft_length
	fft_length = 256
else:
	decim = 1
decim = 1

# Configure device
sdr0.center_freq = f_cal
sdr0.sample_rate = sample_rate
sdr0.gain = gain

# Create normalized window function of proper length
window = signal.blackmanharris(fft_length)
window = window/np.mean(window)

# Calculate response of device for compensation. This should
# be done at a frequency where there is no sgnal to interfere
if cal == 'on':
	print 'Calibrating the Device\'s response'
	for y in range(0,cal_iterations):
		samples = sdr0.read_samples(fft_length)
		FFT = abs(fft(window*samples,fft_length))
		#if decim > 1:
		#	FFT = signal.decimate(FFT, decim)
		if y == 0:
			FFT_sum = FFT
		else:
			FFT_sum = FFT_sum + FFT
	response = FFT_sum/cal_iterations
elif cal == 'off':
	response = 1

# Plot Settings
fig = plt.figure()
ax = fig.add_subplot(1,1,1)
def form(x,p):
	return "%.2f" % (x * (10 ** (-6)))
ax.get_xaxis().set_major_formatter(tckr.FuncFormatter(form))
xlabel('Frequency (MHz)')
ylabel('Relative power (dB)')
axis([f_min,f_max,-110,-40])
f_step = sample_rate/(fft_length/decim)

# Scan Spectrum
sdr0.center_freq = f_min + sample_rate/2
if avg_method == 'running':
	y = 0
	FFT_mem = [0]*avg*iterations
while(1):
	for x in range(0,iterations):
		f = f_min + (x+0.5)*sample_rate
		f_start = int((f-sample_rate/2))
		f_stop = int((f+sample_rate/2))
		f_dom = np.arange(f_start, f_stop, f_step)

		if avg_method == 'chunk':
			for y in range(0, avg):
				samples = sdr0.read_samples(fft_length)
				if y == 0:
					FFT = abs(fft(window*samples,fft_length))/(sample_rate*fft_length*response)
				else:
					FFT = FFT + abs(fft(window*samples,fft_length))/(sample_rate*fft_length*response)
			sdr0.center_freq = f + sample_rate
			FFT = FFT/avg
			FFT = np.fft.fftshift(FFT)
			if decim > 1:
				FFT = FFT.reshape(decim,fft_length/decim)
				FFT = np.sum(FFT, axis=0)
			FFT = 10*log10(FFT)
			ax.plot(f_dom, FFT, 'r')
			if delete == 1:
				del ax.lines[0]
			draw()

		if avg_method == 'running':
			samples = sdr0.read_samples(fft_length)
			FFT = 10*log10(abs(fft(window*samples,fft_length))/(sample_rate*fft_length*response))
			if decim > 1:
				FFT = signal.decimate(FFT, decim,1)
			if y == 0:
				FFT_mem[x*avg] = FFT
			else:
				FFT_mem[x*avg+1:(x+1)*avg] = FFT_mem[x*avg:(x+1)*avg-1]
				FFT_mem[x*avg] = FFT
			FFT_sum = FFT
			for j in range(0,y):
				FFT_sum = map(add, FFT_sum, FFT_mem[x*avg+j])
			FFT_avg = [z/(y+1) for z in FFT_sum]
			FFT_avg = np.fft.fftshift(FFT_avg)
			ax.plot(f_dom, FFT_avg, 'r')
			if delete == 1:
				del ax.lines[0]
			if x == iterations:
				delete = 1
			draw()				
			sdr0.center_freq = f + sample_rate
	if y < avg:
		y = y + 1
	delete = 1
