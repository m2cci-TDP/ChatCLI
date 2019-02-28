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
#include <string.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "fon.h"     		/* Primitives de la boite a outils */

#define SERVICE_DEFAUT "65432"
#define SERVEUR_DEFAUT "127.0.0.1"


void serveur_appli (char *service);   /* programme serveur */
void getString (char message[]) {
	strcpy(message, "");
	char c;
	do {
		c=getchar();
		strcat(message, &c);
	}
	while (c != '\n');
}


/******************************************************************************/
/*---------------- programme serveur ------------------------------*/

int main(int argc,char *argv[])
{

	char *service= SERVICE_DEFAUT; /* numero de service par defaut */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
	{
		case 1:
		printf("defaut service = %s\n", service);
		break;
		case 2:
		service=argv[1];
		break;

		default :
		printf("Usage:serveur service (nom ou port) \n");
		exit(1);
	}

	/* service est le service (ou numero de port) auquel sera affecte
	ce serveur*/

	serveur_appli(service);
}

#define BUFFERSIZE 1024
/******************************************************************************/
/* Procedure correspondant au traitemnt du serveur de votre application */
void serveur_appli(char *service)
{
	char tamponLecture [BUFFERSIZE+1];
	char tamponEcriture [BUFFERSIZE+1];
	char message [BUFFERSIZE+1];
	int nb_octets;
	struct sockaddr_in *socket_target;
	struct sockaddr_in *p_adr_client;
	int socketcree;
	
	int num_soc;
	int stop=1;
	int envoi=1;
	/*printf("choisissez le mode SOCK_STREAM (tcp) ou SOCK_DGRAM (udp)");
	scanf(%d,typesock);*/
	num_soc = h_socket(AF_INET,SOCK_STREAM);
	adr_socket (service, NULL, SOCK_STREAM,&socket_target);
	h_bind(num_soc,socket_target);
	h_listen(num_soc,5);
	socketcree=h_accept(num_soc,p_adr_client);
	printf("socketcree = %d",socketcree);
	
	while (stop !=0 || envoi!=0){
		printf("J'écris au client\n");
		getString(message);
		printf("message = %s\n",message);
		strcpy(message, "");
		printf("message = %s\n",message);
		sprintf(tamponEcriture, "%s", message);
		envoi=h_writes(socketcree,tamponEcriture,BUFFERSIZE);
		printf("Je lis ce que le client me dit\n");
		printf("tamponLecture : %s\n",tamponLecture);
		stop=h_reads(socketcree,tamponLecture,BUFFERSIZE);
		printf("%s\n",tamponLecture);
	}
	
	printf("je suis sorti de la boucle\n");
	h_close(num_soc);
}

/******************************************************************************/
