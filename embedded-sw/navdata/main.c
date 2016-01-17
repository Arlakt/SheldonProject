#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ATCMD_IP "192.168.1.1"
#define NAVDATA_IP "192.168.1.1"

#include "navdata.c"

void printhex8(unsigned char const * buf, size_t size)
{
	for (unsigned int i = 0; i < size; i++) {
		printf("%02X ", buf[i]);
	}
}

void printhex32(unsigned int const * buf, size_t size)
{
	for (unsigned int i = 0; i < size; i++) {
		printf("%08X    ", buf[i]);
	}
}

void debug(unsigned char * buf, size_t size)
{
	printf("%u >\t", size);
	printhex8(buf, size);
	printf("\n    \t");
	printhex32((unsigned int *)buf, size*sizeof(char)/sizeof(int));
	printf("\n");
}

int main()
{
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_aton(ATCMD_IP, &addr.sin_addr);
    addr.sin_port = htons(5556);

	navdata_init();
	navdata_start();
	int n;
	unsigned char buffer[64];
	n = navdata_recv(buffer, sizeof(buffer));
	debug(buffer, n);
	
	//exit(0);
	
	if (*(uint32_t *)(buffer + 4) & ARDRONE_NAVDATA_BOOTSTRAP) {
		printf("Enabling navdata demo mode...\n");
		sendto(sock, "AT*CONFIG=1,\"general:navdata_demo\",\"TRUE\"\r", 42, 0,
			(struct sockaddr *) &addr, sizeof(addr));
	}
	
	n = navdata_recv(buffer, sizeof(buffer));
	debug(buffer, n);
	
	sendto(sock, "AT*CTRL=2,0\r", 42, 0,
		(struct sockaddr *) &addr, sizeof(addr));
		
	n = navdata_recv(buffer, sizeof(buffer));
	debug(buffer, n);
		
	exit(0);
	
	for (;;) {
		int n = navdata_recv(buffer, sizeof(buffer));
		if (n > 0) {
			debug(buffer, n);
		}
	}
}

