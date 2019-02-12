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

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "65432"
#define SERVEUR_DEFAUT "127.0.0.1"

void client_appli (char *serveur, char *service);


/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{

	char *serveur= SERVEUR_DEFAUT; /* serveur par defaut */
	char *service= SERVICE_DEFAUT; /* numero de service par defaut (no de port) */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch(argc)
	{
		case 1 :		/* arguments par defaut */
		printf("serveur par defaut: %s\n",serveur);
		printf("service par defaut: %s\n",service);
		break;
		case 2 :		/* serveur renseigne  */
		serveur=argv[1];
		printf("service par defaut: %s\n",service);
		break;
		case 3 :		/* serveur, service renseignes */
		serveur=argv[1];
		service=argv[2];
		break;
		default:
		printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
		exit(1);
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */

	client_appli(serveur,service);
}
#define BUFFERSIZE 1024
/*****************************************************************************/
/* procedure correspondant au traitement du client de votre application */
void client_appli (char *serveur,char *service)
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
	h_connect(num_soc,socket_target);
	
	printf("connexion reussie\n");
	
	while (stop !=0 || envoi!=0){
		printf("J'écris au serveur\n");
		scanf("%s",tamponEcriture);
		envoi=write(num_soc,tamponEcriture,BUFFERSIZE);
		printf("Je lis ce que le serveur me dit\n");
		printf("tamponLecture : %s\n",tamponLecture);
		printf("num_soc : %d\n",num_soc);
		stop=h_reads(num_soc,tamponLecture,BUFFERSIZE);
		printf("%s\n",tamponLecture);
		if()
		
	}
	printf("je suis sorti de la boucle\n");
	h_close(num_soc);

	/* a completer .....  */

}

/*****************************************************************************/
