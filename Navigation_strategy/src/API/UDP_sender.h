#ifndef SENDER_H
#define SENDER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// ADRESSE IP AR.DRONE
#define ADRESSEIP "127.0.0.1"
#define PORT_AT 5556
#define BUFLEN 256 // Taille des paquets UDP
#define TAILLE_MESSAGE 65 // Taille maximale commande AT

#define DELAI_MICROSECONDES 40000 // Délai entre deux commandes AT (microsecondes)

int sockfd, slen;

int init_socket();
int send_message(char *message);

#endif

