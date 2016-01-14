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
	struct sockaddr_in addr;
	socklen_t len;
	ssize_t n = recvfrom(navdata_sock, buffer, size, 0,
		(struct sockaddr *) &addr, &len);
	if (n < 0) {
		perror("recvfrom");
	}
	return n;
}

int navdata_get(navdata_t * data)
{
	// Read from socket
	unsigned char buffer[1024];
	int n = navdata_recv(buffer, sizeof(buffer));
	// Parse buffer to fill navdata struct
	// TODO
}

