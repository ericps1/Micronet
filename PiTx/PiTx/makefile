# the compiler: gcc for c program
CC=gcc

# compiler flags
#  -Wall turns on most compiler warnings
#  -c will compile but not link
CFLAGS= -Wall -lm

# Library Dependencies 
LDFLAGS=

# Source file
OBJ=main.o pitx.o dma_pitx.o file_handler.o
# Header files
DEPS=pitx.h dma_pitx.h file_handler.h

# Executable
TARGET=PITX

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $^

clean:
	rm -f *.o $(TARGET)

