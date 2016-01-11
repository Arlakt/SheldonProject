#ifndef SENDER_H
#define SENDER_H

#ifndef BASIC
#define BASIC
#include <stdlib.h>
#include <stdio.h>
#endif

#include <sys/types.h>
#include <strings.h>

#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// ADRESSE IP AR.DRONE
#define ADRESSEIP "127.0.0.1"
#define PORT_AT 5556
#define BUFLEN 256 // Taille des paquets UDP
#define TAILLE_MESSAGE 65 // Taille maximale commande AT

#define DELAI_MICROSECONDES 40000 // Délai entre deux commandes AT (microsecondes)

int sockfd, slen;

int init_socket();
//Send a message
//if wait != 0, wait after sending the message
//if wait = 0, don't wait
int send_message(char *message, int wait);

#endif

