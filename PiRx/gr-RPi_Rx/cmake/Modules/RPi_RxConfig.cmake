INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_RPI_RX RPi_Rx)

FIND_PATH(
    RPI_RX_INCLUDE_DIRS
    NAMES RPi_Rx/api.h
    HINTS $ENV{RPI_RX_DIR}/include
        ${PC_RPI_RX_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    RPI_RX_LIBRARIES
    NAMES gnuradio-RPi_Rx
    HINTS $ENV{RPI_RX_DIR}/lib
        ${PC_RPI_RX_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(RPI_RX DEFAULT_MSG RPI_RX_LIBRARIES RPI_RX_INCLUDE_DIRS)
MARK_AS_ADVANCED(RPI_RX_LIBRARIES RPI_RX_INCLUDE_DIRS)

