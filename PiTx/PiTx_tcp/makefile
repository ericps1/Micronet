# the compiler: gcc for c program
CC=gcc

# compiler flags
#  -Wall turns on most compiler warnings
#  -c will compile but not link
CFLAGS= -Wall -lm

# Library Dependencies 
LDFLAGS=

# Source file
OBJ=main.o pitx.o dma_pitx.o file_handler.o tcp_socket.o
# Header files
DEPS=pitx.h dma_pitx.h file_handler.h tcp_socket.h

# Executable
TARGET=TCP_PITX

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $^

clean:
	rm -f *.o $(TARGET)

