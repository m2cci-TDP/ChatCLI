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

#include "fon.h"     		/* Primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"

void serveur_appli (char *service);   /* programme serveur */


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


/******************************************************************************/
/* Procedure correspondant au traitemnt du serveur de votre application */
void serveur_appli(char *service)
{
	char tamponLecture [BUFFERSIZE+1];
	char tamponEcriture [BUFFERSIZE+1];
	int nb_octets;
	struct sockaddr_in *socket_target;
	
	int num_soc;
	int stop=1;
	int envoi=1;
	/*printf("choisissez le mode SOCK_STREAM (tcp) ou SOCK_DGRAM (udp)");
	scanf(%d,typesock);*/
	printf("test0\n");
	num_soc = h_socket(AF_INET,SOCK_STREAM);
	printf("test1\n");
	adr_socket (service, serveur, SOCK_STREAM,&socket_target);
	printf("test2\n");	

}

/******************************************************************************/
