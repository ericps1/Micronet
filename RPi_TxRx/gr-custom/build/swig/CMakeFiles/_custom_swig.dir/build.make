# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/gnuradio/gr-custom

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/gnuradio/gr-custom/build

# Include any dependencies generated for this target.
include swig/CMakeFiles/_custom_swig.dir/depend.make

# Include the progress variables for this target.
include swig/CMakeFiles/_custom_swig.dir/progress.make

# Include the compile flags for this target's objects.
include swig/CMakeFiles/_custom_swig.dir/flags.make

swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o: swig/CMakeFiles/_custom_swig.dir/flags.make
swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o: swig/custom_swigPYTHON_wrap.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/user/gnuradio/gr-custom/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o"
	cd /home/user/gnuradio/gr-custom/build/swig && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o -c /home/user/gnuradio/gr-custom/build/swig/custom_swigPYTHON_wrap.cxx

swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.i"
	cd /home/user/gnuradio/gr-custom/build/swig && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/user/gnuradio/gr-custom/build/swig/custom_swigPYTHON_wrap.cxx > CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.i

swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.s"
	cd /home/user/gnuradio/gr-custom/build/swig && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/user/gnuradio/gr-custom/build/swig/custom_swigPYTHON_wrap.cxx -o CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.s

swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o.requires:
.PHONY : swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o.requires

swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o.provides: swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o.requires
	$(MAKE) -f swig/CMakeFiles/_custom_swig.dir/build.make swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o.provides.build
.PHONY : swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o.provides

swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o.provides.build: swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o

swig/custom_swigPYTHON_wrap.cxx: /usr/local/include/gnuradio/swig/gnuradio.i
swig/custom_swigPYTHON_wrap.cxx: /usr/local/include/gnuradio/swig/gr_extras.i
swig/custom_swigPYTHON_wrap.cxx: swig/custom_swig_doc.i
swig/custom_swigPYTHON_wrap.cxx: /usr/local/include/gnuradio/swig/gr_shared_ptr.i
swig/custom_swigPYTHON_wrap.cxx: /usr/local/include/gnuradio/swig/gnuradio_swig_bug_workaround.h
swig/custom_swigPYTHON_wrap.cxx: ../swig/custom_swig.i
swig/custom_swigPYTHON_wrap.cxx: /usr/local/include/gnuradio/swig/gr_types.i
swig/custom_swigPYTHON_wrap.cxx: swig/custom_swig.tag
swig/custom_swigPYTHON_wrap.cxx: ../swig/custom_swig.i
	$(CMAKE_COMMAND) -E cmake_progress_report /home/user/gnuradio/gr-custom/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Swig source"
	cd /home/user/gnuradio/gr-custom/build/swig && /usr/bin/cmake -E make_directory /home/user/gnuradio/gr-custom/build/swig
	cd /home/user/gnuradio/gr-custom/build/swig && /usr/bin/swig2.0 -python -fvirtual -modern -keyword -w511 -module custom_swig -I/usr/local/include/gnuradio/swig -I/usr/include/python2.7 -I/usr/include/python2.7 -I/home/user/gnuradio/gr-custom/swig -I/home/user/gnuradio/gr-custom/build/swig -outdir /home/user/gnuradio/gr-custom/build/swig -c++ -I/home/user/gnuradio/gr-custom/lib -I/home/user/gnuradio/gr-custom/include -I/home/user/gnuradio/gr-custom/build/lib -I/home/user/gnuradio/gr-custom/build/include -I/usr/include -I/usr/local/include -I/usr/local/include/gnuradio/swig -I/usr/include/python2.7 -I/home/user/gnuradio/gr-custom/swig -I/home/user/gnuradio/gr-custom/build/swig -o /home/user/gnuradio/gr-custom/build/swig/custom_swigPYTHON_wrap.cxx /home/user/gnuradio/gr-custom/swig/custom_swig.i

swig/custom_swig.py: swig/custom_swigPYTHON_wrap.cxx

