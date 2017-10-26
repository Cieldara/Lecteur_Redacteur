#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "thread_safe_list_version2.h"

typedef struct {
    lecteur_redacteur_t lecteur_redacteur;
    int iterations;
    int donnee;
} donnees_thread_t;

void dodo(int scale) {
    usleep((random()%1000000)*scale);
}

void debut_lecture(lecteur_redacteur_t* lr){
    add_element(&lr->list,LECTEUR,(int) pthread_self());
	pthread_mutex_lock(&lr->global);
	while (lr->nbR > 0 || !is_head(&lr->list,LECTEUR,(int) pthread_self())){
		pthread_cond_wait(&lr->fileL, &lr->global);
	}
	lr->nbL++;
	//pthread_cond_broadcast(&lr->fileL);
	pthread_mutex_unlock(&lr->global);
}

void fin_lecture(lecteur_redacteur_t* lr){
	pthread_mutex_lock(&lr->global);
	lr->nbL--;
	if (lr->nbL == 0 && head_element_type(&lr->list) == REDACTEUR){
		pthread_cond_broadcast(&lr->fileR);
	}
	pthread_mutex_unlock(&lr->global);
}

void debut_redaction(lecteur_redacteur_t* lr){
    add_element(&lr->list,REDACTEUR,(int) pthread_self());
	pthread_mutex_lock(&lr->global);
	while (lr->isWriting || lr->nbL > 0 || !is_head(&lr->list,REDACTEUR,(int) pthread_self())){
		pthread_cond_wait(&lr->fileR, &lr->global);
	}
	lr->nbR++;
	lr->isWriting = 1;
	pthread_mutex_unlock(&lr->global);
}

void fin_redaction(lecteur_redacteur_t* lr){
	pthread_mutex_lock(&lr->global);
	lr->nbR--;
	lr->isWriting = 0;
	if (head_element_type(&lr->list) == REDACTEUR){
		pthread_cond_broadcast(&lr->fileR);
	}else {
		pthread_cond_broadcast(&lr->fileL);
	}
	pthread_mutex_unlock(&lr->global);
}


void initialiser_lecteur_redacteur(lecteur_redacteur_t* lr){
	int i;
	lr->nbL = 0;
	lr->nbR = 0;
	lr->isWriting = 0;
	i = pthread_mutex_init(&lr->global, NULL);
	i =pthread_cond_init(&lr->fileL,NULL);
	i = pthread_cond_init(&lr->fileR,NULL);
	i = init_list(&lr->list);
}

void detruire_lecteur_redacteur(lecteur_redacteur_t* lr){
	pthread_mutex_destroy(&lr->global);
	pthread_mutex_destroy(&lr->list.mutex_on_list);
	pthread_cond_destroy(&lr->fileL);
	pthread_cond_destroy(&lr->fileR);
}




void *lecteur(void *args) {
    donnees_thread_t *d = args;
    int i, valeur;
    srand((int) pthread_self());

    for (i=0; i < d->iterations; i++) {
        dodo(2);
	debut_lecture(&d->lecteur_redacteur);
        	printf("Thread %x : debut lecture\n", (int) pthread_self());
        	valeur = d->donnee;
        	dodo(1);
        	printf("Thread %x : ", (int) pthread_self());
        	if (valeur != d->donnee)
            		printf("LECTURE INCOHERENTE !!!\n");
        	else
            		printf("lecture coherente\n");
        fin_lecture(&d->lecteur_redacteur);
    }
    pthread_exit(0);
}

void *redacteur(void *args) {
    donnees_thread_t *d = args;
    int i, valeur;
    srand((int) pthread_self());

    for (i=0; i < d->iterations; i++) {
        dodo(2);
        debut_redaction(&d->lecteur_redacteur);
        	printf("Thread %x : debut redaction......\n", (int) pthread_self());
        	valeur = rand();
        	d->donnee = valeur;
        	dodo(1);
        	printf("Thread %x : ", (int) pthread_self());
        	if (valeur != d->donnee)
            		printf("REDACTION INCOHERENTE !!!\n");
        	else
            		printf("redaction coherente......\n");
        fin_redaction(&d->lecteur_redacteur);
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
    initialiser_lecteur_redacteur(&donnees_thread.lecteur_redacteur);
	printf("%d et %d\n",donnees_thread.lecteur_redacteur.nbL,donnees_thread.lecteur_redacteur.nbR);

    for (i=0; i<nb_lecteurs; i++)
        pthread_create(thread_courant++, NULL, lecteur, &donnees_thread);
    for (i=0; i<nb_redacteurs; i++)
        pthread_create(thread_courant++, NULL, redacteur, &donnees_thread);

    for (i=0; i<nb_lecteurs+nb_redacteurs; i++)
        pthread_join(threads[i], &resultat);
    detruire_lecteur_redacteur(&donnees_thread.lecteur_redacteur);
    free(threads);
    return 0;
}
