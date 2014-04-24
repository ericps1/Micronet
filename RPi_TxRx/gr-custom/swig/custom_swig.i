/* -*- c++ -*- */

#define CUSTOM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "custom_swig_doc.i"

%{
#include "custom/UART.h"
#include "custom/FSK_Rx.h"
#include "custom/RPi_Rx.h"
%}


%include "custom/UART.h"
GR_SWIG_BLOCK_MAGIC2(custom, UART);

%include "custom/FSK_Rx.h"
GR_SWIG_BLOCK_MAGIC2(custom, FSK_Rx);

%include "custom/RPi_Rx.h"
GR_SWIG_BLOCK_MAGIC2(custom, RPi_Rx);
