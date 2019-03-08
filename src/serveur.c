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
#define BUFFERSIZE 1024


void serveur_appli (char *service);   /* programme serveur */


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
/* Aiguilleur, selon le système repéré la saisie des données au clavier n'utilise pas la même fonction */

void scanClavier(char *message){
#if defined (__APPLE__) && defined (__MACH__)
	gets(message);
#else
	getString(message);
#endif
}

/* Fonciton envoi de message, ecrit le message dans le buffer puis envoie avec la primitive h_writes */
void sendMessage (int socket, char message[], char bufferEmission[]) {
	sprintf(bufferEmission, "%s", message);
	h_writes(socket, bufferEmission, BUFFERSIZE);
}
/* Message envoyé au client à sa connexion, présente le jeu et permet de sléectionner un niveau de difficulté*/

void presentationJeu(int socket, char buffer[]){
	sprintf(buffer, "----------------------------------------\n----Bienvenue dans le jeu MasterMind----\n----------------------------------------\nVeuillez choisir votre niveau de difficulté entre 1 et 8 ? \n(1 très facile et 8 très difficile)");
	h_writes(socket, buffer, BUFFERSIZE);
}

/* Récupère le choix de diffuculté envoyé par le client, vérifie la validité de sa réponse et génère un MasterMind */


void modeDeJeu(char* message, int* combinaison, int* etapeSuivante){
	int niveauDeJeu;
	int i = 0;
	int j;
	int a=0;
	int existeDeja;
	int suivant;
	
	
	niveauDeJeu=atoi(message);
	
	if(niveauDeJeu>8 || niveauDeJeu<1){
		sprintf(message, "mode de jeu non valide merci de choisir un chiffre entre 1 et 8");
		*etapeSuivante=0;
	}
	else{
		sprintf(message, "mode de jeu retenu à %d couleurs\nVeuillez rentrer une premiere combinaison\n", niveauDeJeu);
		*etapeSuivante=1;
		srand(time(NULL));
		for (i= 0 ; i < niveauDeJeu ; i++) {
			suivant =0;
			while(suivant!=1){
				a = rand()%(niveauDeJeu-1+1) + 1;
				j=0;
				existeDeja=0;
				while(j<i+1 && existeDeja!=1){
					if(combinaison[j]==a){
						existeDeja=1;
					}
					else {
						j++;
					}		
				}
				if (j==i+1){
					suivant=1;
				}
			}
			combinaison[i]=a;
		}
	}
}
/* Permet de clore la partie, coupe la boucle et permet le h_close*/

void finDePartie(char* message, int* gagne){
	*gagne=1;
	sprintf(message, "Jeu en cours de construction\nPartie Terminée\n");
	
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


/******************************************************************************/
/* Procedure correspondant au traitemnt du serveur de votre application */
void serveur_appli(char *service)
{
	int combinaison [8];
	char tamponLecture [BUFFERSIZE+1];
	char tamponEcriture [BUFFERSIZE+1];
	char message [BUFFERSIZE+1];
	int nb_octets;
	struct sockaddr_in *socket_target;
	struct sockaddr_in *p_adr_client;
	int socketcree;
	int etapeSuivante =0;
	
	int num_soc;
	int stop=1;
	int envoi=1;
	int gagne=0;
	
	num_soc = h_socket(AF_INET,SOCK_STREAM);
	adr_socket (service, NULL, SOCK_STREAM,&socket_target);
	h_bind(num_soc,socket_target);
	h_listen(num_soc,5);
	socketcree=h_accept(num_soc,p_adr_client);
	printf("socketcree = %d\n",socketcree);
	
	while ((stop !=0 || envoi!=0) && gagne!=1){
		presentationJeu(socketcree,tamponEcriture);
		while(etapeSuivante!=1){
			stop=h_reads(socketcree,tamponLecture,BUFFERSIZE);
			strcpy(message,tamponLecture);
			modeDeJeu(message,combinaison,&etapeSuivante);
			sendMessage(socketcree,message,tamponEcriture);
		}
		finDePartie(message,&gagne);
		sendMessage(socketcree,message,tamponEcriture);
		
		/* To do la suite du programme */
	}
	h_close(socketcree);
	
}

/******************************************************************************/


/*
Fonctions pensées mais pas implantées dans le serveur 
void proposerEtEvaluerUneCombinaison() {    // faire une fonction

	int nbBonnePlace = 0 ;
	int nbMauvaisePlace = 0 ;
	int inexistant = 0;

	// proposition du joueur : on lui demande de saisir une valeur "niveauDeJeu fois"

	printf("veuillez saisir votre proposition de combinaison avec les chiffres de 1 à %d",niveauDeJeu);
	int i = 0;
	while (i < niveauDeJeu) {
		printf("choix case N°%d : ", i);
		proposition[i] = scanf("%d", &i);
		i++;
	}



	// comparer la proposition à la combinaison
	// existe-t-il ? => si non inexistant++
	// est-il à la bonne place? OUI nbBonnePlace ++   NON nbMauvaisePlace ++

	int j = 0;
	int k;
	for(j=0 ; j < niveauDeJeu ; j++){
		k = 0;
		if (proposition[j] == combinaison[k]) {
			nbBonnePlace++;
		}
		else
		{
			for (k=0 ; k < niveauDeJeu ; k++){
				if (proposition[j] == combinaison[k]) {
					nbMauvaisePlace++;
				}
			}
		}

		if (nbBonnePlace == niveauDeJeu){
			printf("Bravo ! Vous avez trouvé la bonne combinaison !");
		}
		else
		{
			proposerEtEvaluerUneCombinaison(); // on recommance
		}
	}


}
*/


