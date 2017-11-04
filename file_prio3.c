#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "thread_safe_list_version3.h"
#include "semaphore.h"

typedef struct {
    lecteur_redacteur_t lecteur_redacteur;
    int iterations;
    int donnee;
} donnees_thread_t;

typedef struct{
	 donnees_thread_t* donnee;
	 int id;
}donnees_et_id;

void dodo(int scale) {
    usleep((random()%1000000)*scale);
}

void debut_lecture(lecteur_redacteur_t* lr,int id){
    	add_element(&lr->list,LECTEUR,id);
	pthread_mutex_lock(&lr->global);
	while(lr->nbR > 0  || !is_head(&lr->list,LECTEUR,id)){
		pthread_cond_wait(&lr->list.cond_tab[id], &lr->global);
	}
	lr->nbL++;
	signal_head(&lr->list);
	pthread_mutex_unlock(&lr->global);
}

void fin_lecture(lecteur_redacteur_t* lr, int id){
	pthread_mutex_lock(&lr->global);
	lr->nbL--;	
	if(head_element_type(&lr->list) == REDACTEUR){
		signal_head(&lr->list);
	}
	pthread_mutex_unlock(&lr->global);
}

void debut_redaction(lecteur_redacteur_t* lr, int id){
   	add_element(&lr->list,REDACTEUR,id);
	pthread_mutex_lock(&lr->global);
	while (lr->isWriting || lr->nbL > 0 || !is_head(&lr->list,REDACTEUR,id)){	
		pthread_cond_wait(&lr->list.cond_tab[id], &lr->global);
	}
	lr->nbR++;
	lr->isWriting = 1;
	pthread_mutex_unlock(&lr->global);
}

void fin_redaction(lecteur_redacteur_t* lr, int id){
	pthread_mutex_lock(&lr->global);
	lr->nbR--;
	lr->isWriting = 0;
	signal_head(&lr->list);
	pthread_mutex_unlock(&lr->global);
}


void initialiser_lecteur_redacteur(lecteur_redacteur_t* lr, int nb){
	
	printf("%d\n",pthread_mutex_init(&lr->global, NULL));
	init_list(&lr->list);
	lr->nbL = 0;
	lr->nbR = 0;int i = 0;
	lr->isWriting = 0;
	lr->list.cond_tab = malloc(nb*sizeof(pthread_cond_t));
	for( i = 0; i <nb;i++){
		printf("%d\n",pthread_cond_init(&lr->list.cond_tab[i],NULL));
	}
}

void detruire_lecteur_redacteur(lecteur_redacteur_t* lr){
	pthread_mutex_destroy(&lr->global);
	pthread_mutex_destroy(&lr->list.mutex_on_list);
}




void *lecteur(void *args) {
    donnees_et_id* data = args;
    donnees_thread_t *d = data->donnee;
    int i, valeur,id;
    id = data->id;
    srand((int) pthread_self());
    for (i=0; i < d->iterations; i++) {
        dodo(2);
	debut_lecture(&d->lecteur_redacteur,id);
        	printf("Thread %d : debut lecture\n", id);
        	valeur = d->donnee;
        	dodo(1);
        	printf("Thread %d : ", id);
        	if (valeur != d->donnee)
            		printf("LECTURE INCOHERENTE !!!\n");
        	else
            		printf("lecture coherente\n");
        fin_lecture(&d->lecteur_redacteur,id);
    }
    pthread_exit(0);
}

void *redacteur(void *args) {
    donnees_et_id* data = args;
    donnees_thread_t *d = data->donnee;
    int i, valeur, id;
    srand((int) pthread_self());
    id = data->id;
    for (i=0; i < d->iterations; i++) {
        dodo(2);
        debut_redaction(&d->lecteur_redacteur,id);
        	printf("Thread %d : debut redaction......\n", id);
        	valeur = rand();
        	d->donnee = valeur;
        	dodo(1);
        	printf("Thread %d : ", id);
        	if (valeur != d->donnee)
            		printf("REDACTION INCOHERENTE !!!\n");
        	else
            		printf("redaction coherente......\n");
        fin_redaction(&d->lecteur_redacteur,id);
    }
    pthread_exit(0);
}



int main(int argc, char *argv[]) {
    pthread_t *threads, *thread_courant;
    donnees_thread_t donnees_thread;
    int i, nb_lecteurs, nb_redacteurs;
    void *resultat;

    if (argc < 4) {
      //printf(stderr, "Utilisation: %s nb_lecteurs nb_redacteurs "
        //              "nb_iterations\n", argv[0]);
        exit(1);
    }
	
    nb_lecteurs = atoi(argv[1]);
    nb_redacteurs = atoi(argv[2]);
    donnees_thread.iterations = atoi(argv[3]);

    threads = malloc((nb_lecteurs+nb_redacteurs)*sizeof(pthread_t));
    thread_courant = threads;
    initialiser_lecteur_redacteur(&donnees_thread.lecteur_redacteur, nb_lecteurs+nb_redacteurs);

    donnees_et_id* data;

    for (i=0; i<nb_lecteurs; i++){
	data = malloc(sizeof(donnees_et_id));
	data->donnee = &donnees_thread;
	data->id = i;
        pthread_create(thread_courant++, NULL, lecteur, data);
    }
    for (; i<nb_lecteurs+nb_redacteurs; i++){
	data = malloc(sizeof(donnees_et_id));
	data->donnee = &donnees_thread;
	data->id = i;
        pthread_create(thread_courant++, NULL, redacteur, data);
    }
    for (i=0; i<nb_lecteurs+nb_redacteurs; i++)
        pthread_join(threads[i], &resultat);
    detruire_lecteur_redacteur(&donnees_thread.lecteur_redacteur);
    free(threads);
    return 0;
}
