#ifndef __TSL3_H
#define __TSL3_H
#include <pthread.h>
#include "semaphore.h"

typedef enum {LECTEUR,REDACTEUR,NO_TYPE} type;

//Pour identifier le type d'un thread s'etant enregistre
typedef struct {
    int thread_id;
    type type;
} identifier_t;


typedef struct node {
   	identifier_t* id;
	struct node* next;

} node;

typedef struct {
	pthread_mutex_t mutex_on_list;
	node* head;
	node* tail;
	pthread_cond_t* cond_tab;
} thread_safe_list;

typedef struct {
    	thread_safe_list list;
	pthread_mutex_t global;
	int nbL;
	int nbR;
	int isWriting;
        
} lecteur_redacteur_t;

int init_list(thread_safe_list* list);

void add_element(thread_safe_list* list,type type, int thread_id);

int is_head(thread_safe_list* list,type type, int thread_id);

type head_element_type(thread_safe_list* list);

void signal_head(thread_safe_list* list);

#endif
