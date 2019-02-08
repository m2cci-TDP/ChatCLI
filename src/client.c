/******************************************************************************/
/*			Application: ...					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : ... 					*/
/*									      */
/******************************************************************************/


#include <stdio.h>
#include <curses.h> 		/* Primitives de gestion d'ecran */
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#include "fon.h"   		/* primitives de la boite a outils */
#include "util.h"

void client_appli (char *serveur, char *service);

/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{
	char *serveur = SERVEUR_DEFAUT; /* serveur par defaut */
	char *service = SERVICE_DEFAUT; /* numero de service par defaut (no de port) */

	/* Permet de passer un nombre de parametre variable a l'executable */
	cli(argc, argv, &service, &serveur);

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */

	client_appli(serveur,service);
	return 0;
}

void send_tcp (char *serveur, char *service)
{
	printf("MODE D'ENVOI : TCP\n");
	int noSocket = h_socket(AF_INET, SOCK_STREAM);
	struct sockaddr_in *socket_target;
	adr_socket(SERVICE_DEFAUT, serveur, SOCK_STREAM, &socket_target);
	sprintf(bufferEmission, "Salut mon loulou, je suis le message TCP %d :)", 1);
	h_connect(noSocket, socket_target);
	//	int octetsLus = h_reads(noSocket, bufferReception, BUFFER_SIZE);
	h_writes(noSocket, bufferEmission, BUFFER_SIZE);

	int nbOctRecus = h_reads(noSocket, bufferReception, BUFFER_SIZE);
	if (nbOctRecus == -1) {
		fprintf(stderr, "Erreur lors de la réception de la socket.\n");
	} else {
		#ifdef DEBUG
		printf("Nombres d'octets reçus : %d\n", nbOctRecus);
		#endif
		printf("%s\n", bufferReception);
	}

	h_close(noSocket);
}

void send_udp (char *serveur, char *service)
{
	printf("MODE D'ENVOI : UDP\n");
	int noSocket = h_socket(AF_INET, SOCK_DGRAM);
	struct sockaddr_in *socket_target;
	adr_socket(SERVICE_DEFAUT, serveur, SOCK_DGRAM, &socket_target);
	for(int i = 0; i < 10; i++) {
		sprintf(bufferEmission, "Salut mon loulou, je suis le message UDP %d :)", i);
		#ifdef DEBUG
		printf("Envoi de %s\n", bufferEmission);
		#endif
		h_sendto(noSocket, bufferEmission, BUFFER_SIZE, socket_target);
	}
	h_close(noSocket);
}
/*****************************************************************************/
/* procedure correspondant au traitement du client de votre application */
void client_appli (char *serveur, char *service)
{
	if (isFlag(service, "tcp")) {
		send_tcp(serveur, service);
	} else {
		send_udp(serveur, service);
	}
}
/*****************************************************************************/
