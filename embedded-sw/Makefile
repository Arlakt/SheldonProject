# Supposed to work on Cygwin, if the compiler is in $PATH

CC = arm-linux-gnueabi-gcc
CFLAGS = -Wall -march=armv7-a -std=gnu99 -pthread
LDFLAGS =

CFLAGS += -I .

OBJS = serial/serial.o movement/at_commands_builder.o movement/flight_functions.o movement/UDP_sender.o navdata/navdata.o threads/find_position.o threads/track_position.o threads/altitude.o

# $@ = cible
# $^ = toutes les dependances
# $< = premiere dependance

all: main.elf

main.elf: main/main.o $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJS)
	rm -f *.elf

