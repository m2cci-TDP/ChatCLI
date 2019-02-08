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

#define SERVICE_DEFAUT "1111"
#define SERVEUR_DEFAUT "127.0.0.1"

void client_appli (char *serveur, char *service);


/*****************************************************************************/
/*--------------- programme client -----------------------*/

int isFlag(char* string, char* flag){
	return strcmp(string, flag) == 0;
}

int main(int argc, char *argv[])
{

	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */

	for (int i=1; i<argc; i++){
		if(isFlag(argv[i], "-t") || isFlag(argv[i], "--target")){
			serveur = argv[++i];
		} else if (isFlag(argv[i], "-p") || isFlag(argv[i], "--port")) {
			service = argv[++i];
		} else {
			printf("Flag [%s] not recognized\n", argv[i]);
			printf("Usage: client [OPTIONS]\n");
			printf("-t, --target <IPaddr>\t\tIP address of target\n");
			printf("-p, --port <port>\t\tport of target\n");
			exit(1);
		}
	}

	printf("serveur: %s, port: %s\n", serveur, service);
	/* Permet de passer un nombre de parametre variable a l'executable */
/*	switch(argc)
	{
		case 1 :
		printf("serveur par defaut: %s\n",serveur);
		printf("service par defaut: %s\n",service);
		break;
		case 2 :
		serveur=argv[1];
		printf("service par defaut: %s\n",service);
		break;
		case 3 :
		serveur=argv[1];
		service=argv[2];
		break;
		default:
		printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
		exit(1);
	}
*/
	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */

	client_appli(serveur,service);
}

#define BUFFER_SIZE 2000
char buffer_emission[BUFFER_SIZE+1];
char buffer_reception[BUFFER_SIZE+1];

void send_tcp (char *serveur, char *service)
{
	printf("MODE D'ENVOI: TCP\n");
	int noSocket = h_socket(AF_INET, SOCK_STREAM);
	struct sockaddr_in *socket_target;
	adr_socket(service, serveur, SOCK_STREAM, &socket_target);
	sprintf(buffer_emission, "Salut mon loulou, je suis le message TCP %d :)", 1);
	h_connect(noSocket, socket_target);
//	int octetsLus = h_reads(noSocket, buffer_reception, BUFFER_SIZE);
	h_writes(noSocket, buffer_emission, BUFFER_SIZE);
	h_close(noSocket);
}

void send_udp (char *serveur, char *service)
{
	printf("MODE D'ENVOI: UDP\n");
	int noSocket = h_socket(AF_INET, SOCK_DGRAM);
	struct sockaddr_in *socket_target;
	adr_socket(service, serveur, SOCK_DGRAM, &socket_target);
	for(int i = 0; i < 10; i++) {
		sprintf(buffer_emission, "Salut mon loulou, je suis le message UDP %d :)", i);
		#ifdef DEBUG
		printf("Envoi de %s\n", buffer_emission);
		#endif
		h_sendto(noSocket, buffer_emission, BUFFER_SIZE, socket_target);
	}
	h_close(noSocket);
}
/*****************************************************************************/
/* procedure correspondant au traitement du client de votre application */
void client_appli (char *serveur, char *service)
{
	if (strcmp(service, "tcp") == 0) {
		send_tcp(serveur, service);
	} else {
		send_udp(serveur, service);
	}
}
/*****************************************************************************/
