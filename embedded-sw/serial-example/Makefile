ifndef ARCH
# Compile for host
CC = gcc
CXX = g++
CFLAGS = -Wall -std=gnu99
CXXFLAGS = -Wall
LDFLAGS = 
else
ifeq ($(ARCH), arm)
# Cross compile for arm
CC = arm-linux-gnueabi-gcc
CXX = arm-linux-gnueabi-g++
CFLAGS = -Wall -std=gnu99 -march=armv7-a
CXXFLAGS = -Wall -march=armv7-a
LDFLAGS = 
else
$(error Unknown architecture)
endif
endif

ifdef DEBUG
CFLAGS += -DDEBUG
CXXFLAGS += -DDEBUG
endif

all: serial.elf
	
serial.elf: serial.o
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf *.elf

