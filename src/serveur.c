#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <string.h>
#include "util.h"
#include "serveur.h"
#include "fon.h"



void serverTCP (char *port) {
	printf("Running chat as server on port: %s\n", port);

	/* création du processus serveur */
	pid_t listeningSocket_pid;
	int numSocket = 0;
	createListeningSocket(&listeningSocket_pid, &numSocket, port);

	char stop;
	printf("Entrez [%s] pour arrêter Le processus.\n",EXIT_CHAR);
	do {
		/* boucle d'arrêt */
		stop = getchar();
		viderBuffer();
	}	while (isFlag(&stop, EXIT_CHAR));
	printf("Fermeture du chat\n");
	// TODO avec tas: closeSocket(everySocket pid);
	closeSocket(listeningSocket_pid, numSocket);
}

void closeSocket(pid_t p, int numSocket) {
	kill(p, SIGUSR1); /* kill child process, need sudo if SIGKILL */
	h_close(numSocket); /* fermeture de la socket en attente */
}

void createListeningSocket(pid_t* pid, int* numSocket, char* port) {
	/* SOCK_STREAM = TCP */
	*numSocket = h_socket(AF_INET, SOCK_STREAM); /* création de la socket */
	struct sockaddr_in *p_adr_serveur;
	adr_socket(port, NULL, SOCK_STREAM, &p_adr_serveur); /* création de l'adresse de la socket */
	if (h_bind(*numSocket, p_adr_serveur) != -1) {
		h_listen(*numSocket, NB_CON);
	}
	*pid = fork();
	if (*pid < 0) {
		fprintf(stderr, "Erreur lors de la création du processus serveur.\n");
	} else if (*pid == PROCESSUS_FILS) {
		while (1) {
			serverChat(*numSocket);
		}
	}
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
