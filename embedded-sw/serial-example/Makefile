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
else
$(error Unknown architecture)
endif
endif
EXEC = serial.elf

SRCS_C = $(wildcard *.c)
SRCS_CPP = $(wildcard *.cpp)
OBJS = $(SRCS_C:.c=.o) $(SRCS_CPP:.cpp=.o) 

all: $(EXEC)
	
$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf $(EXEC)

