#ifndef __LECTEUR_REDACTEUR_H
#define __LECTEUR_REDACTEUR_H
#include <pthread.h>

typedef struct {
	pthread_mutex_t global;
	pthread_cond_t fileL, fileR;
	int nbL, nbR, isWriting;

} lecteur_redacteur_t;

#endif
