CC = arm-linux-gnueabi-gcc
CFLAGS = -march=armv7-a
CXXFLAGS = -W -Wall
LDFLAGS =
SRCDIR= ./src
TESTDIR= ./src/test_movement
BUILDDIR= ./build
MOVEDIR= $(SRCDIR)/movement
EXEC = clean sheldon_run.elf

SRCS = $(MOVEDIR)/at_commands_builder.c $(MOVEDIR)/flight_functions.c $(MOVEDIR)/UDP_sender.c $(TESTDIR)/test.c
OBJS = $(BUILDDIR)/at_commands_builder.o $(BUILDDIR)/flight_functions.o $(BUILDDIR)/UDP_sender.o $(BUILDDIR)/test.o

all : $(EXEC)
	
sheldon_run.elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BUILDDIR)/sheldon_run.elf

$(BUILDDIR)/at_commands_builder.o: $(MOVEDIR)/at_commands_builder.c
	$(CC) -o $(BUILDDIR)/at_commands_builder.o -c $(MOVEDIR)/at_commands_builder.c $(CXXFLAGS)

$(BUILDDIR)/flight_functions.o: $(MOVEDIR)/flight_functions.c
	$(CC) -o $(BUILDDIR)/flight_functions.o -c $(MOVEDIR)/flight_functions.c $(CXXFLAGS)

$(BUILDDIR)/UDP_sender.o: $(MOVEDIR)/UDP_sender.c
	$(CC) -o $(BUILDDIR)/UDP_sender.o -c $(MOVEDIR)/UDP_sender.c $(CXXFLAGS)

$(BUILDDIR)/test.o: $(TESTDIR)/test.c
	$(CC) -o $(BUILDDIR)/test.o -c $(TESTDIR)/test.c $(CXXFLAGS)
clean:
	rm -rf $(BUILDDIR)/*.o
	rm -rf $(BUILDDIR)/~.o
	rm -rf $(BUILDDIR)/*.elf
	rm -rf $(BUILDDIR)/~.elf
