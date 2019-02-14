#ifndef H_CLIENT
#define H_CLIENT

void clientTCP (char *serveur, char *service);
void clientChat (int socket);
int hasServerConnection (int socket);
void putCharToStdin (pid_t pid, char c);
void eraseSendMessage (void);

#endif
