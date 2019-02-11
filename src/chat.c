#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include "fon.h"
#include "chat.h"
#include "util.h"

int main (int argc,char *argv[]) {
  	char *serveur = SERVEUR_DEFAUT; /* serveur par defaut */
  	char *service = SERVICE_DEFAUT; /* numero de service par defaut (no de port) */

    if (cli(argc, argv, &service, &serveur)) {
      clientTCP(service, serveur);
    } else {
      serverTCP(service);
    }

    return 0;
}

void serverTCP (char *service) {
	printf("SERVEUR TCP DE CHAT\n");
	/* SOCK_STREAM = UDP */
	int numSocket = h_socket(AF_INET, SOCK_STREAM); /* création de la socket */
	struct sockaddr_in *p_adr_serveur;
	adr_socket(SERVICE_DEFAUT, NULL, SOCK_STREAM, &p_adr_serveur); /* création de l'adresse de la socket */
	h_bind(numSocket, p_adr_serveur); /* association de la socket et de son adresse */
	h_listen(numSocket, NB_CON);

	/* création du processus serveur */
	pid_t p = fork();
	if (p < 0) {
		fprintf(stderr, "Erreur lors de la création du processus serveur.\n");
	} else if (p == PROCESSUS_FILS) {
		/* fils */
		while (1) {
			serverChat(numSocket);
		}
	}
	/* père */
	char stop;
	printf("Appuyez sur \"q\" pour arrêter.\n");
	do {
		/* boucle d'arrêt */
		stop = getchar();
		viderBuffer();
	}	while (stop != SERVEUR_EXIT_CHAR);
	kill(p, SIGUSR1); /* kill child process, need sudo if SIGKILL */
	h_close(numSocket); /* fermeture de la socket en attente */
	printf("FIN SERVEUR TCP DE CHAT\n");
}

void clientTCP (char *serveur, char *service) {
	int noSocket = h_socket(AF_INET, SOCK_STREAM);
	struct sockaddr_in *socket_target;
	adr_socket(SERVICE_DEFAUT, serveur, SOCK_STREAM, &socket_target);
	h_connect(noSocket, socket_target);
	clientChat(noSocket);
	h_close(noSocket);
}

void throwSocketReceptionError() {
    fprintf(stderr, "Erreur lors de la réception de la socket.\n");
}

void serverChat (int socket) {
  struct sockaddr_in p_adr_client;
  int socketClient;

  if ((socketClient = h_accept(socket, &p_adr_client)) != -1) {
    pid_t p;
    if ((p = fork()) < 0) {
      fprintf(stderr, "Erreur lors de la connexion.\n");
    } else if (p == PROCESSUS_FILS) {
      char clientName[BUFFER_SIZE];
      int nbOctRecus = h_reads(socketClient, bufferReception, BUFFER_SIZE); /* lecture du message pseudo */
      if (nbOctRecus == -1) {
        throwSocketReceptionError();
      } else {
        strcpy(clientName, bufferReception);
      }

      /* chat */
      char ipAddr[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &p_adr_client.sin_addr, ipAddr, INET_ADDRSTRLEN);
      printf("%s (%s) entre dans le chat.\n", clientName, ipAddr);
      while (!isFlag(bufferReception, CLIENT_EXIT_CHAR)) {
        int nbOctRecus = h_reads(socketClient, bufferReception, BUFFER_SIZE); /* lecture du message avant espaces */
        if (nbOctRecus == -1) {
          throwSocketReceptionError();
        } else {
          sprintf(bufferEmission, "%s : %s", clientName, bufferReception);
          h_writes(socketClient, bufferEmission, BUFFER_SIZE);
        }
      }

      printf("%s quitte le chat.\n", clientName);
      h_close(socketClient); /* fermeture de la socket ouverte */
      exit(0);
    }

    /* p = pid du fils code du père */
    h_close(socketClient); /* fermeture de la socket ouverte */
  } else {
    fprintf(stderr, "Nombre de connexions complet.\n");
  }
}

void clientChat (int socket) {
  printf("Bienvenue dans le client de chat !\n");
  printf("Veuillez entrez votre pseudo : ");
  setMessage(bufferEmission);
  h_writes(socket, bufferEmission, BUFFER_SIZE);
  printf("\nVous avez choisi \"%s\" comme nom.\n", bufferEmission);
  printf("Vous pouvez quitter l'application à tout moment en tapant le caractère \".\".\n\n");

  while (!isFlag(bufferEmission, CLIENT_EXIT_CHAR)) {
    printf("Votre message : ");
    setMessage(bufferEmission);
    printf("\033[1A"); // move cursor one ligne up
    printf("\x0d"); // move the cursor in first column
    printf("\033[K"); // erase the ligne
    h_writes(socket, bufferEmission, BUFFER_SIZE);

    /* reception */
    int nbOctRecus = h_reads(socket, bufferReception, BUFFER_SIZE); /* lecture du message avant espaces */
    if (nbOctRecus == -1) {
      fprintf(stderr, "Erreur lors de la réception de la socket.\n");
    } else {
      /* écriture */
      printf("%s\n", bufferReception);
    }
  }
  printf("\nA bientôt !\nMerci d'avoir utiliser le chat !\n");
}
