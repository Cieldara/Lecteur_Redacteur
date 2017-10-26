#ifndef __TSL2_H
#define __TSL2_H
#include <pthread.h>

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

} thread_safe_list;

typedef struct {
    thread_safe_list list;
	pthread_mutex_t global;
	pthread_cond_t fileL, fileR;
	int nbL, nbR, isWriting;

} lecteur_redacteur_t;

int init_list(thread_safe_list* list);

int is_head(thread_safe_list* list,type type, int thread_id);

void add_element(thread_safe_list* list,type type, int thread_id);

type head_element_type(thread_safe_list* list);


#endif
