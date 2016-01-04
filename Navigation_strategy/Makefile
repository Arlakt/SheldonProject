CC = arm-linux-gnueabi-gcc -pthread
CFLAGS = -W -Wall -march=armv7-a -std=gnu99
CXXFLAGS = -W -Wall -march=armv7-a -std=gnu99
LDFLAGS =
SRCDIR= ./src
TESTPOSDIR= ./src/test_position
BUILDDIR= ./build
MOVEDIR= $(SRCDIR)/movement
POSIDIR= $(SRCDIR)/position
EXEC = sheldon_position.elf

SRCS = $(MOVEDIR)/at_commands_builder.c $(MOVEDIR)/flight_functions.c $(MOVEDIR)/UDP_sender.c $(POSIDIR)/find_position.c $(POSIDIR)/track_position.c $(TESTPOSDIR)/test_pos.c

OBJS = $(BUILDDIR)/at_commands_builder.o $(BUILDDIR)/flight_functions.o $(BUILDDIR)/UDP_sender.o $(BUILDDIR)/find_position.o $(BUILDDIR)/track_position.o  $(TESTPOSDIR)/test_pos.o

all : clean $(EXEC)
	
sheldon_position.elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BUILDDIR)/sheldon_position.elf

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

$(BUILDDIR)/test_pos.o: $(TESTPOSDIR)/test_pos.c
	$(CC) -o $(BUILDDIR)/test_pos.o -c $(TESTPOSDIR)/test_pos.c $(CXXFLAGS)

clean:
	rm -rf $(BUILDDIR)/*.o
	rm -rf $(BUILDDIR)/~.o
	rm -rf $(BUILDDIR)/*.elf
	rm -rf $(BUILDDIR)/~.elf
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)