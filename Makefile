CC = arm-linux-gnueabi-gcc -pthread
CFLAGS = -W -Wall -march=armv7-a -std=gnu99
CXXFLAGS = -W -Wall -march=armv7-a -std=gnu99
LDFLAGS =
NAVDIR= ./Navigation_strategy/src
TESTPOSDIR= ./Navigation_strategy/src/test_position
TESTMOVDIR= ./Navigation_strategy/src/test_movement
ESWDIR= ./embedded-sw
BUILDDIR= ./build
MOVEDIR= $(NAVDIR)/movement
POSIDIR= $(NAVDIR)/position
EXEC = clean sheldon_position.elf sheldon_move.elf

SRCS = $(MOVEDIR)/at_commands_builder.c $(MOVEDIR)/flight_functions.c $(MOVEDIR)/UDP_sender.c $(POSIDIR)/find_position.c $(POSIDIR)/track_position.c $(ESWDIR)/serial.c $(TESTPOSDIR)/test_pos.c $(TESTMOVDIR)/test_mov.c

OBJS = $(BUILDDIR)/at_commands_builder.o $(BUILDDIR)/flight_functions.o $(BUILDDIR)/UDP_sender.o $(BUILDDIR)/find_position.o $(BUILDDIR)/track_position.o $(BUILDDIR)/serial.o

all : $(EXEC)
	
sheldon_position.elf: $(OBJS) $(BUILDDIR)/test_pos.o
	$(CC) $(CFLAGS) $(OBJS) $(BUILDDIR)/test_pos.o -o $(BUILDDIR)/sheldon_position.elf

sheldon_move.elf: $(OBJS) $(BUILDDIR)/test_mov.o
	$(CC) $(CFLAGS) $(OBJS) $(BUILDDIR)/test_mov.o -o $(BUILDDIR)/sheldon_move.elf

$(BUILDDIR)/at_commands_builder.o: $(MOVEDIR)/at_commands_builder.c
	$(CC) -o $(BUILDDIR)/at_commands_builder.o -c $(MOVEDIR)/at_commands_builder.c $(CXXFLAGS)

$(BUILDDIR)/flight_functions.o: $(MOVEDIR)/flight_functions.c
	$(CC) -o $(BUILDDIR)/flight_functions.o -c $(MOVEDIR)/flight_functions.c $(CXXFLAGS)

$(BUILDDIR)/UDP_sender.o: $(MOVEDIR)/UDP_sender.c
	$(CC) -o $(BUILDDIR)/UDP_sender.o -c $(MOVEDIR)/UDP_sender.c $(CXXFLAGS)

$(BUILDDIR)/find_position.o: $(POSIDIR)/find_position.c
	$(CC) -o $(BUILDDIR)/find_position.o -c $(POSIDIR)/find_position.c $(CXXFLAGS)

$(BUILDDIR)/track_position.o: $(POSIDIR)/track_position.c
	$(CC) -o $(BUILDDIR)/track_position.o -c $(POSIDIR)/track_position.c $(CXXFLAGS)
	
$(BUILDDIR)/serial.o: $(ESWDIR)/serial.c
	$(CC) -o $(BUILDDIR)/serial.o -c $(ESWDIR)/serial.c $(CXXFLAGS)

$(BUILDDIR)/test_pos.o: $(TESTPOSDIR)/test_pos.c
	$(CC) -o $(BUILDDIR)/test_pos.o -c $(TESTPOSDIR)/test_pos.c $(CXXFLAGS)

$(BUILDDIR)/test_mov.o: $(TESTMOVDIR)/test_mov.c
	$(CC) -o $(BUILDDIR)/test_mov.o -c $(TESTMOVDIR)/test_mov.c $(CXXFLAGS)
clean:
	rm -rf $(BUILDDIR)/*.o
	rm -rf $(BUILDDIR)/~.o
	rm -rf $(BUILDDIR)/*.elf
	rm -rf $(BUILDDIR)/~.elf