swig/custom_swig_doc.i: swig/custom_swig_doc_swig_docs/xml/index.xml
	$(CMAKE_COMMAND) -E cmake_progress_report /home/user/gnuradio/gr-custom/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating custom_swig_doc.i"
	cd /home/user/gnuradio/gr-custom/docs/doxygen && /usr/bin/python -B /home/user/gnuradio/gr-custom/docs/doxygen/swig_doc.py /home/user/gnuradio/gr-custom/build/swig/custom_swig_doc_swig_docs/xml /home/user/gnuradio/gr-custom/build/swig/custom_swig_doc.i

swig/custom_swig.tag: swig/custom_swig_doc.i
swig/custom_swig.tag: swig/_custom_swig_swig_tag
	$(CMAKE_COMMAND) -E cmake_progress_report /home/user/gnuradio/gr-custom/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating custom_swig.tag"
	cd /home/user/gnuradio/gr-custom/build/swig && ./_custom_swig_swig_tag
	cd /home/user/gnuradio/gr-custom/build/swig && /usr/bin/cmake -E touch /home/user/gnuradio/gr-custom/build/swig/custom_swig.tag

swig/custom_swig_doc_swig_docs/xml/index.xml: swig/_custom_swig_doc_tag
	$(CMAKE_COMMAND) -E cmake_progress_report /home/user/gnuradio/gr-custom/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating doxygen xml for custom_swig_doc docs"
	cd /home/user/gnuradio/gr-custom/build/swig && ./_custom_swig_doc_tag
	cd /home/user/gnuradio/gr-custom/build/swig && /usr/bin/doxygen /home/user/gnuradio/gr-custom/build/swig/custom_swig_doc_swig_docs/Doxyfile

# Object files for target _custom_swig
_custom_swig_OBJECTS = \
"CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o"

# External object files for target _custom_swig
_custom_swig_EXTERNAL_OBJECTS =

swig/_custom_swig.so: swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o
swig/_custom_swig.so: /usr/lib/libpython2.7.so
swig/_custom_swig.so: lib/libgnuradio-custom.so
swig/_custom_swig.so: /usr/lib/libboost_filesystem-mt.so
swig/_custom_swig.so: /usr/lib/libboost_system-mt.so
swig/_custom_swig.so: /usr/local/lib/libgnuradio-runtime.so
swig/_custom_swig.so: swig/CMakeFiles/_custom_swig.dir/build.make
swig/_custom_swig.so: swig/CMakeFiles/_custom_swig.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module _custom_swig.so"
	cd /home/user/gnuradio/gr-custom/build/swig && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/_custom_swig.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
swig/CMakeFiles/_custom_swig.dir/build: swig/_custom_swig.so
.PHONY : swig/CMakeFiles/_custom_swig.dir/build

swig/CMakeFiles/_custom_swig.dir/requires: swig/CMakeFiles/_custom_swig.dir/custom_swigPYTHON_wrap.cxx.o.requires
.PHONY : swig/CMakeFiles/_custom_swig.dir/requires

swig/CMakeFiles/_custom_swig.dir/clean:
	cd /home/user/gnuradio/gr-custom/build/swig && $(CMAKE_COMMAND) -P CMakeFiles/_custom_swig.dir/cmake_clean.cmake
.PHONY : swig/CMakeFiles/_custom_swig.dir/clean

swig/CMakeFiles/_custom_swig.dir/depend: swig/custom_swigPYTHON_wrap.cxx
swig/CMakeFiles/_custom_swig.dir/depend: swig/custom_swig.py
swig/CMakeFiles/_custom_swig.dir/depend: swig/custom_swig_doc.i
swig/CMakeFiles/_custom_swig.dir/depend: swig/custom_swig.tag
swig/CMakeFiles/_custom_swig.dir/depend: swig/custom_swig_doc_swig_docs/xml/index.xml
	cd /home/user/gnuradio/gr-custom/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/gnuradio/gr-custom /home/user/gnuradio/gr-custom/swig /home/user/gnuradio/gr-custom/build /home/user/gnuradio/gr-custom/build/swig /home/user/gnuradio/gr-custom/build/swig/CMakeFiles/_custom_swig.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : swig/CMakeFiles/_custom_swig.dir/depend

