#!/bin/bash

sudo apt-get -y update
sudo apt-get -y upgrade

# RTL Driver
sudo apt-get -y install cmake libusb-1.0-0-dev
cd ~
git clone git://git.osmocom.org/rtl-sdr.git
cd rtl-sdr/
mkdir build
cd build
cmake ../
make
sudo make install
sudo ldconfig
sudo cp ../rtl-sdr.rules /etc/udev/rules.d/rtl-sdr.rules

# Dependencies for gnuradio
sudo apt-get -y install xorg python-cheetah libboost-all-dev python-lxml python-wxgtk2.8 python-numpy python-lxml libfftw3-dev libsdl1.2-dev python-scipy python-matplotlib python-tk octave liboctave-dev libgsl0-dev python-sphinx libcppunit-dev libuhd-dev swig python-qt4-dev libqwt-dev libqwt5-qt4-dev git doxygen

# Gnuradio installation
cd ~
git clone http://gnuradio.org/git/gnuradio.git
cd ./gnuradio
mkdir ./build
cd ./build
sudo cmake -Dhave_mfpu_neon=0 -DCMAKE_CXX_FLAGS:STRING="-march=armv6 -mfpu=vfp -mfloat-abi=hard" -DCMAKE_C_FLAGS:STRING="-march=armv6 -mfpu=vfp -mfloat-abi=hard" ../
sudo ldconfig

# Setup swap space before compiling
sudo swapoff -a
sudo dd if=/dev/zero of=/myswap bs=1024 count=800k
sudo mkswap /myswap
sudo swapon /myswap
sudo make
sudo make install
sudo ldconfig

# Osmocom gr-blocks
cd ..
git clone git://git.osmocom.org/gr-osmosdr
cd gr-osmosdr/
mkdir build
cd build/
cmake ../
make
sudo make install
sudo ldconfig
sudo swapoff -artl

