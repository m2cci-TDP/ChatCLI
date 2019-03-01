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

#define SERVICE_DEFAUT "65432"
#define SERVEUR_DEFAUT "127.0.0.1"
#define BUFFERSIZE 1024

void client_appli (char *serveur, char *service);

void getString (char message[]) {
	strcpy(message, "");
	char c;
	if ((c = getchar()) != '\n' && c != EOF) {
		strcpy(message, &c);
	}
	while ((c = getchar()) != '\n' && c != EOF)
	{
		strcat(message, &c);
	}
}
void sendMessage(char *message){
#if defined (__APPLE__) && defined (__MACH__)
	gets(message);
#else
	getString(message);
#endif
}


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
/*****************************************************************************/
/* procedure correspondant au traitement du client de votre application */


void client_appli (char *serveur,char *service)
{
	char tamponLecture [BUFFERSIZE+1];
	char message [BUFFERSIZE+1];
	char tamponEcriture [BUFFERSIZE+1];
	int nb_octets;
	struct sockaddr_in *socket_target;
	
	int num_soc;
	int stop=1;
	int envoi=1;
	/*printf("choisissez le mode SOCK_STREAM (tcp) ou SOCK_DGRAM (udp)");
	scanf(%d,typesock);*/
	
	num_soc = h_socket(AF_INET,SOCK_STREAM);
	
	adr_socket (service, serveur, SOCK_STREAM,&socket_target);
	
	h_connect(num_soc,socket_target);
	
	printf("connexion reussie\n");
	
	while (stop !=0 || envoi!=0){
		printf("J'écris au serveur\n");
		/* Sous Mac utiliser la fonction gets();*/
		//gets(message);
		/* sous Linux utiliser getString();*/
		//getString(message);
		sendMessage(message);
		printf("message = %s\n",message);
		sprintf(tamponEcriture, "%s", message);
		envoi=h_writes(num_soc,tamponEcriture,BUFFERSIZE);
		printf("Je lis ce que le serveur me dit\n");
		stop=h_reads(num_soc,tamponLecture,BUFFERSIZE);
		printf("%s\n",tamponLecture);
	}
	printf("je suis sorti de la boucle\n");
	h_close(num_soc);

	/* a completer .....  */

}

/*****************************************************************************/
