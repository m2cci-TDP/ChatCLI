#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "util.h"
#include "client.h"
#include "fon.h"

void clientTCP (char *serveur, char *service) {
	clearScreen();
	printf("Running chat as client on server: %s and port: %s\n", serveur, service);

	int noSocket = h_socket(AF_INET, SOCK_STREAM);
	struct sockaddr_in *socket_target;

	adr_socket(SERVICE_DEFAUT, serveur, SOCK_STREAM, &socket_target);
	if (h_connect(noSocket, socket_target) != -1) {
		clientChat(noSocket);
	}
	h_close(noSocket);
}

void clientChat (int socket) {
	pid_t pid = fork();
	if (pid < 0) {
		fprintf(stderr, "[clientChat] Erreur lors de la création du processus client.\n");
	} else if (pid == PROCESSUS_FILS) {
		kill(getpid(), SIGSTOP); /* stop process */
		sendKeyboardMessage(socket);
		kill(getppid(), SIGKILL); /* kill parent process */
	} else {
		readPrint(socket);
		getString(bufferEmission);
		h_writes(socket, bufferEmission, BUFFER_SIZE);
		kill(pid, SIGCONT); /* resume child process */

		while (hasServerConnection(socket) && readPrint(socket)) {
			//printf("\033[1A"); // move cursor one ligne up
			printf("\x0d"); // move the cursor in first column
			printf("\033[1B"); // move cursor one ligne down
		}
		kill(pid, SIGKILL); /* kill child process */
		fprintf(stderr, "[clientChat] Connexion perdue avec le serveur.\n"); /* no connection */
	}
}

void sendKeyboardMessage (int socket) {
	printf("\033[2B"); // move cursor two ligne down
	do {
		//printf("\nVotre message : ");
		getString(bufferEmission);
		eraseSendMessage();
		h_writes(socket, bufferEmission, BUFFER_SIZE);
	} while (!isFlag(bufferEmission, EXIT_CHAR));
	printf("\033[1B"); // move cursor one ligne down
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

/* only for linux terminal */
void eraseSendMessage (void) {
	printf("\033[1A"); // move cursor one ligne up
	printf("\x0d"); // move the cursor in first column
	printf("\033[K"); // erase the ligne
	printf("\033[1A"); // move cursor one ligne up
}
