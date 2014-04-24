# the compiler: gcc for c program
CC=gcc

# compiler flags
#  -Wall turns on most compiler warnings
#  -c will compile but not link
CFLAGS= -Wall -lm

# Library Dependencies 
LDFLAGS=

# Source file
OBJ=main.o xmit.o dma_xmit.o file_handler.o
# Header files
DEPS=xmit.h dma_xmit.h file_handler.h

# Executable
TARGET=XMIT

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $^

clean:
	rm -f *.o $(TARGET)

