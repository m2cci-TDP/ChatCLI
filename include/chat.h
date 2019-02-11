#ifndef H_CHAT
#define H_CHAT

void serverChat (int socket);
void serverTCP (char *service);
void clientChat (int socket);
void clientTCP (char *serveur, char *service);

#endif
