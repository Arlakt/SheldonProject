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


int main(int argc, char * argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		return 1;
	}

	int fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		perror("Unable to open device");
		return 1;
	}

	fcntl(fd, F_SETFL, FNDELAY);  // Set non-blocking read

	//serial_config(fd);

	int n = write(fd, "S", 1);
	if (n < 0) {
		perror("Write failed");
		return 1;
	}

	for (;;) {
		char buffer[32];
		int n = read(fd, buffer, sizeof(buffer) - 1);
		if (n < 0) {
			perror("Read failed");
			return 1;
		} else if (n == 0) {
			//fprintf(stderr, "EOF\n");
			//break;
		} else {
			buffer[n] = '\0';
			printf("%s", buffer);
		}
	}

	close(fd);
	return 0;
}
