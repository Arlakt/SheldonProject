#include <stdlib.h>
#include <stdio.h>		/* Standard input/output definitions */
#include <string.h>		/* String function definitions */
#include <unistd.h>		/* UNIX standard function definitions */
#include <fcntl.h>		/* File control definitions */
#include <errno.h>		/* Error number definitions */
#include <termios.h>		/* POSIX terminal control definitions */


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
	tcsetattr(fd, TCSAFLUSH, &options);
}


int serial_init(char * device)
{
	int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		perror("Unable to open device");
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


int main(int argc, char * argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	int fd = serial_init(argv[1]);
	if (! isatty(fd)) {
		fprintf(stderr, "This device is not a serial port!\n");
		exit(1);
	}
	if (serial_start(fd)) {
		exit(1);
	} else {
		printf("OK\n");
	}

	char buffer[18];
	char * bufptr = buffer;

	for (;;) {
		int nmax = sizeof(buffer) + buffer - bufptr;
		//printf("buffer = %p, bufptr = %p, nmax = %d, ", buffer, bufptr, nmax);
		int n = read(fd, bufptr, nmax);
		//printf("n = %d\n", n);
		if (n < 0) {
			perror("Read failed");
			exit(1);
		} else if (n == 0) {
			continue;
		}
		bufptr += n;
		//printf("-> "); printhex(buffer, bufptr - buffer); printf("\n");
		if (bufptr == buffer + sizeof(buffer)) {
			printf("Buffer: "); printhex(buffer, sizeof(buffer)); printf("\n");
			if (*(bufptr-2) == 'F' && *(bufptr-1) == 'F') {
				int data[8];
				data[0] = buffer[0] | buffer[1] << 8;
				data[1] = buffer[2] | buffer[3] << 8;
				data[2] = buffer[4] | buffer[5] << 8;
				data[3] = buffer[6] | buffer[7] << 8;
				data[4] = buffer[8] | buffer[9] << 8;
				data[5] = buffer[10] | buffer[11] << 8;
				data[6] = buffer[12] | buffer[13] << 8;
				data[7] = buffer[14] | buffer[15] << 8;
				printf("-> Data: %04x %04x %04x %04x %04x %04x %04x %04x\n",
					data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
			} else {
				printf("Illegal data!\n");
			}
			bufptr = buffer;
		}
	}

	close(fd);
	return 0;
}
