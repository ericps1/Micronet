/* -*- c++ -*- */

#define RPI_RX_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "RPi_Rx_swig_doc.i"

%{
#include "RPi_Rx/RPi_Rx.h"
#include "RPi_Rx/FSK_Rx.h"
%}


%include "RPi_Rx/RPi_Rx.h"
GR_SWIG_BLOCK_MAGIC2(RPi_Rx, RPi_Rx);
%include "RPi_Rx/FSK_Rx.h"
GR_SWIG_BLOCK_MAGIC2(RPi_Rx, FSK_Rx);

