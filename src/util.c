#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include "util.h"
#include "fon.h"

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

void printUsage (){
	printf("Usage: chat [-c/-s] [OPTIONS]\n");
	printf("-s, --server\t\tmode server\n");
	printf("-c, --client\t\tmode client\n");
	printf("OPTIONS:\n");
	printf("-t, --target\t<IP>\tIP address of target if client (default: %s)\n", SERVEUR_DEFAUT);
	printf("-p, --port\t<port>\tport (default: %s)\n", SERVICE_DEFAUT);
	printf("-h, --help\t\tprint usage\n");
}

void exitWithUsage (void) {
	printUsage();
	exit(1);
}

int readStringParam(int argc, char *argv[], int index, char** output) {
	if (index >= argc || strncmp(argv[index], "-", 1) == 0 ) {
		return 0;
	} else {
		*output = argv[index];
		return 1;
	}
}

int cli (int argc, char *argv[], char **service, char **serveur, Mode* mode) {
	*mode = -1;
	int readingSuccess = 1;
	char* token;
	for (int i = 1; i < argc; i++) {
		token = argv[i];
		if (isFlag(token, "-t") || isFlag(token, "--target")) {
			readingSuccess = readStringParam(argc, argv, ++i, serveur);
		} else if (isFlag(token, "-p") || isFlag(token, "--port")) {
			readingSuccess = readStringParam(argc, argv, ++i, service);
		} else if (isFlag(token, "-c") || isFlag(token, "--client")) {
			*mode = CLIENT;
		} else if (isFlag(token, "-s") || isFlag(token, "--server")) {
			*mode = SERVEUR;
		} else if (isFlag(token, "-h") || isFlag(token, "--help")) {
			exitWithUsage();
		} else {
			printf("Flag [%s] not recognized\n", token);
			exitWithUsage();
		}
		if (readingSuccess != 1) {
			printf("Syntax error, missing parameter after [%s] flag\n", token);
		}
	}

	return readingSuccess;
}

void throwSocketReceptionError() {
	fprintf(stderr, "Erreur lors de la réception de la socket.\n");
}

void readPrint (int socket) {
	if (h_reads(socket, bufferReception, BUFFER_SIZE) == -1) { /* lecture du message avant espaces */
		fprintf(stderr, "Erreur lors de la réception de la socket.\n");
	} else {
		printf("%s", bufferReception); /* écriture */
	}
}
void sendMessage (int socket, char message[]) {
	sprintf(bufferEmission, "%s", message);
	h_writes(socket, bufferEmission, BUFFER_SIZE);
}

/* liste chainée */
int getLength (lSocket S) {
	return S.length;
}
void makeLSocket (lSocket *S) {
	S->length = 0;
	S->head = NULL;
	S->tail = NULL;
}
void rmLSocket (lSocket *S) {

}
void setSocket (lSocket *S, int socket) {
	pCellSock newCell = (pCellSock)malloc(sizeof(cellSock));
	if (newCell == NULL) {
		fprintf(stderr, "Mémoire pleine.\n");
		exit(1);
	}
	newCell->socket = socket;
	newCell->pNext = NULL;
	if (S->head == NULL) {
		/* première socket */
		S->head = newCell;
		S->tail = newCell;
	} else {
		(S->tail)->pNext = newCell;
		S->tail = newCell;
	}
	(S->length)++;
}
void rmSocket (lSocket *S, int socket) {

}
int getSocket (lSocket S, int noSocket) {

}
