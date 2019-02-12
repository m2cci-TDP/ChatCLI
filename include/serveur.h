#ifndef H_CLIENT
#define H_CLIENT

void serverChat (int);
void serverTCP (char*);
void createListeningSocket(pid_t*, int*, char*);
void closeSocket(pid_t, int);

#endif
