#ifndef H_UTIL
#define H_UTIL

#define BUFFER_SIZE 2000 /* taille du buffer emission/reception */
#define NB_CON 5 /* nombre de connection serveur TCP */
#define SERVICE_DEFAUT "1111" /* port par défaut */
#define SERVEUR_DEFAUT "127.0.0.1" /* serveur par défaut */

char bufferEmission[BUFFER_SIZE+1];
char bufferReception[BUFFER_SIZE+1];

/* cellule de socket */
typedef struct cellSock {
  int socket;
  struct lSocket* pNext;
  struct lSocket* pPast;
} cellSock;
typedef cellSock *pCellSock;
/* liste chainée de cellule de socket */
typedef struct {
  int length;
  pCellSock head;
  pCellSock tail;
} lSocket;
/* methods */
int getLength (lSocket S); /* return length */
void makeLSocket (lSocket *S); /* constructeur */
void rmLSocket (lSocket *S); /* destructeur */
void setSocket (lSocket *S, int socket); /* add */
void rmSocket (lSocket *S, int socket); /* remove */
int getSocket (lSocket S, int noSocket); /* getter */

int isFlag(char* string, char* flag);
void cli (int argc, char *argv[], char **service, char **serveur);

#endif
