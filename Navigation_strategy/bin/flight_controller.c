// AR Drone Flight Controller

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

// HEADERS

#include <SDL.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


// DEFINES

// ADRESSE IP AR.DRONE
#define ADRESSEIP "127.0.0.1"
#define PORT_AT 5556
#define BUFLEN 256 // Taille des paquets UDP
#define TAILLE_MESSAGE 65 // Taille maximale commande AT

// CONFIGURATION DU DRONE
#define DELAI_MICROSECONDES 45000 // Délai entre deux commandes AT (microsecondes)
#define ANIMATION_TURN_AROUND "\"control:flight_anim\",\"6,5000\""
#define ANIMATION_LOOPING_AVANT "\"control:flight_anim\",\"16,15\""
#define ANIMATION_LOOPING_ARRIERE "\"control:flight_anim\",\"17,15\""
#define ANIMATION_LOOPING_GAUCHE "\"control:flight_anim\",\"18,15\""
#define ANIMATION_LOOPING_DROITE "\"control:flight_anim\",\"19,15\""
#define MODE_DE_VOL_NORMAL "\"control:flying_mode\",\"0\""
#define MODE_DE_VOL_AUTONOME "\"control:flying_mode\",\"1\""
#define TYPE_DE_CIBLE "\"detect:detect_type\",\"12\""

// MODE "NERVOSITÉ"
#define NERVOSITE_ALTITUDE_MAXIMALE "\"control:altitude_max\",\"15000\"" // Altitude maximale (millimètres)
#define NERVOSITE_ALTITUDE_MINIMALE "\"control:altitude_min\",\"500\"" // Altitude minimale (millimètres)
#define NERVOSITE_VITESSE_VERTICALE_MAXIMALE "\"control:control_vz_max\",\"3500\"" // Vitesse verticale maximale (millimètres par seconde)
#define NERVOSITE_VITESSE_LACET "\"control:control_yaw\",\"8\"" // Vitesse de lacet (radians par seconde)
#define NERVOSITE_ANGLE_MAXIMAL "\"control:euler_angle_max\",\"0.55\"" // Angle de flexion maximal (radians)

// MODE "SÉCURITÉ"
#define SECURITE_ALTITUDE_MAXIMALE "\"control:altitude_max\",\"5000\"" // Altitude maximale (millimètres)
#define SECURITE_ALTITUDE_MINIMALE "\"control:altitude_min\",\"50\"" // Altitude minimale (millimètres)
#define SECURITE_VITESSE_VERTICALE_MAXIMALE "\"control:control_vz_max\",\"800\"" // Vitesse verticale maximale (millimètres par seconde)
#define SECURITE_VITESSE_LACET "\"control:control_yaw\",\"3.0\"" // Vitesse de lacet (radians par seconde)
#define SECURITE_ANGLE_MAXIMAL "\"control:euler_angle_max\",\"0.25\"" // Angle de flexion maximal (radians)

// PROTOTYPES DES FONCTIONS

int envoyer_AT(char* commande,
               unsigned int *compteur,
               char* argument);

int envoyer_AT_PCMD(unsigned int *compteur,
                    float my_floating_point_variable_4,
                    float my_floating_point_variable_3,
                    float my_floating_point_variable_1,
                    float my_floating_point_variable_2);

// VARIABLES GLOBALES

struct sockaddr_in serv_addr;
int sockfd, slen;
int status = 0; // status = 0 : Le drone est au sol - status = 1 : Le drone est en vol
int mode = 1; // mode = 0 : Nervosité - mode = 1 : Sécurité
int urgence = 0; //urgence = 0 : anti arret urgence - urgence = 1 : Arret d'urgence
int flying_mode = 0; // flying_mode = 0 : Mode normal - flying_mode = 1 "hover over oriented roundel mode"
unsigned int compteur = 0; // Initialisation du compteur

//********************************
// FONCTION PRINCIPALE
//********************************

//Activate safety mode
void on_safety_mode(void) {
    envoyer_AT("AT*CONFIG",&compteur,SECURITE_ALTITUDE_MAXIMALE);
    envoyer_AT("AT*CONFIG",&compteur,SECURITE_ALTITUDE_MINIMALE);
    envoyer_AT("AT*CONFIG",&compteur,SECURITE_VITESSE_VERTICALE_MAXIMALE);
    envoyer_AT("AT*CONFIG",&compteur,SECURITE_VITESSE_LACET);
    envoyer_AT("AT*CONFIG",&compteur,SECURITE_ANGLE_MAXIMAL);
    
    mode = 1;
}
//Send take off command
void on_take_off(void) {
    if (status == 0) {
        envoyer_AT("AT*REF",&compteur,"290718208"); // Décollage du drone
        status = 1;
    }
}

