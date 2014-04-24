# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_custom_swig', [dirname(__file__)])
        except ImportError:
            import _custom_swig
            return _custom_swig
        if fp is not None:
            try:
                _mod = imp.load_module('_custom_swig', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _custom_swig = swig_import_helper()
    del swig_import_helper
else:
    import _custom_swig
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr



def high_res_timer_now():
  """high_res_timer_now() -> high_res_timer_type"""
  return _custom_swig.high_res_timer_now()

def high_res_timer_now_perfmon():
  """high_res_timer_now_perfmon() -> high_res_timer_type"""
  return _custom_swig.high_res_timer_now_perfmon()

def high_res_timer_tps():
  """high_res_timer_tps() -> high_res_timer_type"""
  return _custom_swig.high_res_timer_tps()

def high_res_timer_epoch():
  """high_res_timer_epoch() -> high_res_timer_type"""
  return _custom_swig.high_res_timer_epoch()
class UART(object):
    """<+description of block+>"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    def make(*args, **kwargs):
        """
        make(int samp_per_sym, int word_length) -> sptr

        Return a shared_ptr to a new instance of custom::UART.

        To avoid accidental use of raw pointers, custom::UART's constructor is in a private implementation class. custom::UART::make is the public interface for creating new instances.

        Params: (samp_per_sym, word_length)
        """
        return _custom_swig.UART_make(*args, **kwargs)

    make = staticmethod(make)
    __swig_destroy__ = _custom_swig.delete_UART
    __del__ = lambda self : None;
UART_swigregister = _custom_swig.UART_swigregister
UART_swigregister(UART)

def UART_make(*args, **kwargs):
  """
    UART_make(int samp_per_sym, int word_length) -> sptr

    Return a shared_ptr to a new instance of custom::UART.

    To avoid accidental use of raw pointers, custom::UART's constructor is in a private implementation class. custom::UART::make is the public interface for creating new instances.

    Params: (samp_per_sym, word_length)
    """
  return _custom_swig.UART_make(*args, **kwargs)

class UART_sptr(object):
    """Proxy of C++ boost::shared_ptr<(gr::custom::UART)> class"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self) -> UART_sptr
        __init__(self, UART p) -> UART_sptr
        """
        this = _custom_swig.new_UART_sptr(*args)
        try: self.this.append(this)
        except: self.this = this
    def __deref__(self):
        """__deref__(self) -> UART"""
        return _custom_swig.UART_sptr___deref__(self)

    __swig_destroy__ = _custom_swig.delete_UART_sptr
    __del__ = lambda self : None;
    def make(self, *args, **kwargs):
        """
        make(self, int samp_per_sym, int word_length) -> sptr

        Return a shared_ptr to a new instance of custom::UART.

        To avoid accidental use of raw pointers, custom::UART's constructor is in a private implementation class. custom::UART::make is the public interface for creating new instances.

        Params: (samp_per_sym, word_length)
        """
        return _custom_swig.UART_sptr_make(self, *args, **kwargs)

    def history(self):
        """history(self) -> unsigned int"""
        return _custom_swig.UART_sptr_history(self)

    def declare_sample_delay(self, *args):
        """
        declare_sample_delay(self, int which, int delay)
        declare_sample_delay(self, unsigned int delay)
        """
        return _custom_swig.UART_sptr_declare_sample_delay(self, *args)

    def sample_delay(self, *args, **kwargs):
        """sample_delay(self, int which) -> unsigned int"""
        return _custom_swig.UART_sptr_sample_delay(self, *args, **kwargs)

    def output_multiple(self):
        """output_multiple(self) -> int"""
        return _custom_swig.UART_sptr_output_multiple(self)

    def relative_rate(self):
        """relative_rate(self) -> double"""
        return _custom_swig.UART_sptr_relative_rate(self)

    def start(self):
        """start(self) -> bool"""
        return _custom_swig.UART_sptr_start(self)

    def stop(self):
        """stop(self) -> bool"""
        return _custom_swig.UART_sptr_stop(self)

    def nitems_read(self, *args, **kwargs):
        """nitems_read(self, unsigned int which_input) -> uint64_t"""
        return _custom_swig.UART_sptr_nitems_read(self, *args, **kwargs)

    def nitems_written(self, *args, **kwargs):
        """nitems_written(self, unsigned int which_output) -> uint64_t"""
        return _custom_swig.UART_sptr_nitems_written(self, *args, **kwargs)

    def max_noutput_items(self):
        """max_noutput_items(self) -> int"""
        return _custom_swig.UART_sptr_max_noutput_items(self)

    def set_max_noutput_items(self, *args, **kwargs):
        """set_max_noutput_items(self, int m)"""
        return _custom_swig.UART_sptr_set_max_noutput_items(self, *args, **kwargs)

    def unset_max_noutput_items(self):
        """unset_max_noutput_items(self)"""
        return _custom_swig.UART_sptr_unset_max_noutput_items(self)

    def is_set_max_noutput_items(self):
        """is_set_max_noutput_items(self) -> bool"""
        return _custom_swig.UART_sptr_is_set_max_noutput_items(self)

    def max_output_buffer(self, *args, **kwargs):
        """max_output_buffer(self, int i) -> long"""
        return _custom_swig.UART_sptr_max_output_buffer(self, *args, **kwargs)

    def set_max_output_buffer(self, *args):
        """
        set_max_output_buffer(self, long max_output_buffer)
        set_max_output_buffer(self, int port, long max_output_buffer)
        """
        return _custom_swig.UART_sptr_set_max_output_buffer(self, *args)

    def min_output_buffer(self, *args, **kwargs):
        """min_output_buffer(self, int i) -> long"""
        return _custom_swig.UART_sptr_min_output_buffer(self, *args, **kwargs)

    def set_min_output_buffer(self, *args):
        """
        set_min_output_buffer(self, long min_output_buffer)
        set_min_output_buffer(self, int port, long min_output_buffer)
        """
        return _custom_swig.UART_sptr_set_min_output_buffer(self, *args)

    def pc_noutput_items(self):
        """pc_noutput_items(self) -> float"""
        return _custom_swig.UART_sptr_pc_noutput_items(self)

    def pc_noutput_items_avg(self):
        """pc_noutput_items_avg(self) -> float"""
        return _custom_swig.UART_sptr_pc_noutput_items_avg(self)

    def pc_noutput_items_var(self):
        """pc_noutput_items_var(self) -> float"""
        return _custom_swig.UART_sptr_pc_noutput_items_var(self)

    def pc_nproduced(self):
        """pc_nproduced(self) -> float"""
        return _custom_swig.UART_sptr_pc_nproduced(self)

    def pc_nproduced_avg(self):
        """pc_nproduced_avg(self) -> float"""
        return _custom_swig.UART_sptr_pc_nproduced_avg(self)

    def pc_nproduced_var(self):
        """pc_nproduced_var(self) -> float"""
        return _custom_swig.UART_sptr_pc_nproduced_var(self)

    def pc_input_buffers_full(self, *args):
        """
        pc_input_buffers_full(self, int which) -> float
        pc_input_buffers_full(self) -> pmt_vector_float
        """
        return _custom_swig.UART_sptr_pc_input_buffers_full(self, *args)

    def pc_input_buffers_full_avg(self, *args):
        """
        pc_input_buffers_full_avg(self, int which) -> float
        pc_input_buffers_full_avg(self) -> pmt_vector_float
        """
        return _custom_swig.UART_sptr_pc_input_buffers_full_avg(self, *args)

    def pc_input_buffers_full_var(self, *args):
        """
        pc_input_buffers_full_var(self, int which) -> float
        pc_input_buffers_full_var(self) -> pmt_vector_float
        """
        return _custom_swig.UART_sptr_pc_input_buffers_full_var(self, *args)

    def pc_output_buffers_full(self, *args):
        """
        pc_output_buffers_full(self, int which) -> float
        pc_output_buffers_full(self) -> pmt_vector_float
        """
        return _custom_swig.UART_sptr_pc_output_buffers_full(self, *args)

    def pc_output_buffers_full_avg(self, *args):
        """
        pc_output_buffers_full_avg(self, int which) -> float
        pc_output_buffers_full_avg(self) -> pmt_vector_float
        """
        return _custom_swig.UART_sptr_pc_output_buffers_full_avg(self, *args)

    def pc_output_buffers_full_var(self, *args):
        """
        pc_output_buffers_full_var(self, int which) -> float
        pc_output_buffers_full_var(self) -> pmt_vector_float
        """
        return _custom_swig.UART_sptr_pc_output_buffers_full_var(self, *args)

    def pc_work_time(self):
        """pc_work_time(self) -> float"""
        return _custom_swig.UART_sptr_pc_work_time(self)

    def pc_work_time_avg(self):
        """pc_work_time_avg(self) -> float"""
        return _custom_swig.UART_sptr_pc_work_time_avg(self)

    def pc_work_time_var(self):
        """pc_work_time_var(self) -> float"""
        return _custom_swig.UART_sptr_pc_work_time_var(self)

    def pc_work_time_total(self):
        """pc_work_time_total(self) -> float"""
        return _custom_swig.UART_sptr_pc_work_time_total(self)

    def set_processor_affinity(self, *args, **kwargs):
        """set_processor_affinity(self, __dummy_5__ mask)"""
        return _custom_swig.UART_sptr_set_processor_affinity(self, *args, **kwargs)

    def unset_processor_affinity(self):
        """unset_processor_affinity(self)"""
        return _custom_swig.UART_sptr_unset_processor_affinity(self)

    def processor_affinity(self):
        """processor_affinity(self) -> __dummy_5__"""
        return _custom_swig.UART_sptr_processor_affinity(self)

    def active_thread_priority(self):
        """active_thread_priority(self) -> int"""
        return _custom_swig.UART_sptr_active_thread_priority(self)

    def thread_priority(self):
        """thread_priority(self) -> int"""
        return _custom_swig.UART_sptr_thread_priority(self)

    def set_thread_priority(self, *args, **kwargs):
        """set_thread_priority(self, int priority) -> int"""
        return _custom_swig.UART_sptr_set_thread_priority(self, *args, **kwargs)

    def name(self):
        """name(self) -> string"""
        return _custom_swig.UART_sptr_name(self)

    def symbol_name(self):
        """symbol_name(self) -> string"""
        return _custom_swig.UART_sptr_symbol_name(self)

    def input_signature(self):
        """input_signature(self) -> sptr"""
        return _custom_swig.UART_sptr_input_signature(self)

    def output_signature(self):
        """output_signature(self) -> sptr"""
        return _custom_swig.UART_sptr_output_signature(self)

    def unique_id(self):
        """unique_id(self) -> long"""
        return _custom_swig.UART_sptr_unique_id(self)

    def to_basic_block(self):
        """to_basic_block(self) -> basic_block_sptr"""
        return _custom_swig.UART_sptr_to_basic_block(self)

    def check_topology(self, *args, **kwargs):
        """check_topology(self, int ninputs, int noutputs) -> bool"""
        return _custom_swig.UART_sptr_check_topology(self, *args, **kwargs)

    def alias(self):
        """alias(self) -> string"""
        return _custom_swig.UART_sptr_alias(self)

    def set_block_alias(self, *args, **kwargs):
        """set_block_alias(self, string name)"""
        return _custom_swig.UART_sptr_set_block_alias(self, *args, **kwargs)

    def _post(self, *args, **kwargs):
        """_post(self, pmt_t which_port, pmt_t msg)"""
        return _custom_swig.UART_sptr__post(self, *args, **kwargs)

    def message_ports_in(self):
        """message_ports_in(self) -> pmt_t"""
        return _custom_swig.UART_sptr_message_ports_in(self)

    def message_ports_out(self):
        """message_ports_out(self) -> pmt_t"""
        return _custom_swig.UART_sptr_message_ports_out(self)

    def message_subscribers(self, *args, **kwargs):
        """message_subscribers(self, pmt_t which_port) -> pmt_t"""
        return _custom_swig.UART_sptr_message_subscribers(self, *args, **kwargs)

UART_sptr_swigregister = _custom_swig.UART_sptr_swigregister
UART_sptr_swigregister(UART_sptr)

UART_sptr.__repr__ = lambda self: "<gr_block %s (%d)>" % (self.name(), self.unique_id())
UART = UART.make;

class FSK_Rx(object):
    """<+description of block+>"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    def make(*args, **kwargs):
        """
        make(int bits_per_sym, int samp_per_sym, int word_length) -> sptr

        Return a shared_ptr to a new instance of custom::FSK_Rx.

        To avoid accidental use of raw pointers, custom::FSK_Rx's constructor is in a private implementation class. custom::FSK_Rx::make is the public interface for creating new instances.

        Params: (bits_per_sym, samp_per_sym, word_length)
        """
        return _custom_swig.FSK_Rx_make(*args, **kwargs)

    make = staticmethod(make)
    __swig_destroy__ = _custom_swig.delete_FSK_Rx
    __del__ = lambda self : None;
FSK_Rx_swigregister = _custom_swig.FSK_Rx_swigregister
FSK_Rx_swigregister(FSK_Rx)

def FSK_Rx_make(*args, **kwargs):
  """
    FSK_Rx_make(int bits_per_sym, int samp_per_sym, int word_length) -> sptr

    Return a shared_ptr to a new instance of custom::FSK_Rx.

    To avoid accidental use of raw pointers, custom::FSK_Rx's constructor is in a private implementation class. custom::FSK_Rx::make is the public interface for creating new instances.

    Params: (bits_per_sym, samp_per_sym, word_length)
    """
  return _custom_swig.FSK_Rx_make(*args, **kwargs)

class FSK_Rx_sptr(object):
    """Proxy of C++ boost::shared_ptr<(gr::custom::FSK_Rx)> class"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self) -> FSK_Rx_sptr
        __init__(self, FSK_Rx p) -> FSK_Rx_sptr
        """
        this = _custom_swig.new_FSK_Rx_sptr(*args)
        try: self.this.append(this)
        except: self.this = this
    def __deref__(self):
        """__deref__(self) -> FSK_Rx"""
        return _custom_swig.FSK_Rx_sptr___deref__(self)

    __swig_destroy__ = _custom_swig.delete_FSK_Rx_sptr
    __del__ = lambda self : None;
    def make(self, *args, **kwargs):
        """
        make(self, int bits_per_sym, int samp_per_sym, int word_length) -> sptr

        Return a shared_ptr to a new instance of custom::FSK_Rx.

        To avoid accidental use of raw pointers, custom::FSK_Rx's constructor is in a private implementation class. custom::FSK_Rx::make is the public interface for creating new instances.

        Params: (bits_per_sym, samp_per_sym, word_length)
        """
        return _custom_swig.FSK_Rx_sptr_make(self, *args, **kwargs)

    def history(self):
        """history(self) -> unsigned int"""
        return _custom_swig.FSK_Rx_sptr_history(self)

    def declare_sample_delay(self, *args):
        """
        declare_sample_delay(self, int which, int delay)
        declare_sample_delay(self, unsigned int delay)
        """
        return _custom_swig.FSK_Rx_sptr_declare_sample_delay(self, *args)

    def sample_delay(self, *args, **kwargs):
        """sample_delay(self, int which) -> unsigned int"""
        return _custom_swig.FSK_Rx_sptr_sample_delay(self, *args, **kwargs)

    def output_multiple(self):
        """output_multiple(self) -> int"""
        return _custom_swig.FSK_Rx_sptr_output_multiple(self)

    def relative_rate(self):
        """relative_rate(self) -> double"""
        return _custom_swig.FSK_Rx_sptr_relative_rate(self)

    def start(self):
        """start(self) -> bool"""
        return _custom_swig.FSK_Rx_sptr_start(self)

    def stop(self):
        """stop(self) -> bool"""
        return _custom_swig.FSK_Rx_sptr_stop(self)

    def nitems_read(self, *args, **kwargs):
        """nitems_read(self, unsigned int which_input) -> uint64_t"""
        return _custom_swig.FSK_Rx_sptr_nitems_read(self, *args, **kwargs)

    def nitems_written(self, *args, **kwargs):
        """nitems_written(self, unsigned int which_output) -> uint64_t"""
        return _custom_swig.FSK_Rx_sptr_nitems_written(self, *args, **kwargs)

    def max_noutput_items(self):
        """max_noutput_items(self) -> int"""
        return _custom_swig.FSK_Rx_sptr_max_noutput_items(self)

    def set_max_noutput_items(self, *args, **kwargs):
        """set_max_noutput_items(self, int m)"""
        return _custom_swig.FSK_Rx_sptr_set_max_noutput_items(self, *args, **kwargs)

    def unset_max_noutput_items(self):
        """unset_max_noutput_items(self)"""
        return _custom_swig.FSK_Rx_sptr_unset_max_noutput_items(self)

    def is_set_max_noutput_items(self):
        """is_set_max_noutput_items(self) -> bool"""
        return _custom_swig.FSK_Rx_sptr_is_set_max_noutput_items(self)

    def max_output_buffer(self, *args, **kwargs):
        """max_output_buffer(self, int i) -> long"""
        return _custom_swig.FSK_Rx_sptr_max_output_buffer(self, *args, **kwargs)

    def set_max_output_buffer(self, *args):
        """
        set_max_output_buffer(self, long max_output_buffer)
        set_max_output_buffer(self, int port, long max_output_buffer)
        """
        return _custom_swig.FSK_Rx_sptr_set_max_output_buffer(self, *args)

    def min_output_buffer(self, *args, **kwargs):
        """min_output_buffer(self, int i) -> long"""
        return _custom_swig.FSK_Rx_sptr_min_output_buffer(self, *args, **kwargs)

    def set_min_output_buffer(self, *args):
        """
        set_min_output_buffer(self, long min_output_buffer)
        set_min_output_buffer(self, int port, long min_output_buffer)
        """
        return _custom_swig.FSK_Rx_sptr_set_min_output_buffer(self, *args)

    def pc_noutput_items(self):
        """pc_noutput_items(self) -> float"""
        return _custom_swig.FSK_Rx_sptr_pc_noutput_items(self)

    def pc_noutput_items_avg(self):
        """pc_noutput_items_avg(self) -> float"""
        return _custom_swig.FSK_Rx_sptr_pc_noutput_items_avg(self)

    def pc_noutput_items_var(self):
        """pc_noutput_items_var(self) -> float"""
        return _custom_swig.FSK_Rx_sptr_pc_noutput_items_var(self)

    def pc_nproduced(self):
        """pc_nproduced(self) -> float"""
        return _custom_swig.FSK_Rx_sptr_pc_nproduced(self)

    def pc_nproduced_avg(self):
        """pc_nproduced_avg(self) -> float"""
        return _custom_swig.FSK_Rx_sptr_pc_nproduced_avg(self)

    def pc_nproduced_var(self):
        """pc_nproduced_var(self) -> float"""
        return _custom_swig.FSK_Rx_sptr_pc_nproduced_var(self)

    def pc_input_buffers_full(self, *args):
        """
        pc_input_buffers_full(self, int which) -> float
        pc_input_buffers_full(self) -> pmt_vector_float
        """
        return _custom_swig.FSK_Rx_sptr_pc_input_buffers_full(self, *args)

    def pc_input_buffers_full_avg(self, *args):
        """
        pc_input_buffers_full_avg(self, int which) -> float
        pc_input_buffers_full_avg(self) -> pmt_vector_float
        """
        return _custom_swig.FSK_Rx_sptr_pc_input_buffers_full_avg(self, *args)

    def pc_input_buffers_full_var(self, *args):
        """
        pc_input_buffers_full_var(self, int which) -> float
        pc_input_buffers_full_var(self) -> pmt_vector_float
        """
        return _custom_swig.FSK_Rx_sptr_pc_input_buffers_full_var(self, *args)

    def pc_output_buffers_full(self, *args):
        """
        pc_output_buffers_full(self, int which) -> float
        pc_output_buffers_full(self) -> pmt_vector_float
        """
        return _custom_swig.FSK_Rx_sptr_pc_output_buffers_full(self, *args)

    def pc_output_buffers_full_avg(self, *args):
        """
        pc_output_buffers_full_avg(self, int which) -> float
        pc_output_buffers_full_avg(self) -> pmt_vector_float
        """
        return _custom_swig.FSK_Rx_sptr_pc_output_buffers_full_avg(self, *args)

    def pc_output_buffers_full_var(self, *args):
        """
        pc_output_buffers_full_var(self, int which) -> float
        pc_output_buffers_full_var(self) -> pmt_vector_float
        """
        return _custom_swig.FSK_Rx_sptr_pc_output_buffers_full_var(self, *args)

    def pc_work_time(self):
        """pc_work_time(self) -> float"""
        return _custom_swig.FSK_Rx_sptr_pc_work_time(self)

    def pc_work_time_avg(self):
        """pc_work_time_avg(self) -> float"""
        return _custom_swig.FSK_Rx_sptr_pc_work_time_avg(self)

    def pc_work_time_var(self):
        """pc_work_time_var(self) -> float"""
        return _custom_swig.FSK_Rx_sptr_pc_work_time_var(self)

    def pc_work_time_total(self):
        """pc_work_time_total(self) -> float"""
        return _custom_swig.FSK_Rx_sptr_pc_work_time_total(self)

    def set_processor_affinity(self, *args, **kwargs):
        """set_processor_affinity(self, __dummy_5__ mask)"""
        return _custom_swig.FSK_Rx_sptr_set_processor_affinity(self, *args, **kwargs)

    def unset_processor_affinity(self):
        """unset_processor_affinity(self)"""
        return _custom_swig.FSK_Rx_sptr_unset_processor_affinity(self)

    def processor_affinity(self):
        """processor_affinity(self) -> __dummy_5__"""
        return _custom_swig.FSK_Rx_sptr_processor_affinity(self)

    def active_thread_priority(self):
        """active_thread_priority(self) -> int"""
        return _custom_swig.FSK_Rx_sptr_active_thread_priority(self)

    def thread_priority(self):
        """thread_priority(self) -> int"""
        return _custom_swig.FSK_Rx_sptr_thread_priority(self)

    def set_thread_priority(self, *args, **kwargs):
        """set_thread_priority(self, int priority) -> int"""
        return _custom_swig.FSK_Rx_sptr_set_thread_priority(self, *args, **kwargs)

    def name(self):
        """name(self) -> string"""
        return _custom_swig.FSK_Rx_sptr_name(self)

    def symbol_name(self):
        """symbol_name(self) -> string"""
        return _custom_swig.FSK_Rx_sptr_symbol_name(self)

    def input_signature(self):
        """input_signature(self) -> sptr"""
        return _custom_swig.FSK_Rx_sptr_input_signature(self)

    def output_signature(self):
        """output_signature(self) -> sptr"""
        return _custom_swig.FSK_Rx_sptr_output_signature(self)

    def unique_id(self):
        """unique_id(self) -> long"""
        return _custom_swig.FSK_Rx_sptr_unique_id(self)

    def to_basic_block(self):
        """to_basic_block(self) -> basic_block_sptr"""
        return _custom_swig.FSK_Rx_sptr_to_basic_block(self)

    def check_topology(self, *args, **kwargs):
        """check_topology(self, int ninputs, int noutputs) -> bool"""
        return _custom_swig.FSK_Rx_sptr_check_topology(self, *args, **kwargs)

    def alias(self):
        """alias(self) -> string"""
        return _custom_swig.FSK_Rx_sptr_alias(self)

    def set_block_alias(self, *args, **kwargs):
        """set_block_alias(self, string name)"""
        return _custom_swig.FSK_Rx_sptr_set_block_alias(self, *args, **kwargs)

    def _post(self, *args, **kwargs):
        """_post(self, pmt_t which_port, pmt_t msg)"""
        return _custom_swig.FSK_Rx_sptr__post(self, *args, **kwargs)

    def message_ports_in(self):
        """message_ports_in(self) -> pmt_t"""
        return _custom_swig.FSK_Rx_sptr_message_ports_in(self)

    def message_ports_out(self):
        """message_ports_out(self) -> pmt_t"""
        return _custom_swig.FSK_Rx_sptr_message_ports_out(self)

    def message_subscribers(self, *args, **kwargs):
        """message_subscribers(self, pmt_t which_port) -> pmt_t"""
        return _custom_swig.FSK_Rx_sptr_message_subscribers(self, *args, **kwargs)

FSK_Rx_sptr_swigregister = _custom_swig.FSK_Rx_sptr_swigregister
FSK_Rx_sptr_swigregister(FSK_Rx_sptr)

FSK_Rx_sptr.__repr__ = lambda self: "<gr_block %s (%d)>" % (self.name(), self.unique_id())
FSK_Rx = FSK_Rx.make;

class RPi_Rx(object):
    """Proxy of C++ gr::custom::RPi_Rx class"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    def make(*args, **kwargs):
        """make(int bits_per_sym, int samp_per_sym, int word_length) -> sptr"""
        return _custom_swig.RPi_Rx_make(*args, **kwargs)

    make = staticmethod(make)
    __swig_destroy__ = _custom_swig.delete_RPi_Rx
    __del__ = lambda self : None;
RPi_Rx_swigregister = _custom_swig.RPi_Rx_swigregister
RPi_Rx_swigregister(RPi_Rx)

def RPi_Rx_make(*args, **kwargs):
  """RPi_Rx_make(int bits_per_sym, int samp_per_sym, int word_length) -> sptr"""
  return _custom_swig.RPi_Rx_make(*args, **kwargs)

class RPi_Rx_sptr(object):
    """Proxy of C++ boost::shared_ptr<(gr::custom::RPi_Rx)> class"""
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self) -> RPi_Rx_sptr
        __init__(self, RPi_Rx p) -> RPi_Rx_sptr
        """
        this = _custom_swig.new_RPi_Rx_sptr(*args)
        try: self.this.append(this)
        except: self.this = this
    def __deref__(self):
        """__deref__(self) -> RPi_Rx"""
        return _custom_swig.RPi_Rx_sptr___deref__(self)

    __swig_destroy__ = _custom_swig.delete_RPi_Rx_sptr
    __del__ = lambda self : None;
    def make(self, *args, **kwargs):
        """make(self, int bits_per_sym, int samp_per_sym, int word_length) -> sptr"""
        return _custom_swig.RPi_Rx_sptr_make(self, *args, **kwargs)

    def history(self):
        """history(self) -> unsigned int"""
        return _custom_swig.RPi_Rx_sptr_history(self)

    def declare_sample_delay(self, *args):
        """
        declare_sample_delay(self, int which, int delay)
        declare_sample_delay(self, unsigned int delay)
        """
        return _custom_swig.RPi_Rx_sptr_declare_sample_delay(self, *args)

    def sample_delay(self, *args, **kwargs):
        """sample_delay(self, int which) -> unsigned int"""
        return _custom_swig.RPi_Rx_sptr_sample_delay(self, *args, **kwargs)

    def output_multiple(self):
        """output_multiple(self) -> int"""
        return _custom_swig.RPi_Rx_sptr_output_multiple(self)

    def relative_rate(self):
        """relative_rate(self) -> double"""
        return _custom_swig.RPi_Rx_sptr_relative_rate(self)

    def start(self):
        """start(self) -> bool"""
        return _custom_swig.RPi_Rx_sptr_start(self)

    def stop(self):
        """stop(self) -> bool"""
        return _custom_swig.RPi_Rx_sptr_stop(self)

    def nitems_read(self, *args, **kwargs):
        """nitems_read(self, unsigned int which_input) -> uint64_t"""
        return _custom_swig.RPi_Rx_sptr_nitems_read(self, *args, **kwargs)

    def nitems_written(self, *args, **kwargs):
        """nitems_written(self, unsigned int which_output) -> uint64_t"""
        return _custom_swig.RPi_Rx_sptr_nitems_written(self, *args, **kwargs)

    def max_noutput_items(self):
        """max_noutput_items(self) -> int"""
        return _custom_swig.RPi_Rx_sptr_max_noutput_items(self)

    def set_max_noutput_items(self, *args, **kwargs):
        """set_max_noutput_items(self, int m)"""
        return _custom_swig.RPi_Rx_sptr_set_max_noutput_items(self, *args, **kwargs)

    def unset_max_noutput_items(self):
        """unset_max_noutput_items(self)"""
        return _custom_swig.RPi_Rx_sptr_unset_max_noutput_items(self)

    def is_set_max_noutput_items(self):
        """is_set_max_noutput_items(self) -> bool"""
        return _custom_swig.RPi_Rx_sptr_is_set_max_noutput_items(self)

    def max_output_buffer(self, *args, **kwargs):
        """max_output_buffer(self, int i) -> long"""
        return _custom_swig.RPi_Rx_sptr_max_output_buffer(self, *args, **kwargs)

    def set_max_output_buffer(self, *args):
        """
        set_max_output_buffer(self, long max_output_buffer)
        set_max_output_buffer(self, int port, long max_output_buffer)
        """
        return _custom_swig.RPi_Rx_sptr_set_max_output_buffer(self, *args)

    def min_output_buffer(self, *args, **kwargs):
        """min_output_buffer(self, int i) -> long"""
        return _custom_swig.RPi_Rx_sptr_min_output_buffer(self, *args, **kwargs)

    def set_min_output_buffer(self, *args):
        """
        set_min_output_buffer(self, long min_output_buffer)
        set_min_output_buffer(self, int port, long min_output_buffer)
        """
        return _custom_swig.RPi_Rx_sptr_set_min_output_buffer(self, *args)

    def pc_noutput_items(self):
        """pc_noutput_items(self) -> float"""
        return _custom_swig.RPi_Rx_sptr_pc_noutput_items(self)

    def pc_noutput_items_avg(self):
        """pc_noutput_items_avg(self) -> float"""
        return _custom_swig.RPi_Rx_sptr_pc_noutput_items_avg(self)

    def pc_noutput_items_var(self):
        """pc_noutput_items_var(self) -> float"""
        return _custom_swig.RPi_Rx_sptr_pc_noutput_items_var(self)

    def pc_nproduced(self):
        """pc_nproduced(self) -> float"""
        return _custom_swig.RPi_Rx_sptr_pc_nproduced(self)

    def pc_nproduced_avg(self):
        """pc_nproduced_avg(self) -> float"""
        return _custom_swig.RPi_Rx_sptr_pc_nproduced_avg(self)

    def pc_nproduced_var(self):
        """pc_nproduced_var(self) -> float"""
        return _custom_swig.RPi_Rx_sptr_pc_nproduced_var(self)

    def pc_input_buffers_full(self, *args):
        """
        pc_input_buffers_full(self, int which) -> float
        pc_input_buffers_full(self) -> pmt_vector_float
        """
        return _custom_swig.RPi_Rx_sptr_pc_input_buffers_full(self, *args)

    def pc_input_buffers_full_avg(self, *args):
        """
        pc_input_buffers_full_avg(self, int which) -> float
        pc_input_buffers_full_avg(self) -> pmt_vector_float
        """
        return _custom_swig.RPi_Rx_sptr_pc_input_buffers_full_avg(self, *args)

    def pc_input_buffers_full_var(self, *args):
        """
        pc_input_buffers_full_var(self, int which) -> float
        pc_input_buffers_full_var(self) -> pmt_vector_float
        """
        return _custom_swig.RPi_Rx_sptr_pc_input_buffers_full_var(self, *args)

    def pc_output_buffers_full(self, *args):
        """
        pc_output_buffers_full(self, int which) -> float
        pc_output_buffers_full(self) -> pmt_vector_float
        """
        return _custom_swig.RPi_Rx_sptr_pc_output_buffers_full(self, *args)

    def pc_output_buffers_full_avg(self, *args):
        """
        pc_output_buffers_full_avg(self, int which) -> float
        pc_output_buffers_full_avg(self) -> pmt_vector_float
        """
        return _custom_swig.RPi_Rx_sptr_pc_output_buffers_full_avg(self, *args)

    def pc_output_buffers_full_var(self, *args):
        """
        pc_output_buffers_full_var(self, int which) -> float
        pc_output_buffers_full_var(self) -> pmt_vector_float
        """
        return _custom_swig.RPi_Rx_sptr_pc_output_buffers_full_var(self, *args)

    def pc_work_time(self):
        """pc_work_time(self) -> float"""
        return _custom_swig.RPi_Rx_sptr_pc_work_time(self)

    def pc_work_time_avg(self):
        """pc_work_time_avg(self) -> float"""
        return _custom_swig.RPi_Rx_sptr_pc_work_time_avg(self)

    def pc_work_time_var(self):
        """pc_work_time_var(self) -> float"""
        return _custom_swig.RPi_Rx_sptr_pc_work_time_var(self)

    def pc_work_time_total(self):
        """pc_work_time_total(self) -> float"""
        return _custom_swig.RPi_Rx_sptr_pc_work_time_total(self)

    def set_processor_affinity(self, *args, **kwargs):
        """set_processor_affinity(self, __dummy_5__ mask)"""
        return _custom_swig.RPi_Rx_sptr_set_processor_affinity(self, *args, **kwargs)

    def unset_processor_affinity(self):
        """unset_processor_affinity(self)"""
        return _custom_swig.RPi_Rx_sptr_unset_processor_affinity(self)

    def processor_affinity(self):
        """processor_affinity(self) -> __dummy_5__"""
        return _custom_swig.RPi_Rx_sptr_processor_affinity(self)

    def active_thread_priority(self):
        """active_thread_priority(self) -> int"""
        return _custom_swig.RPi_Rx_sptr_active_thread_priority(self)

    def thread_priority(self):
        """thread_priority(self) -> int"""
        return _custom_swig.RPi_Rx_sptr_thread_priority(self)

    def set_thread_priority(self, *args, **kwargs):
        """set_thread_priority(self, int priority) -> int"""
        return _custom_swig.RPi_Rx_sptr_set_thread_priority(self, *args, **kwargs)

    def name(self):
        """name(self) -> string"""
        return _custom_swig.RPi_Rx_sptr_name(self)

    def symbol_name(self):
        """symbol_name(self) -> string"""
        return _custom_swig.RPi_Rx_sptr_symbol_name(self)

    def input_signature(self):
        """input_signature(self) -> sptr"""
        return _custom_swig.RPi_Rx_sptr_input_signature(self)

    def output_signature(self):
        """output_signature(self) -> sptr"""
        return _custom_swig.RPi_Rx_sptr_output_signature(self)

    def unique_id(self):
        """unique_id(self) -> long"""
        return _custom_swig.RPi_Rx_sptr_unique_id(self)

    def to_basic_block(self):
        """to_basic_block(self) -> basic_block_sptr"""
        return _custom_swig.RPi_Rx_sptr_to_basic_block(self)

    def check_topology(self, *args, **kwargs):
        """check_topology(self, int ninputs, int noutputs) -> bool"""
        return _custom_swig.RPi_Rx_sptr_check_topology(self, *args, **kwargs)

    def alias(self):
        """alias(self) -> string"""
        return _custom_swig.RPi_Rx_sptr_alias(self)

    def set_block_alias(self, *args, **kwargs):
        """set_block_alias(self, string name)"""
        return _custom_swig.RPi_Rx_sptr_set_block_alias(self, *args, **kwargs)

    def _post(self, *args, **kwargs):
        """_post(self, pmt_t which_port, pmt_t msg)"""
        return _custom_swig.RPi_Rx_sptr__post(self, *args, **kwargs)

    def message_ports_in(self):
        """message_ports_in(self) -> pmt_t"""
        return _custom_swig.RPi_Rx_sptr_message_ports_in(self)

    def message_ports_out(self):
        """message_ports_out(self) -> pmt_t"""
        return _custom_swig.RPi_Rx_sptr_message_ports_out(self)

    def message_subscribers(self, *args, **kwargs):
        """message_subscribers(self, pmt_t which_port) -> pmt_t"""
        return _custom_swig.RPi_Rx_sptr_message_subscribers(self, *args, **kwargs)

RPi_Rx_sptr_swigregister = _custom_swig.RPi_Rx_sptr_swigregister
RPi_Rx_sptr_swigregister(RPi_Rx_sptr)

RPi_Rx_sptr.__repr__ = lambda self: "<gr_block %s (%d)>" % (self.name(), self.unique_id())
RPi_Rx = RPi_Rx.make;



