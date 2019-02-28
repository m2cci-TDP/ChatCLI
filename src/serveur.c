#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <sys/mman.h>
#include <string.h>
#include "util.h"
#include "serveur.h"
#include "fon.h"

static lSocket *globalSocketList;

void serverTCP (char *port) {
	clearScreen();
	printf("[serverTCP] Running chat as server on port: %s\n", port);

	// on crée la socket principale qui va écouter pour de nouvelles connexions
	int listeningSocket = createListeningSocket(port);
	if ((globalSocketList = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) { /* init shared memory */
		fprintf(stderr, "Echec de la création de la mémoire partagée.\n");
		exit(1);
	}
	makeLSocket(globalSocketList);

	// Le main thread se sépare en deux, le main va attendre "exit" pour que le serveur ferme
	// Le fils va boucler a l'infini en attendant les nouvelles connexions
	pid_t pid = fork();
	if (pid > 0) {
		// ici pid = PID{process_fils
		runMainThread();
		closeChat(listeningSocket, 0);
	} else if (pid == PROCESSUS_FILS) { // new thread: listeningSocket, pid = 0
		runListeningSocketThread(listeningSocket);
	} else {
		fprintf(stderr, "[serverTCP] Erreur lors de la création du processus serveur.\n");
	}
	// A la fin de chaque boucle infinie, on ferme la scocket principale, et la mémoire partagée
	rmLSocket(globalSocketList);
	munmap(globalSocketList, MAP_SIZE);
}

void closeChat (int listeningSocket, pid_t listeningSocketPid) {
	//TODO: sendMessage ("le serveur ferme ses portes");
	printf("[closeChat] Fermeture du chat\n");
	h_close(listeningSocket); /* fermeture de la socket en attente */
	// kill(p, SIGUSR1); /* kill child process, need sudo if SIGKILL */
	kill(listeningSocketPid, SIGTERM);
}

void registerSocket (pid_t pid, int socket) {
	// Pour monitorer les sockets ouvertes et les fermer proprement,
	// on ajoute les infos nécessaires à une liste partagée entre tous les threads
	addSocket(globalSocketList, socket);
	printf("[registerSocket] Nombre de socket : %d\n\tNo des sockets ouvertes : ", getLength(*globalSocketList));
	printAll(*globalSocketList);
}
void closeSocketClient (pid_t pid, int socket) {
	// Pour fermer une socket proprement, on ferme la socket, puis on kill le thread qui écoutait dessus
	rmSocket(globalSocketList, socket);
	printf("[closeSocketClient] Nombre de socket : %d\n\tNo des sockets ouvertes : ", getLength(*globalSocketList));
	printAll(*globalSocketList);
	h_close(socket);
}

void runMainThread () {
	// Le thread principal tourne tant que le gérant du serveur (l'humain), ne tape pas "exit"
	char stop[5];
	printf("[runMainThread] Entrez [%s] pour arrêter Le processus.\n", EXIT_CHAR);
	do {
		getString(stop);
	}	while (!isFlag(stop, EXIT_CHAR));
}

int createListeningSocket(char* port) {
	/* SOCK_STREAM = TCP */
	// Notre application est un chat, on n'écoute donc que en TCP
	int numSocket = h_socket(AF_INET, SOCK_STREAM); /* création de la socket */
	struct sockaddr_in *p_adr_serveur;
	adr_socket(port, NULL, SOCK_STREAM, &p_adr_serveur); /* création de l'adresse de la socket */
	if (h_bind(numSocket, p_adr_serveur) == -1) {
		fprintf(stderr, "[createListeningSocket] Echec de la création du server.\n");
		exit(1);
	} else {
		// Si on a réussi à binder, on mets la socket en écoute
		h_listen(numSocket, NB_CON);
	}
	return numSocket;
}

void runListeningSocketThread (int listeningSocket) {
	// Le thread de la socket principale est d'attendre pour de nouvelles connexions
	// et de créer les sockets/threads correspondants
	struct sockaddr_in clientIp;
	while (1) {
		// waitForNewConnection est bloquant. On ne passe à la ligne suivante que lorsqu'une
		// nouvelle connexion a été détectée
		int newSocket = waitForNewConnection(listeningSocket, &clientIp);
		// On fait ensuite le nécessaire pour acceuillir le nouveau client
		handleNewConnection(newSocket, clientIp);
	}
}

int waitForNewConnection (int listeningSocket, struct sockaddr_in* clientIp) {
	int newSocket = 0;
	// h_accept est bloquant. L'affectation à newSocket se fait que lors d'une nouvelle connexion
	while ((newSocket = h_accept(listeningSocket, clientIp)) == -1) {
		// Si l'opération est un échec, on continue d'écouter, et on affiche un message d'erreur
		fprintf(stderr, "[waitForNewConnection] Nombre de connexions complet.\n");
	}
	// Si l'opération est réussie, on enregistre la socket nouvellement créée
	registerSocket(getpid(), newSocket);
	return newSocket;
}

void handleNewConnection (int dedicatedSocket, struct sockaddr_in clientSocket) {
	printf( "[handleNewConnection] Handling new connection to socket %d\n", dedicatedSocket);
	// Pour chaque nouvelle connexion, on crée un thread dédié qui sera chargé de gérer les échanges
	// avec ce client particulier
	pid_t pid = fork();
	if (pid > 0) { // main thread : listeningSocket
		//registerSocket(pid, dedicatedSocket);
	} else if (pid == PROCESSUS_FILS) {
		// Le processus fils va réaliser toutes les étapes de gestion d'un client.
		char clientName[BUFFER_SIZE] = "";
		// On va d"abord extraire les informations pour le chat
		registerClient(dedicatedSocket, clientSocket, clientName);
		// On va ensuite gérer les intéractions dans une boucle infinie, jusqu'a fermeture
		// handleClient est semi-bloquant
		handleClient(dedicatedSocket, clientSocket, clientName);
		// Lorsque la connexion est fermée, on ferme la socket
		closeSocketClient(getpid(), dedicatedSocket);
	} else {
		fprintf(stderr, "[handleNewConnection] Erreur lors de la création du processus du client.\n");
	}
}

void registerClient (int dedicatedSocket, struct sockaddr_in clientSocAddr, char* clientName) {
	// On va extraire les informations nécessaires à l'application
	char clientIp[INET_ADDRSTRLEN];
	// Le premier échange après l'établissement de la connexion est le pseudo du client sur le chat
	parseClientName(dedicatedSocket, clientName);
	// On enregistre également son adresse IP (pour faire pro)
	parseClientIp(clientSocAddr, clientIp);
	// On notifie tout le monde d'un nouvel arrivant dans le chat
	sprintf(bufferEmission, "%s (%s) entre dans le chat.", clientName, clientIp);
	printf("[registerClient] %s\n", bufferEmission);
	sendToAll(*globalSocketList, bufferEmission, BUFFER_SIZE);
}

void parseClientName (int socketClient, char* clientName) {
	// On gère ici le pseudo du client. D'abord on le demande, ensuite on le recoit
	// On envoie la demande de login au client
	sendMessage(socketClient, "Bienvenue dans le client de chat !\nVeuillez entrez votre pseudo : ");

	int nbOctRecus = h_reads(socketClient, bufferReception, BUFFER_SIZE); /* lecture du message pseudo */
	if (nbOctRecus == -1) {
		throwSocketReceptionError();
	} else {
		strcpy(clientName, bufferReception);
		sprintf(bufferEmission, "Vous avez choisi \"%s\" comme nom.", clientName);
		sendMessage(socketClient, bufferEmission);
		sprintf(bufferEmission, "Vous pouvez quitter l'application à tout moment en tapant [%s]\n\n\n", EXIT_CHAR);
		sendMessage(socketClient, bufferEmission);
	}
}

void parseClientIp (struct sockaddr_in p_adr_client, char *ipAddr) {
	// On utilise la macro pour lire l'addresse IP (on est en TCP)
	inet_ntop(AF_INET, &p_adr_client.sin_addr, ipAddr, INET_ADDRSTRLEN);
}

void handleClient (int dedicatedSocket, struct sockaddr_in clientIp, char* clientName) {
	// readClientInput s'arrete quand le client tape "exit", ou que la connexion s'arrete
	while (readClientInput(dedicatedSocket, clientIp, clientName)) {
		// Chaque entrée du client est retransmise à tous les autres clients
		sendToAll(*globalSocketList, bufferEmission, BUFFER_SIZE);
	}
	// Quand le client sors, on gère sa deconnexion, et on notifie tous les autres utilisateurs
	processClientLogout(clientName);
	sendMessage(dedicatedSocket, "\nA bientôt !\nMerci d'avoir utiliser le chat !\n");
}

int readClientInput (int dedicatedSocket, struct sockaddr_in clientIp, char* clientName) {
	//sendMessage(dedicatedSocket, "\nVotre message : ");
	int test = -1;
	int nbOctRecus = h_reads(dedicatedSocket, bufferReception, BUFFER_SIZE); /* lecture du message avant espaces */
	if (nbOctRecus == -1) {
		throwSocketReceptionError();
	} else {
		if (!isFlag(bufferReception, "")) {
			// Le serveur lit le message
			sprintf(bufferEmission, "%s : %s\n", clientName, bufferReception);
			printf("[readClientInput process %d] %s says \"%s\"\n", getpid(), clientName, bufferReception);
		}
		test = !isFlag(bufferReception, EXIT_CHAR);
	}
	return test;
}

void processClientLogout (char* clientName) {
	printf("[processClientLogout] %s quitte le chat.\n", clientName);
}
