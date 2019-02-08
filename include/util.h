#ifndef H_UTIL
#define H_UTIL

#define BUFFER_SIZE 2000 /* taille du buffer emission/reception */
#define NB_CON 5 /* nombre de connection serveur TCP */
#define SERVICE_DEFAUT "1111" /* port par défaut */
#define SERVEUR_DEFAUT "127.0.0.1" /* serveur par défaut */

char bufferEmission[BUFFER_SIZE+1];
char bufferReception[BUFFER_SIZE+1];

int isFlag(char* string, char* flag);
void cli (int argc, char *argv[], char **service, char **serveur);

#endif
