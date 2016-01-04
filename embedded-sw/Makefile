CC = arm-linux-gnueabi-gcc
CFLAGS = -W -Wall -march=armv7-a -std=gnu99 -pthread
LDFLAGS = -pthread

CFLAGS += -I .

OBJS = serial/serial.o movement/at_commands_builder.o movement/flight_functions.o movement/UDP_sender.o threads/find_position.o threads/track_position.o

# $@ = cible
# $^ = toutes les dependances
# $< = premiere dependance

all: main.elf

main.elf: main/main.o $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm $(OBJS)
	rm *.elf

