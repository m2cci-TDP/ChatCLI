#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include "util.h"
#include "client.h"
#include "fon.h"

void clientTCP (char *serveur, char *service) {
	printf("Running chat as client on server: %s and port: %s\n", serveur, service);

	int noSocket = h_socket(AF_INET, SOCK_STREAM);
	struct sockaddr_in *socket_target;

	adr_socket(SERVICE_DEFAUT, serveur, SOCK_STREAM, &socket_target);
	if (h_connect(noSocket, socket_target) != -1) {
		pid_t pid = fork();
		if (pid < 0) {
			fprintf(stderr, "[clientTCP] Erreur lors de la création du processus client.\n");
		} else if (pid == PROCESSUS_FILS) {
			clientChat(noSocket);
			kill(getppid(), SIGKILL); /* kill father process */
		} else {
			while (hasServerConnection(noSocket)) {}
			//putCharToStdin (pid, '\n'); /* not working */
			kill(pid, SIGKILL); /* kill child process */
			fprintf(stderr, "[clientTCP] Connexion perdue avec le serveur.\n"); /* no connection */
		}
	}
	h_close(noSocket);
}

void clientChat (int socket) {
	readPrint(socket);
	setMessage(bufferEmission);
	h_writes(socket, bufferEmission, BUFFER_SIZE);
	printf("\nVous avez choisi \"%s\" comme nom.\n", bufferEmission);
	printf("Vous pouvez quitter l'application à tout moment en tapant [%s]\n\n", EXIT_CHAR);

	do {
		readPrint(socket);
		setMessage(bufferEmission);
		printf("\033[1A"); // move cursor one ligne up
		printf("\x0d"); // move the cursor in first column
		printf("\033[K"); // erase the ligne
		h_writes(socket, bufferEmission, BUFFER_SIZE);
		readPrint(socket); // read response
	} while (!isFlag(bufferEmission, EXIT_CHAR));
}

int hasServerConnection (int socket) {
	int error = 0;
	socklen_t len = sizeof (error);
	getsockopt (socket, SOL_SOCKET, SO_ERROR, &error, &len);
	return error == 0;
}

void putCharToStdin (pid_t pid, char c) {
	char fileName[20];
	sprintf(fileName, "/proc/%d/fd/0", pid);
	FILE *f = fopen(fileName, "w");
	if (f == NULL) {
		fprintf(stderr, "[putCharToStdin] Error opening stdin.\n");
		exit(1);
	}
	fprintf(f, "%c", c);
	fclose(f);
}
