#ifndef H_CHAT
#define H_CHAT

#define CLIENT_EXIT_CHAR "exit"
#define SERVEUR_EXIT_CHAR 'q'

void serverChat (int socket);
void serverTCP (char *service);
void clientChat (int socket);
void clientTCP (char *serveur, char *service);



#endif
