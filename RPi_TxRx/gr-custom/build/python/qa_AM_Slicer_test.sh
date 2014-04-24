#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/user/gnuradio/gr-custom/python
export PATH=/home/user/gnuradio/gr-custom/build/python:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=/home/user/gnuradio/gr-custom/build/swig:$PYTHONPATH
/usr/bin/python /home/user/gnuradio/gr-custom/python/qa_AM_Slicer.py 
