#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "util.h"
#include "fon.h"

/* comparaison de chaine de caractère */
int isFlag (char* string, char* flag) {
	return strcmp(string, flag) == 0;
}

/* remplissage d'un tableau par le clavier avec vidange du buffer */
void getString (char message[]) {
	strcpy(message, "");
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

/* not use */
void viderBuffer(void)
{
	char poubelle;
	do poubelle = getchar();
	while (poubelle != '\n' && poubelle != EOF);
}

/* help interface */
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

/* lecture des options du CLI */
int readStringParam(int argc, char *argv[], int index, char** output) {
	if (index >= argc || strncmp(argv[index], "-", 1) == 0 ) {
		return 0;
	} else {
		*output = argv[index];
		return 1;
	}
}

/* CLI */
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

void throwSocketReceptionError () {
	fprintf(stderr, "Erreur lors de la réception de la socket.\n");
}

/* reception du message du serveur puis impression à l'écran */
int readPrint (int socket) {
	int success = h_reads(socket, bufferReception, BUFFER_SIZE) > 0; /* lecture du message avant espaces */
	if (!success) {
		fprintf(stderr, "[readPrint] Erreur lors de la réception de la socket.\n");
	} else {
		printf("%s", bufferReception); /* écriture */
	}
	return success;
}

/* envoi d'un chaine de caractère par TCP */
void sendMessage (int socket, char message[]) {
	sprintf(bufferEmission, "%s", message);
	h_writes(socket, bufferEmission, BUFFER_SIZE);
}

/* liste chainée
* voir util.h
*/
int getLength (lSocket S) {
	return S.length;
}
void makeLSocket (lSocket *S) {
	S->length = 0;
	S->head = NULL;
}
void rmLSocket (lSocket *S) {
	if (S->head != NULL) {
		rmSocket(S, (S->head)->socket);
		rmLSocket(S);
	}
}
void exitIfMemoryFull (pCellSock p) {
	if (p == NULL) {
		fprintf(stderr, "Mémoire pleine.\n");
		exit(1);
	}
}
void addSocket (lSocket *S, int socket) {
	//pCellSock newCell = (pCellSock)malloc(CELL_SIZE);
	//exitIfMemoryFull(newCell);
	pCellSock newCell;
	if ((newCell = mmap(NULL, CELL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) { /* init shared memory */
		fprintf(stderr, "Echec de la création de la mémoire partagée.\n");
		exit(1);
	}

	newCell->socket = socket;
	newCell->pNext = NULL;
	if (S->head == NULL) {
		/* première socket */
		S->head = newCell;
	} else {
		/* ajout en tête */
		newCell->pNext = S->head;
		S->head = newCell;
	}
	(S->length)++;
}
void getCellSock (int socket, int* noSocket, pCellSock* ac, pCellSock* ap) {
	if (*ac != NULL && (*ac)->socket != socket && *noSocket != 1) {
		*ap = *ac;
		*ac = (*ac)->pNext;
		(*noSocket)--;
		getCellSock(socket, noSocket, ac, ap);
	}
}
void exitSocketNotFind (pCellSock ac, int socket, int isNo) {
	char type[14] = "socket";
	if (isNo) {
		sprintf(type, "%s numéro", type);
	}
	if (ac == NULL) {
		printf("%s %d non trouvée\n", type, socket);
		exit(1);
	}
}
void rmSocket (lSocket *S, int socket) {
	pCellSock ficCell = (pCellSock)malloc(sizeof(cellSock));
	exitIfMemoryFull(ficCell);
	ficCell->pNext = S->head;

	pCellSock ac = S->head, ap = ficCell;
	int noSocket = -1;
	getCellSock(socket, &noSocket, &ac, &ap);
	exitSocketNotFind(ac, socket, 0);
	ap->pNext = ac->pNext;
	munmap(ac, CELL_SIZE);
	//free(ac);
	S->head = ficCell->pNext;
	free(ficCell);
	(S->length)--;
}
int getSocket (lSocket S, int noSocket) {
	pCellSock ac = S.head, ap = ac;
	int noSocketTMP = noSocket;
	getCellSock(-1, &noSocket, &ac, &ap);
	exitSocketNotFind(ac, noSocketTMP, 1);
	return ac->socket;
}
void sendToAllSockets (pCellSock ac, char* message, int bufferSize) {
	if (ac != NULL) {
		printf("[sendToAllSockets process %d] %d at %p\n", getpid(), ac->socket, ac);
		h_writes(ac->socket, message, bufferSize);
		sendToAllSockets(ac->pNext, message, bufferSize);
	}
}
void sendToAll (lSocket S, char* message, int bufferSize) {
	if (getLength(S) > 0 && !isFlag(message, "")) {
		printf("[sendToAll process %d] Tete at %p\n", getpid(), S.head);
		sendToAllSockets(S.head, message, bufferSize);
		strcpy(message, "");
	}
}
void printAllSockets (pCellSock ac) {
	if (ac != NULL) {
		printf("%d ", ac->socket);
		printAllSockets(ac->pNext);
	}
}
void printAll (lSocket S) {
	printAllSockets(S.head);
	printf("\n");
}
