#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "navdata.h"

#ifndef NAVDATA_IP
#define NAVDATA_IP "127.0.0.1"
#endif

#ifndef NAVDATA_PORT
#define NAVDATA_PORT 5554
#endif

static int navdata_sock;
static struct sockaddr_in navdata_addr;

int navdata_init()
{
	// Create UDP socket
	navdata_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (navdata_sock == -1) {
		perror("socket");
		goto err;
	}
	
	// Set source address to *:35666
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(35666);
    if (bind(navdata_sock, (struct sockaddr *) &addr, sizeof(addr))) {
    	perror("bind");
    	goto err;
	}
	
	// Save destination address to 127.0.0.1:5554
	memset(&navdata_addr, 0, sizeof(navdata_addr));
    navdata_addr.sin_family = AF_INET;
    if (! inet_aton(NAVDATA_IP, &(navdata_addr.sin_addr))) {
    	goto err;
    }
    navdata_addr.sin_port = htons(NAVDATA_PORT);
    
	return 0;
	
err:
	close(navdata_sock);
	return 1;
}

static int navdata_send(unsigned char const * msg)
{
	ssize_t n = sendto(navdata_sock, msg, strlen(msg), 0,
		(struct sockaddr *) &navdata_addr, sizeof(navdata_addr));
    if (n < 0) {
        perror("sendto");
        return 1;
    }
    return 0;
}

int navdata_start()
{
	// Send start flag
	return navdata_send("\x01\x00");
}

static int navdata_recv(unsigned char * buffer, size_t size)
{
	memset(buffer, 0, size);
	ssize_t n = recvfrom(navdata_sock, buffer, size, 0, NULL, NULL);
	if (n < 0) {
		perror("recvfrom");
	}
	return n;
}

int navdata_get(navdata_t * data)
{
	// Read from socket
	unsigned char buffer[4096];
	unsigned char * bufptr = buffer;
	int n = navdata_recv(buffer, sizeof(buffer));
	if (n < 18) {
		return 1;
	}
	// Parse buffer to fill navdata struct
	if (((uint32_t *)buffer)[0] != 0x55667788) {
		return 1;
	}
	memcpy(&(data->header), bufptr, sizeof(data->header));
	bufptr += sizeof(data->header);
	struct navdata_option option;
	memcpy(&option, bufptr, sizeof(option));
	if (option.tag == 0) {
		memcpy(&(data->demo), bufptr, sizeof(data->demo));
		bufptr += sizeof(data->demo);
	}
	return 0;
}

void navdata_print(FILE * stream, navdata_t const * data)
{
	fprintf(stream, "State: %08X\tSeq: %u\tVision: %u\n", data->header.state,
		data->header.seq, data->header.vision);
	if (data->demo.header.tag == 0) {
		fprintf(stream, "Demo\n");
		fprintf(stream, "    Battery: %u%%\tPitch: %f\tRoll: %f\tYaw: %f\n",
			data->demo.battery, data->demo.theta, data->demo.phi, data->demo.psi);
		fprintf(stream, "    Altitude: %d\tVx: %f\tVy: %f\tVz: %f\n",
			data->demo.altitude, data->demo.vx, data->demo.vy, data->demo.vz);
	}
}

