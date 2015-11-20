#include <stdio.h>		/* Standard input/output definitions */
#include <string.h>		/* String function definitions */
#include <unistd.h>		/* UNIX standard function definitions */
#include <fcntl.h>		/* File control definitions */
#include <errno.h>		/* Error number definitions */
#include <termios.h>		/* POSIX terminal control definitions */


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

	fcntl(fd, F_SETFL, 0);

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
			fprintf(stderr, "EOF\n");
			break;
		} else {
			buffer[n] = '\0';
			printf("%s", buffer);
		}
	}

	close(fd);
	return 0;
}
