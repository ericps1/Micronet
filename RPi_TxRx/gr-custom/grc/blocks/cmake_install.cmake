# Install script for directory: /home/user/gnuradio/grc/blocks

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "grc")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/user/gnuradio/grc/blocks/parameter.xml"
    "/home/user/gnuradio/grc/blocks/virtual_sink.xml"
    "/home/user/gnuradio/grc/blocks/blks2_tcp_source.xml"
    "/home/user/gnuradio/grc/blocks/xmlrpc_client.xml"
    "/home/user/gnuradio/grc/blocks/blks2_selector.xml"
    "/home/user/gnuradio/grc/blocks/options.xml"
    "/home/user/gnuradio/grc/blocks/blks2_valve.xml"
    "/home/user/gnuradio/grc/blocks/variable.xml"
    "/home/user/gnuradio/grc/blocks/blks2_tcp_sink.xml"
    "/home/user/gnuradio/grc/blocks/virtual_source.xml"
    "/home/user/gnuradio/grc/blocks/pad_sink.xml"
    "/home/user/gnuradio/grc/blocks/blks2_error_rate.xml"
    "/home/user/gnuradio/grc/blocks/variable_config.xml"
    "/home/user/gnuradio/grc/blocks/xmlrpc_server.xml"
    "/home/user/gnuradio/grc/blocks/block_tree.xml"
    "/home/user/gnuradio/grc/blocks/blks2_packet_decoder.xml"
    "/home/user/gnuradio/grc/blocks/import.xml"
    "/home/user/gnuradio/grc/blocks/bus_structure_source.xml"
    "/home/user/gnuradio/grc/blocks/note.xml"
    "/home/user/gnuradio/grc/blocks/blks2_packet_encoder.xml"
    "/home/user/gnuradio/grc/blocks/pad_source.xml"
    "/home/user/gnuradio/grc/blocks/bus_structure_sink.xml"
    "/home/user/gnuradio/grc/blocks/bus_sink.xml"
    "/home/user/gnuradio/grc/blocks/variable_function_probe.xml"
    "/home/user/gnuradio/grc/blocks/bus_source.xml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "grc")

