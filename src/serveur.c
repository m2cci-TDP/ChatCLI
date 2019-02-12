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
	pid_t pid = fork();
	if (pid > 0) {
		runMainThread();
	} else if (pid == PROCESSUS_FILS) { // new thread: listeningSocket
		int listeningSocket = createListeningSocket(port);
		registerSocket(listeningSocket, pid);	// TODO FAUX: car pid=0 pour process fils !!!
		runListeningSocketThread(numSocket);
	} else {
		fprintf(stderr, "Erreur lors de la création du processus serveur.\n");
	}

	printf("Fermeture du chat\n");
	// TODO avec tas: closeSocket(everySocket pid);
	// mieux: utiliser un trap pour tuer les subprocesses
	// encore mieux: kill 0
	closeSocket(pid, numSocket);
}

void registerSocket (pid_t pid, int socket) {
	//TODO: list.add(pid, socket);
}
void closeSocket(pid_t p, int numSocket) {
	kill(p, SIGUSR1); /* kill child process, need sudo if SIGKILL */
	h_close(numSocket); /* fermeture de la socket en attente */
}

void runMainThread () {	char stop;
	printf("Entrez [%s] pour arrêter Le processus.\n",EXIT_CHAR);
	do {
		stop = getchar();
		viderBuffer();
	}	while (isFlag(&stop, EXIT_CHAR));
}

int createListeningSocket(char* port) {
	/* SOCK_STREAM = TCP */
	int numSocket = h_socket(AF_INET, SOCK_STREAM); /* création de la socket */
	struct sockaddr_in *p_adr_serveur;
	adr_socket(port, NULL, SOCK_STREAM, &p_adr_serveur); /* création de l'adresse de la socket */
	if (h_bind(numSocket, p_adr_serveur) != -1) {
		h_listen(numSocket, NB_CON);
	}
	return numSocket;
}

void runListeningSocketThread (int listeningSocket) {
	struct sockaddr_in clientIp;
	while (1) {
		int newSocket = waitForNewConnection(listeningSocket, &clientIp);
		handleNewConnection(newSocket, clientIp);
	}
}

int waitForNewConnection (int listeningSocket, struct sockaddr_in* clientIp) {
	int newSocket = 0;
	while ((newSocket = h_accept(listeningSocket, clientIp)) == -1) {
		fprintf(stderr, "Nombre de connexions complet.\n");
	}
	return newSocket;
}

void handleNewConnection (int dedicatedSocket, struct socaddr_in clientIp) {
	pid_t pid = fork();
	if (pid > 0) { // main thread : listeningSocket
		registerSocket(pid, numSocket);
	} else if (pid == PROCESSUS_FILS) { // new thread: clientDedicatedSocket
		registerClient(dedicatedSocket, clientIp);
		handleClient(dedicatedSocket, clientIp);
		closeSocket(pid, dedicatedSocket);	// TODO: Faux: pid = 0 quand on est dans le fils
	} else {
		fprintf(stderr, "Erreur lors de la création du processus dedie client.\n");
	}
}
void registerClient (int dedicatedSocket, struct socaddr_in clientSocAddr) {
		char clientName[BUFFER_SIZE] = "";
		char clientIp[INET_ADDRSTRLEN];
		parseClientName(dedicatedSocket, &clientName);
		parseClientIp(clientSocAddr, &clientIp);
		// TODO sendToAll();
		printf("%s (%s) entre dans le chat.\n", clientName, ipAddr);
}


void processClientLogout (int socketClient, char* clientName) {
	printf("%s quitte le chat.\n", clientName);
	h_close(socketClient);
}

void parseClientName (int socketClient, char* clientName) {
	int nbOctRecus = h_reads(socketClient, bufferReception, BUFFER_SIZE); /* lecture du message pseudo */
	if (nbOctRecus == -1) {
		throwSocketReceptionError();
	} else {
		strcpy(clientName, bufferReception);
	}
}

void parseClientIp (struct sockaddr_in p_adr_client, char *ipAddr) {
	inet_ntop(AF_INET, &p_adr_client.sin_addr, ipAddr, INET_ADDRSTRLEN);
}


void handleClient (int dedicatedSocket, struct socaddr_in clientIp) {
	while (readClientInput() == 1) {
		// TODO: sendToAll()
		h_writes(dedicatedSocket, bufferEmission, BUFFER_SIZE);
	}
	processClientLogout(dedicatedSocket, clientName);
}

int handleClientInput (int dedicatedSocket, struct socaddr_in clientIp) {
	int nbOctRecus = h_reads(dedicatedSocket, bufferReception, BUFFER_SIZE); /* lecture du message avant espaces */
	if (nbOctRecus == -1) {
		throwSocketReceptionError();
	} else {
		sprintf(bufferEmission, "%s : %s", clientName, bufferReception);
	}
	return isFlag(bufferReception, EXIT_CHAR)
}

void processClientLogout (char* clientName) {
	printf("%s quitte le chat.\n", clientName);
}

/*
void serverChat (int socket) {
  struct sockaddr_in p_adr_client;
  int socketClient;

  if ((socketClient = h_accept(socket, &p_adr_client)) != -1) {
    pid_t p;
    if ((p = fork()) < 0) {
      fprintf(stderr, "Erreur lors de la connexion.\n");
    } else if (p == PROCESSUS_FILS) {
    	char clientName[BUFFER_SIZE];
      int nbOctRecus = h_reads(socketClient, bufferReception, BUFFER_SIZE); // lecture du message pseudo
      if (nbOctRecus == -1) {
        throwSocketReceptionError();
      } else {
        strcpy(clientName, bufferReception);
      }

      // chat

      char ipAddr[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &p_adr_client.sin_addr, ipAddr, INET_ADDRSTRLEN);
      printf("%s (%s) entre dans le chat.\n", clientName, ipAddr);
      while (!isFlag(bufferReception, EXIT_CHAR)) {
        int nbOctRecus = h_reads(socketClient, bufferReception, BUFFER_SIZE); // lecture du message avant espaces
        if (nbOctRecus == -1) {
          throwSocketReceptionError();
        } else {
          sprintf(bufferEmission, "%s : %s", clientName, bufferReception);
          h_writes(socketClient, bufferEmission, BUFFER_SIZE);
        }
      }

      printf("%s quitte le chat.\n", clientName);
      h_close(socketClient); // fermeture de la socket ouverte
      exit(0);
    }

    // p = pid du fils code du père
    h_close(socketClient); // fermeture de la socket ouverte
  } else {
    fprintf(stderr, "Nombre de connexions complet.\n");
  }
}
*/
