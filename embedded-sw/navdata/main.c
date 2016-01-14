#include <stdlib.h>
#include <stdio.h>

#include "navdata.c"

void printhex(unsigned char const * buf, size_t size)
{
	for (unsigned int i = 0; i < size; i++) {
		printf("%02X ", buf[i]);
	}
}

int main()
{
	navdata_init();
	navdata_start();
	
	for (;;) {
		unsigned char buffer[16];
		if (navdata_recv(buffer, sizeof(buffer)) > 0) {
			printhex(buffer, sizeof(buffer));
		}
	}
}

