#include <stdlib.h>
#include <stdio.h>

#include "serial.h"

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

	unsigned int data[8];

	for (;;) {
		if (serial_get_data(fd, data)) {
			printf("%5u %5u %5u %5u %5u %5u %5u %5u\n",
				data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
		}
	}

	serial_stop(fd);
	return 0;
}

