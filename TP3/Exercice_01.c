#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>
#include <errno.h> // Ajout pour gérer les erreurs ERRNO

#define SIZE (int)1e8
#define NUM_THREADS 4

int tab[SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int global_min = INT_MAX;
int global_max = INT_MIN;

typedef struct {
    int start;
    int end;
} ThreadRange;

// Cette fonction trouve le min et max dans un sous-ensemble de 'tab'
void *find_min_max(void *arg) {
    ThreadRange *range = (ThreadRange *)arg;
    int local_min = INT_MAX;
    int local_max = INT_MIN;

    // Trouver le min et max locaux
    for (int i = range->start; i < range->end; i++) {
        if (tab[i] < local_min) local_min = tab[i];
        if (tab[i] > local_max) local_max = tab[i];
    }

    // Protéger l'accès aux variables globales
    if(pthread_mutex_lock(&mutex) != 0) {
        perror("pthread_mutex_lock"); // Afficher l'erreur
        return NULL;
    }
    if (local_min < global_min) global_min = local_min;
    if (local_max > global_max) global_max = local_max;
    if(pthread_mutex_unlock(&mutex) != 0) {
        perror("pthread_mutex_unlock"); // Afficher l'erreur
        return NULL;
    }

    return NULL;
}

int main() {
    // Initialisation de 'tab' avec des nombres aléatoires
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        tab[i] = rand();
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    pthread_t threads[NUM_THREADS];
    ThreadRange ranges[NUM_THREADS];

    // Créer les threads et assigner leurs sous-ensembles respectifs de 'tab'
    for (int i = 0; i < NUM_THREADS; i++) {
        ranges[i].start = i * (SIZE / NUM_THREADS);
        ranges[i].end = (i + 1) * (SIZE / NUM_THREADS);
        if(pthread_create(&threads[i], NULL, find_min_max, &ranges[i]) != 0) {
            perror("pthread_create"); // Afficher l'erreur
            return EXIT_FAILURE;
        }
    }

    // Attendre que tous les threads aient terminé
    for (int i = 0; i < NUM_THREADS; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join"); // Afficher l'erreur
            return EXIT_FAILURE;
        }
    }

    gettimeofday(&end, NULL);

    long time_elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    printf("Minimum: %d\n", global_min);
    printf("Maximum: %d\n", global_max);
    printf("Temps écoulé: %ld µs\n", time_elapsed);

    return 0;
}