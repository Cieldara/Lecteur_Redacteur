#ifndef __LECTEUR_REDACTEUR_H
#define __LECTEUR_REDACTEUR_H
#include <pthread.h>

typedef struct {
	//ETAT 0 : PERSONNE NE LIT NI N'ECRIT // ETAT 1 : QUELQU'UN LIT // ETAT 2 : 
	int etat;
	pthread_mutex_t mutex;
	pthread_cond_t cond;

} lecteur_redacteur_t;


#endif
