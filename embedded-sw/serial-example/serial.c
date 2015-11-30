#include <stdlib.h>
#include <stdio.h>		/* Standard input/output definitions */
#include <string.h>		/* String function definitions */
#include <unistd.h>		/* UNIX standard function definitions */
#include <fcntl.h>		/* File control definitions */
#include <errno.h>		/* Error number definitions */
#include <termios.h>		/* POSIX terminal control definitions */
#include <assert.h>


#ifdef DEBUG
#define debug(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif


void serial_config(int fd)
{
	struct termios options;
	tcgetattr(fd, &options);
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	options.c_cflag |= (CLOCAL | CREAD);  // Enable local mode
	options.c_cflag &= ~PARENB;  // Disable parity
	options.c_cflag &= ~CSTOPB;  // One stop bit
	options.c_cflag &= ~CSIZE;  // Clear data size bits
	options.c_cflag |= CS8;	 // Set 8 bits data size
	options.c_cflag &= ~CRTSCTS;	// Disable hardware flow control
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Set raw input (unbuffered, no echo)
	options.c_iflag &= ~(IXON | IXOFF | IXANY);  // Disable software flow control
	options.c_oflag &= ~OPOST;  // Set raw output
	// Set read() to return when 1 or more characters are received
	options.c_cc[VMIN] = 1;
	options.c_cc[VTIME] = 0;
	tcsetattr(fd, TCSAFLUSH, &options);
}


int serial_init(char * device)
{
	int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		perror("Unable to open device");
		return -1;
	}

	if (! isatty(fd)) {
		fprintf(stderr, "This device is not a serial port!\n");
		close(fd);
		return -1;
	}

	// Set blocking read
	/* On the drone, the read is non-blocking anyway, normally we would
	 * use FNDELAY instead of 0 for that */
	fcntl(fd, F_SETFL, 0);

	serial_config(fd);
	
	return fd;
}


int serial_start(int fd)
{
	int n = write(fd, "S", 1);
	if (n < 0) {
		perror("Write failed");
		return -errno;
	} else {
		printf("<- S\n");
	}
	tcflush(fd, TCIFLUSH);
	for (int i = 0; i < 10; i) {
		char buffer[1];
		int n = read(fd, buffer, sizeof(buffer));
		if (n > 0) {
			printf("-> %c\n", buffer[0]);
			if (buffer[0] == 'S') {
				return 0;
			}
		}
	}
	return 1;
}


void printhex(char const * buf, size_t size)
{
	for (int i = 0; i < size; i++) {
		printf("%02X ", buf[i]);
	}
}


int parse(char * buffer, size_t * nbytes, unsigned int * data)
{
	char const start = '\xFF';
	int discard = -1;  // buffer[0..discard] will be deleted
	
	enum { START1, START2, MSB0, LSB, MSB };
	char * state_to_str[] = { "START1", "START2", "MSB0", "LSB", "MSB" };
	int state = START1;
	unsigned int datatmp[8];
	unsigned int ndata = 0;
	int updated = 0;

	for (unsigned int i = 0; i < *nbytes; i++) {
		debug("state = %s, i = %u, char = 0x%02x, ndata = %u, discard = %d\n",
			state_to_str[state], i, buffer[i], ndata, discard);
		switch (state) {
		case START1:
			if (buffer[i] == start) {
				state = START2;
			} else {
				discard = i;
			}
			break;
		case START2:
			if (buffer[i] == start) {
				state = MSB0;
			} else {
				state = START1;
				discard = i;
			}
			break;
		case MSB0:
			ndata = 0;
			if (buffer[i] == start) {
				// Allow any number of start
				discard = i - 1;
			} else {
				datatmp[ndata] = (unsigned int)(buffer[i]) << 8;
				state = LSB;
			}
			break;
		case LSB:
			datatmp[ndata] |= (unsigned int)(buffer[i]);
			ndata++;
			if (ndata == 8) {
				memcpy(data, datatmp, sizeof(datatmp));
				updated = 1;
				discard = i;
				state = START1;
			} else {
				state = MSB;
			}
			break;
		case MSB:
			if (buffer[i] == start) {
				discard = i - 1;
				state = START2;
			} else {
				datatmp[ndata] = (unsigned int)(buffer[i]) << 8;
				state = LSB;
			}
			break;
		default:
			state = START1;
			break;
		}
	}
	debug("exit state = %s, ndata = %u, discard = %d\n",
			state_to_str[state], ndata, discard);
	if (discard >= 0) {
		memmove(buffer, buffer + discard + 1, *nbytes - discard);
		*nbytes -= discard + 1;
	}
	return updated;
}

int main(int argc, char * argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	int fd = serial_init(argv[1]);
	if (fd == -1) {
		exit(1);
	}
/*
	if (serial_start(fd)) {
		exit(1);
	} else {
		printf("OK\n");
	}
*/
	char buffer[18];
	size_t nbytes = 0;
	unsigned int data[8];

	for (;;) {
		debug("buffer = %p, nbytes = %d, read %d\n", buffer, nbytes, sizeof(buffer) - nbytes);
		assert (nbytes <= sizeof(buffer));
		int n = read(fd, buffer + nbytes, sizeof(buffer) - nbytes);
		//printf("n = %d\n", n);
		if (n < 0) {
			perror("Read failed");
			exit(1);
		} else if (n == 0) {
			continue;
		}
		nbytes += n;
		//printf("-> "); printhex(buffer, nbytes); printf("\n");
		if (parse(buffer, &nbytes, data)) {
			printf("data = %04x %04x %04x %04x %04x %04x %04x %04x\n",
				data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
		}
	}

	close(fd);
	return 0;
}
