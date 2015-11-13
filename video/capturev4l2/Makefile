CC = arm-linux-gnueabi-gcc
CXX = arm-linux-gnueabi-g++
CFLAGS = -W -Wall -march=armv7-a
CXXFLAGS = $(CFLAGS)
LDFLAGS = 
EXEC = capturev4l2.elf

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

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

