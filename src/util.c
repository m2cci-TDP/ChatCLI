#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

int isFlag (char* string, char* flag) {
	return strcmp(string, flag) == 0;
}

void cli (int argc, char *argv[], char **service, char **serveur) {
  char *mode = strstr(argv[0], "client");
  if (mode == NULL) {
    mode = strstr(argv[0], "serveur");
  }

  if (serveur == NULL && isFlag(mode, "client")) {
    fprintf(stderr, "L'attribut \"serveur\" doit être instancié.\n");
    exit(1);
  }

  for (int i = 1; i < argc; i++) {
    if (isFlag(mode, "client") && isFlag(argv[i], "-t") || isFlag(argv[i], "--target")) {
      *serveur = argv[++i];
    } else if (isFlag(argv[i], "-p") || isFlag(argv[i], "--port")) {
      *service = argv[++i];
      //printf("%s\n", service);
    } else {
      printf("Flag [%s] not recognized\n", argv[i]);
      printf("Usage: %s [OPTIONS]\n", mode);
      if (isFlag(mode, "client")) {
        printf("-t, --target <IPaddr>\t\tIP address of target\n");
      }
      printf("-p, --port <port>\t\tport\n");
      exit(1);
    }
  }
  if (isFlag(mode, "client")) {
    printf("serveur: %s, ", *serveur);
  }
  printf("port: %s\n", *service);
}

/* liste chainée */
int getLength (lSocket S) {

}
void makeLSocket (lSocket *S) {

}
void rmLSocket (lSocket *S) {

}
void setSocket (lSocket *S, int socket) {

}
void rmSocket (lSocket *S, int socket) {

}
int getSocket (lSocket S, int noSocket) {

}
