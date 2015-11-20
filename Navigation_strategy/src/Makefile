CC = arm-linux-gnueabi-gcc
CFLAGS = -march=armv7-a
CXXFLAGS = -W -Wall
LDFLAGS =
EXEC = clean sheldon_run.elf

SRCS = at_commands_builder.c flight_functions.c UDP_sender.c test.c
OBJS = at_commands_builder.o flight_functions.o UDP_sender.o test.o

all : $(EXEC)
	
sheldon_run.elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o sheldon_run.elf

at_commands_builder.o: at_commands_builder.c
	$(CC) -o at_commands_builder.o -c at_commands_builder.c $(CXXFLAGS)

flight_functions.o: flight_functions.c
	$(CC) -o flight_functions.o -c flight_functions.c $(CXXFLAGS)

UDP_sender.o: UDP_sender.c
	$(CC) -o UDP_sender.o -c UDP_sender.c $(CXXFLAGS)

test.o: test.c
	$(CC) -o test.o -c test.c $(CXXFLAGS)
clean:
	rm -rf *.o
	rm -rf ~.o
	rm -rf *.elf
	rm -rf ~.elf
