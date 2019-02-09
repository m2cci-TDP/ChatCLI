#ifndef H_CHAT
#define H_CHAT

void serverChat (int socket);
void clientChat (int socket);
void setMessage (char message[]);
void viderBuffer(void);

#endif
