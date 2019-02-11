#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include "util.h"

int isFlag (char* string, char* flag) {
	return strcmp(string, flag) == 0;
}

void setMessage (char message[]) {
	char c;
	if ((c = getchar()) != '\n' && c != EOF) {
		strcpy(message, &c);
	}
	while ((c = getchar()) != '\n' && c != EOF)
	{
		strcat(message, &c);
	}
	//viderBuffer();
}

void viderBuffer(void)
{
	char poubelle;
	do poubelle = getchar();
	while (poubelle != '\n' && poubelle != EOF);
}

void cli (int argc, char *argv[], char **service, char **serveur) {
	char *mode = strstr(argv[0], "client");
	if (mode == NULL) {
		mode = strstr(argv[0], "serveur");
		//isSU();
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

void isSU (void) {
	pid_t p = fork();
	if (p < 0) {
		fprintf(stderr, "Erreur lors de la création du processus serveur.\n");
	} else if (p < 0) {
		/* père */
		if (kill(p, SIGKILL) == -1) {
			fprintf(stderr, "Relancez le serveur en super utilisateur.\n");
			exit(1);
		}
	} else {
		/*fils*/
		while (1) {}
	}
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
