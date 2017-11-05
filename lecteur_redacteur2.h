#ifndef __LECTEUR_REDACTEUR2_H
#define __LECTEUR_REDACTEUR2_H
#include <pthread.h>
#include "semaphore.h"


typedef struct {
	pthread_mutex_t global;
	pthread_mutex_t mutex_red;
	sem_t sem;
	int nbL;

} lecteur_redacteur_t;

#endif
