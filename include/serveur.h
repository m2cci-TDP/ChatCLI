#ifndef H_CLIENT
#define H_CLIENT

//TODO move dans socketUtils
void registerSocket (pid_t pid, int socket);
void closeSocket(pid_t p, int numSocket);

void serverTCP (char*);

void runMainThread ();
void closeChat ();
int createListeningSocket(char* port);

void runListeningSocketThread (int listeningSocket);
int waitForNewConnection (int listeningSocket, struct sockaddr_in *clientIp);
void handleNewConnection (int dedicatedSocket, struct sockaddr_in clientIp);

void registerClient (int dedicatedSocket, struct sockaddr_in clientSocAddr, char* clientName);
void parseClientName (int socketClient, char* clientName);
void parseClientIp (struct sockaddr_in p_adr_client, char *ipAddr);

void handleClient (int dedicatedSocket, struct sockaddr_in clientIp, char* clientName);
int readClientInput (int dedicatedSocket, struct sockaddr_in clientIp, char* clientName);
void processClientLogout (char* clientName);

#endif
