#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "fon.h"
#include "chat.h"
#include "util.h"

void serverChat (int socket) {
  struct sockaddr_in p_adr_client;
  int socketClient;

  if ((socketClient = h_accept(socket, &p_adr_client)) != -1) {
    pid_t p;
    if ((p = fork()) < 0) {
      fprintf(stderr, "Erreur lors de la connexion.\n");
    } else if (p == 0) {
      /* code du fils */
      char name[BUFFER_SIZE];
      int nbOctRecus = h_reads(socketClient, bufferReception, BUFFER_SIZE); /* lecture du message pseudo */
      if (nbOctRecus == -1) {
        fprintf(stderr, "Erreur lors de la réception de la socket.\n");
      } else {
        strcpy(name, bufferReception);
      }

      /* chat */
      char str[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &p_adr_client.sin_addr, str, INET_ADDRSTRLEN);
      printf("%s (%s) entre dans le chat.\n", name, str);
      while (!isFlag(bufferReception, ".")) {
        int nbOctRecus = h_reads(socketClient, bufferReception, BUFFER_SIZE); /* lecture du message avant espaces */
        if (nbOctRecus == -1) {
          fprintf(stderr, "Erreur lors de la réception de la socket.\n");
        } else {
          sprintf(bufferEmission, "%s : %s", name, bufferReception);
          h_writes(socketClient, bufferEmission, BUFFER_SIZE);
        }
      }

      printf("%s quitte le chat.\n", name);
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

  while (!isFlag(bufferEmission, ".")) {
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