//Send landing command
void on_land(void) {
    if (status == 1) {
        envoyer_AT("AT*REF",&compteur,"290717696"); // Atterissage du drone
        status = 0;
    }
}

//Send urgent stop command
void on_urgent_stop(void) {
    envoyer_AT("AT*REF",&compteur,"290717952");
    status = 0;
    urgence = 1;
}

//Send anti urgent stop command
void on_anti_urgent_stop(void) {
    envoyer_AT("AT*REF",&compteur,"290717696");
    urgence = 0;
}

//Send anti urgent stop command
void on_stationary_flight(void) {
    envoyer_AT("AT*PCMD",&compteur,"0,0,0,0,0\r");
    urgence = 0;
}

//Send turn around command
void on_front_looping(void) {
    envoyer_AT("AT*CONFIG",&compteur,ANIMATION_TURN_AROUND);
}

//Send front looping command
void on_front_looping(void) {
    envoyer_AT("AT*CONFIG",&compteur,ANIMATION_LOOPING_AVANT);
}

//Send back looping command
void on_back_looping(void) {
    envoyer_AT("AT*CONFIG",&compteur,ANIMATION_LOOPING_ARRIERE);
}

//Send left looping command
void on_left_looping(void) {
    envoyer_AT("AT*CONFIG",&compteur,ANIMATION_LOOPING_GAUCHE);
}

//Send right looping command
void on_right_looping(void) {
    envoyer_AT("AT*CONFIG",&compteur,ANIMATION_LOOPING_DROITE);
}

//Activate normal flight mode
void on_normal_flight_mode()
{
    envoyer_AT("AT*CONFIG",&compteur,MODE_DE_VOL_NORMAL);
}

//Activate autonomous flight mode
void on_autonomous_flight_mode()
{
    envoyer_AT("AT*CONFIG",&compteur,MODE_DE_VOL_AUTONOME);
    envoyer_AT("AT*CONFIG",&compteur,TYPE_DE_CIBLE);
}

//********************************
//NETWORK
//********************************

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

//    close(sockfd);

//********************************
// DÉFINITION DES FONCTIONS
//********************************

int envoyer_AT_PCMD(unsigned int *compteur,
                    float my_floating_point_variable_4,
                    float my_floating_point_variable_3,
                    float my_floating_point_variable_1,
                    float my_floating_point_variable_2)
{
    char message[TAILLE_MESSAGE]; // Commande AT à envoyer
    *compteur=*compteur+1; // Incrémentation du compteur
    
    printf("INT - GAUCHE - AXE HORIZONTAL : %f\n",my_floating_point_variable_4);
    printf("INT - GAUCHE - AXE VERTICAL : %f\n",my_floating_point_variable_3);
    printf("INT - DROITE - AXE HORIZONTAL : %f\n",my_floating_point_variable_1);
    printf("INT - DROITE - AXE VERTICAL : %f\n",my_floating_point_variable_2);
    
        return 0; // On sort de la fonction
        
         sprintf(message,"AT*PCMD=%d,1,%d,%d,%d,%d\r",
                *compteur,
                *(int*)(&my_floating_point_variable_1),
                *(int*)(&my_floating_point_variable_2),
                *(int*)(&my_floating_point_variable_3),
                *(int*)(&my_floating_point_variable_4));
    }
    
    if (sendto(sockfd, message, BUFLEN, 0, (struct sockaddr*)&serv_addr, slen) == -1) {
        perror("sendto() ");
    }
    usleep(DELAI_MICROSECONDES);
    
    return 1;
}

int envoyer_AT(char* commande,
               unsigned int *compteur,
               char *argument)
{
    char message[TAILLE_MESSAGE]; // Commande AT à envoyer
    *compteur=*compteur+1; // Incrémentation du compteur
    sprintf(message,"%s=%d,%s\r",commande,*compteur,argument);
    
#ifdef DEBUG_PRINTF
    printf("%s\n",message);
#endif
    
    if (sendto(sockfd, message, BUFLEN, 0, (struct sockaddr*)&serv_addr, slen)== -1) {
        
        perror("sendto() ");
        
        return 0; // ÉCHEC
    }
    
    usleep(DELAI_MICROSECONDES);
    
    return 1; // SUCCÈS
}