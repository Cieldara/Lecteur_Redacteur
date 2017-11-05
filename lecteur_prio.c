#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lecteur_redacteur2.h"

typedef struct {
    lecteur_redacteur_t lecteur_redacteur;
    int iterations;
    int donnee;
} donnees_thread_t;

void dodo(int scale) {
    usleep((random()%1000000)*scale);
}

void debut_lecture(lecteur_redacteur_t* lr){
	pthread_mutex_lock(&lr->global);
	if(lr->nbL == 0){
		printf("Thread %x : ATTENTE LECTURE\n", (int) pthread_self());
		sem_wait(&lr->sem);
	}
	lr->nbL++;
	pthread_mutex_unlock(&lr->global);
}

void fin_lecture(lecteur_redacteur_t* lr){
	pthread_mutex_lock(&lr->global);
	lr->nbL--;
	if(lr->nbL == 0){
		sem_post(&lr->sem);
	}
	
	pthread_mutex_unlock(&lr->global);
}

void debut_redaction(lecteur_redacteur_t* lr){
	pthread_mutex_lock(&lr->mutex_red);
	sem_wait(&lr->sem);
}

void fin_redaction(lecteur_redacteur_t* lr){
	sem_post(&lr->sem);
	pthread_mutex_unlock(&lr->mutex_red);
}


void initialiser_lecteur_redacteur(lecteur_redacteur_t* lr){
	int i;
	lr->nbL = 0;
	i = pthread_mutex_init(&lr->global, NULL);
	printf("%d\n",i);
	sem_init(&lr->sem,0,1);
	pthread_mutex_init(&lr->mutex_red,NULL);
}

void detruire_lecteur_redacteur(lecteur_redacteur_t* lr){
	pthread_mutex_destroy(&lr->global);
	pthread_mutex_destroy(&lr->mutex_red);
	sem_destroy(&lr->sem);
}




void *lecteur(void *args) {
    donnees_thread_t *d = args;
    int i, valeur;
    srandom((int) pthread_self());

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
    srandom((int) pthread_self());

    for (i=0; i < d->iterations; i++) {
        dodo(2);
        debut_redaction(&d->lecteur_redacteur);
        	printf("Thread %x : debut redaction......\n", (int) pthread_self());
        	valeur = random();
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
        fprintf(stderr, "Utilisation: %s nb_lecteurs nb_redacteurs "
                        "nb_iterations\n", argv[0]);
        exit(1);
    }

    nb_lecteurs = atoi(argv[1]);
    nb_redacteurs = atoi(argv[2]);
    donnees_thread.iterations = atoi(argv[3]);

    threads = malloc((nb_lecteurs+nb_redacteurs)*sizeof(pthread_t));
    thread_courant = threads;
    initialiser_lecteur_redacteur(&donnees_thread.lecteur_redacteur);

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
