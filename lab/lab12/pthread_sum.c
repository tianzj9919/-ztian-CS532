/* 
  Simple Pthread Program to find the sum of a vector.
  Uses mutex locks to update the global sum. 
  Author: Purushotham Bangalore
  Date: Jan 25, 2009

  To Compile: gcc -O -Wall pthread_psum.c -lpthread
  To Run: ./a.out 1000 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    double *a;
    double sum;
    int N;
    int size;
    long tid;
} THREAD_DATA;

void *compute(void *arg) {
    THREAD_DATA *d = (THREAD_DATA *)arg;

    int myN = d->N / d->size;
    int myStart = d->tid * myN;
    int myEnd   = myStart + myN;
    if (d->tid == d->size - 1) {
        myEnd = d->N;
    }

    double mysum = 0.0;
    for (int i = myStart; i < myEnd; i++) {
        mysum += d->a[i];
    }

    pthread_mutex_lock(&mutex);
    d->sum = mysum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <# of elements> <# of threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int N    = atoi(argv[1]); 
    int size = atoi(argv[2]);

    if (N <= 0 || size <= 0) {
        fprintf(stderr, "Error: N and number of threads must be positive.\n");
        exit(EXIT_FAILURE);
    }

    // allocate and initialize the array
    double *a = (double *)malloc(sizeof(double) * N);
    if (a == NULL) {
        perror("malloc a");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < N; i++) {
        a[i] = (double)(i + 1);
    }

    // allocate thread ids and per-thread info
    pthread_t   *tid  = (pthread_t *)malloc(sizeof(pthread_t) * size);
    THREAD_DATA *info = (THREAD_DATA *)malloc(sizeof(THREAD_DATA) * size);
    if (tid == NULL || info == NULL) {
        perror("malloc");
        free(a);
        exit(EXIT_FAILURE);
    }

    // create threads
    for (long i = 0; i < size; i++) {
        info[i].a    = a;
        info[i].sum  = 0.0;
        info[i].N    = N;
        info[i].size = size;
        info[i].tid  = i;

        if (pthread_create(&tid[i], NULL, compute, &info[i]) != 0) {
            perror("pthread_create");
            free(a);
            free(tid);
            free(info);
            exit(EXIT_FAILURE);
        }
    }

    // wait for them to complete
    for (int i = 0; i < size; i++) {
        pthread_join(tid[i], NULL);
    }

    // compute the global sum from all partial sums
    double total = 0.0;
    for (int i = 0; i < size; i++) {
        total += info[i].sum;
    }

    printf("The total is %g, it should be equal to %g\n",
           total, ((double)N * (N + 1)) / 2.0);

    free(a);
    free(tid);
    free(info);

    return 0;
}