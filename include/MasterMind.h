/********************************************************************************/
/*				FON.H						*/
/********************************************************************************/
/*			Auteurs	: 						*/
/*				  Pascal Sicard					*/
/*				  Marc Garnier					*/
/*				  Elizabeth Maillet				*/
/*				  Christian Rabedaoro				*/
/*										*/
/*			date 	: 14 Septembre 1994				*/
/********************************************************************************/
/*	Prototypage des fonctions sur les sockets ( pour fon.c ).		*/
/********************************************************************************/

/*=================== INCLUSIONS STANDARDS de PROTOTYPES =============*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

/*------------------ CAS des prises TCP/IP ----------*/
#include <netinet/in.h>
#include <netdb.h>

/*------------------ definitions --------------------*/
/*
char* presentationJeu();
*/