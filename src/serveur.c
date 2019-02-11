#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <string.h>
#include "util.h"
#include "serveur.h"
#include "fon.h"




void serverTCP (char *service) {
	printf("Running chat as server on port: %s\n", service);
	/* SOCK_STREAM = TCP */
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
	printf("Entrez [%s] pour arrêter Le processus.\n",EXIT_CHAR);
	do {
		/* boucle d'arrêt */
		stop = getchar();
		viderBuffer();
	}	while (isFlag(&stop, EXIT_CHAR));
	kill(p, SIGUSR1); /* kill child process, need sudo if SIGKILL */
	h_close(numSocket); /* fermeture de la socket en attente */
	printf("FIN SERVEUR TCP DE CHAT\n");
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
      while (!isFlag(bufferReception, EXIT_CHAR)) {
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
