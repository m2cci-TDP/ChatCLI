#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "chat.h"
#include "util.h"
#include "client.h"
#include "serveur.h"

int main (int argc,char *argv[]) {
  	char *serveur = SERVEUR_DEFAUT; /* serveur par defaut */
  	char *service = SERVICE_DEFAUT; /* numero de service par defaut (no de port) */
    Mode mode;
    int res = cli(argc, argv, &service, &serveur, &mode);
    if (res == 1) {
      if (mode == CLIENT) {
        clientTCP(serveur, service);
      } else if(mode == SERVEUR) {
        serverTCP(service);
      } else {
        printf("No mode [CLIENT/SERVEUR] detected\n");
        exitWithUsage();
      }
    } else {
      exitWithUsage();
    }

    return 0;
}
