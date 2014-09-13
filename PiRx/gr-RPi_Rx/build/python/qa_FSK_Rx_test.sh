#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/user/gnuradio/gr-RPi_Rx/python
export PATH=/home/user/gnuradio/gr-RPi_Rx/build/python:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=/home/user/gnuradio/gr-RPi_Rx/build/swig:$PYTHONPATH
/usr/bin/python /home/user/gnuradio/gr-RPi_Rx/python/qa_FSK_Rx.py 
