#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>

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

void *find_min_max(void *arg) {
    ThreadRange *range = (ThreadRange *)arg;
    int local_min = INT_MAX;
    int local_max = INT_MIN;

    for (int i = range->start; i < range->end; i++) {
        if (tab[i] < local_min) local_min = tab[i];
        if (tab[i] > local_max) local_max = tab[i];
    }

    pthread_mutex_lock(&mutex);
    if (local_min < global_min) global_min = local_min;
    if (local_max > global_max) global_max = local_max;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        tab[i] = rand();
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    pthread_t threads[NUM_THREADS];
    ThreadRange ranges[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        ranges[i].start = i * (SIZE / NUM_THREADS);
        ranges[i].end = (i + 1) * (SIZE / NUM_THREADS);
        pthread_create(&threads[i], NULL, find_min_max, &ranges[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);

    long time_elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    printf("Minimum: %d\n", global_min);
    printf("Maximum: %d\n", global_max);
    printf("Temps écoulé: %ld µs\n", time_elapsed);

    return 0;
}
