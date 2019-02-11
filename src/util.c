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

/*
0 == server => server = NULL
1 == client
*/
int cli (int argc, char *argv[], char **service, char **serveur) {
	int mode;
	/* faire erreur
	if (serveur == NULL && isFlag(mode, "client")) {
		fprintf(stderr, "L'attribut \"serveur\" doit être instancié.\n");
		exit(1);
	}
	*/

	for (int i = 1; i < argc; i++) {
		if (isFlag(argv[i], "-t") || isFlag(argv[i], "--target")) {
			*serveur = argv[++i];
		} else if (isFlag(argv[i], "-p") || isFlag(argv[i], "--port")) {
			*service = argv[++i];
		} else if (isFlag(argv[i], "-c") || isFlag(argv[i], "--client")) {
			mode = 1;
		} else if (isFlag(argv[i], "-s") || isFlag(argv[i], "--server")) {
			mode = 0;
		} else {
			printf("Flag [%s] not recognized\n", argv[i]);
			printf("Usage: [OPTIONS]\n");
			printf("-s, --server\t\tmode server\n");
			printf("-c, --client\t\tmode client\n");
			printf("-t, --target <IPaddr>\t\tIP address of target if client (-c)\n");
			printf("-p, --port <port>\t\tport\n");
			exit(1);
		}
	}
	if (mode) {
		printf("serveur: %s, ", *serveur);
	}
	printf("port: %s\n", *service);

	return mode;
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
