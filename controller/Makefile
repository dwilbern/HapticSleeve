CC=g++
CFLAGS=-Wall -g -DDEBUG

CONTROLLER_SOURCES = \
		  src/SerialPort.c \
		  src/HapticSleeveModel.cc \
		  src/HapticSleeveInterface.cc \
		  src/HapticSleeveConsoleApp.cc

CONTROLLER_OBJS = \
		SerialPort.o \
		HapticSleeveModel.o \
		HapticSleeveInterface.o \
		HapticSleeveConsoleApp.o

.PHONY: all

all: controller

clean:
	rm -rf *.o HapticSleeveController

controller: $(CONTROLLER_SOURCES)
	$(CC) $(CFLAGS) -o HapticSleeveController $(CONTROLLER_SOURCES)

%.o: $(SOURCES)
	$(CC) -c $(CFLAGS) $< -o $@

