/******************************************************************************/
/*			Application: ....			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  ....						      */
/*		Date :  ....						      */
/*									      */
/******************************************************************************/

#include <stdio.h>
#include <curses.h>

#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#include "fon.h"     		/* Primitives de la boite a outils */
#include "util.h"

void serveur_appli (char *service);   /* programme serveur */
void serveur_udp (char *service);
void serveur_tcp (char *service);

/******************************************************************************/
/*---------------- programme serveur ------------------------------*/

int main(int argc,char *argv[])
{
	char *service = SERVICE_DEFAUT; /* numero de service par defaut */

	/* Permet de passer un nombre de parametre variable a l'executable */
	cli(argc, argv, &service, NULL);

	/* service est le service (ou numero de port) auquel sera affecte ce serveur*/
	serveur_appli(service);
	return 0;
}


/******************************************************************************/
/* Procedure correspondant au traitemnt du serveur de votre application */
void serveur_appli(char *service)
{
	if (isFlag(service, "tcp")) {
		serveur_tcp(service);
	} else {
		serveur_udp(service);
	}
}

void serveur_udp (char *service) {
	printf("SERVEUR UDP\n");
	/* SOCK_DGRAM = UDP */
	int numSocket = h_socket(AF_INET, SOCK_DGRAM); /* création de la socket */
	struct sockaddr_in *p_adr_socket;
	if (isFlag(service, "udp")) {
		service = SERVICE_DEFAUT;
	}
	adr_socket(service, NULL, SOCK_DGRAM, &p_adr_socket); /* création de l'adresse de la socket */
	h_bind(numSocket, p_adr_socket); /* association de la socket et de son adresse */

	/* reception + lecture */
	for (int i = 0; i < 10; i++) {
		struct sockaddr_in p_adr_distant; /* adresse de la machine distante pour une réponse */
		int nbOctRecus = h_recvfrom(numSocket, bufferReception, BUFFER_SIZE, &p_adr_distant);
		if (nbOctRecus == -1) {
			fprintf(stderr, "Erreur lors de la réception de la socket.\n");
		} else {
			#ifdef DEBUG
			printf("Nombres d'octets reçus : %d\n", nbOctRecus);
			#endif
			printf("%s\n", bufferReception);
		}
	}

	h_close(numSocket); /* fermeture */
}

void serveur_tcp (char *service) {
	printf("SERVEUR TCP\n");
	/* SOCK_STREAM = UDP */
	int numSocket = h_socket(AF_INET, SOCK_STREAM); /* création de la socket */
	struct sockaddr_in *p_adr_serveur;
	if (isFlag(service, "tcp")) {
		service = SERVICE_DEFAUT;
	}
	adr_socket(service, NULL, SOCK_STREAM, &p_adr_serveur); /* création de l'adresse de la socket */
	h_bind(numSocket, p_adr_serveur); /* association de la socket et de son adresse */
	h_listen(numSocket, NB_CON);

	pid_t p = fork();
	if (p < 0)
	{
		fprintf(stderr, "Erreur lors de la connection.\n");
	}
	else if (p != 0)
	{
		/* p = pid du fils
		code du père */
		int status;
		waitpid(p, &status, 0);
		h_close(numSocket);
		#ifdef DEBUG
		fprintf(stderr, "Connection terminée.\n");
		#endif
	} else {
		/* code du fils */
		struct sockaddr_in p_adr_client;
		int numSocketClient = h_accept(numSocket, &p_adr_client);

		int nbOctRecus = h_reads(numSocketClient, bufferReception, BUFFER_SIZE);
		if (nbOctRecus == -1) {
			fprintf(stderr, "Erreur lors de la réception de la socket.\n");
		} else {
			#ifdef DEBUG
			printf("Nombres d'octets reçus : %d\n", nbOctRecus);
			#endif
			printf("%s\n", bufferReception);

			sprintf(bufferEmission, "%s\nBien reçu !", bufferReception);
			h_writes(numSocketClient, bufferEmission, BUFFER_SIZE);
		}

		h_close(numSocketClient);
	}
}

/******************************************************************************/
