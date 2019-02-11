#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include "util.h"
#include "client.h"
#include "fon.h"




void clientTCP (char *serveur, char *service) {
	printf("Running chat as client.\n");
	printf("serveur: %s\n", serveur);
	printf("port: %s\n", service);

	int noSocket = h_socket(AF_INET, SOCK_STREAM);
	struct sockaddr_in *socket_target;
	adr_socket(SERVICE_DEFAUT, serveur, SOCK_STREAM, &socket_target);
	if (h_connect(noSocket, socket_target) != -1) {
		clientChat(noSocket);
	}
	h_close(noSocket);
}

void clientChat (int socket) {
  printf("Bienvenue dans le client de chat !\n");
  printf("Veuillez entrez votre pseudo : ");
  setMessage(bufferEmission);
  h_writes(socket, bufferEmission, BUFFER_SIZE);
  printf("\nVous avez choisi \"%s\" comme nom.\n", bufferEmission);
  printf("Vous pouvez quitter l'application à tout moment en tapant [%s]\n\n", EXIT_CHAR);

  while (!isFlag(bufferEmission, EXIT_CHAR)) {
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
