#include "UDP_sender.h"

static struct sockaddr_in serv_addr;

//Initialise a socket
int init_socket(){
    
    int val_out = 0;
    slen=sizeof(serv_addr);
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Socket ");
        val_out=1;
    }
    
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_AT);
    
    if (inet_aton(ADRESSEIP, &serv_addr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() : ERREUR\n");
        val_out=2;
    }
    
    return val_out;
}

//Close a socket
//close(sockfd);

//Send a message
//if wait != 0, wait after sending the message
//if wait = 0, don't wait
int send_message(char *message, int wait)
{
    int val_out = 0;

    if (sendto(sockfd, message, BUFLEN, 0, (struct sockaddr*)&serv_addr, slen) == -1)
    {
        fprintf(stderr, "[%s:%d] Error: sendto() failed\n", __FILE__, __LINE__);
        val_out=1;
    }
    else if (wait)
	usleep(DELAI_MICROSECONDES);

    return val_out;
}

