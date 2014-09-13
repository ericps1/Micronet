#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/user/gnuradio/gr-RPi_Rx/lib
export PATH=/home/user/gnuradio/gr-RPi_Rx/build/lib:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-RPi_Rx 
