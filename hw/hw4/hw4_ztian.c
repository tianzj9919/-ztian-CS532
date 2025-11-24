/*
 * Name: Zhenjiang Tian (BlazerID: ztian)
 * Assignment: hw4
 * To compile: gcc -Wall -Wextra -O2 -std=c11 hw4_ztian.c -lpthread -o hw4 / make
 * To run:     ./hw4 > result.txt
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

/* ---- constants ---- */
#define NUM_PRODUCERS       10
#define NUM_CONSUMERS       20
#define NUM_PER_PRODUCER    500
#define NUM_PER_CONSUMER    250

int pipefd[2];

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t pipe_mutex  = PTHREAD_MUTEX_INITIALIZER;


int write_fd;
int read_fd;
volatile sig_atomic_t start_reading = 0;

long long consumer_sums[NUM_CONSUMERS];

static void on_parent_done(int sig) {
    (void)sig;
    start_reading = 1;
}

void *producer_thread(void *arg) {
    long tid = (long)arg;
    unsigned int seed = (unsigned int)(time(NULL) ^ (tid * 123457u) ^ getpid());

    int produced = 0;
    int used[1001] = {0};

    while (produced < NUM_PER_PRODUCER) {
        int value = rand_r(&seed) % 1001;
        if (used[value]) {
            continue;
        }
        used[value] = 1;
        produced++;

        pthread_mutex_lock(&pipe_mutex);
        ssize_t n = write(write_fd, &value, sizeof(int));
        pthread_mutex_unlock(&pipe_mutex);


        if (n != sizeof(int)) {
            pthread_mutex_lock(&print_mutex);
            fprintf(stderr, "Producer %ld: write error: %s\n", tid, strerror(errno));
            pthread_mutex_unlock(&print_mutex);
            pthread_exit(NULL);
        }

        if (produced % 50 == 0 || produced == NUM_PER_PRODUCER) {
            pthread_mutex_lock(&print_mutex);
            printf("Producer %ld: produced %d/%d numbers\n",
                   tid, produced, NUM_PER_PRODUCER);
            fflush(stdout);
            pthread_mutex_unlock(&print_mutex);
        }
    }

    pthread_mutex_lock(&print_mutex);
    printf("Producer %ld finished.\n", tid);
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);

    return NULL;
}

void *consumer_thread(void *arg) {
    long tid = (long)arg;
    long long sum = 0;

    for (int i = 0; i < NUM_PER_CONSUMER; i++) {
        int value;
        ssize_t n = read(read_fd, &value, sizeof(int));
        if (n == 0) {
            pthread_mutex_lock(&print_mutex);
            fprintf(stderr, "Consumer %ld: unexpected EOF after %d numbers\n", tid, i);
            pthread_mutex_unlock(&print_mutex);
            break;
        } else if (n < 0) {
            pthread_mutex_lock(&print_mutex);
            fprintf(stderr, "Consumer %ld: read error: %s\n", tid, strerror(errno));
            pthread_mutex_unlock(&print_mutex);
            break;
        }
        sum += value;
    }

    consumer_sums[tid] = sum;

    pthread_mutex_lock(&print_mutex);
    printf("Consumer %ld finished, sum = %lld\n", tid, sum);
    fflush(stdout);
    pthread_mutex_unlock(&print_mutex);

    return NULL;
}

int main(void) {
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        close(pipefd[0]);
        write_fd = pipefd[1];


        pthread_t producers[NUM_PRODUCERS];

        for (long i = 0; i < NUM_PRODUCERS; i++) {
            if (pthread_create(&producers[i], NULL, producer_thread, (void *)i) != 0) {
                perror("pthread_create (producer)");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < NUM_PRODUCERS; i++) {
            pthread_join(producers[i], NULL);
        }

        close(write_fd);

        kill(pid, SIGUSR1);

        int status;
        waitpid(pid, &status, 0);

        return 0;

    } else {
        close(pipefd[1]);
        read_fd = pipefd[0];

        struct sigaction sa;
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = on_parent_done;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        if (sigaction(SIGUSR1, &sa, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        while (!start_reading) {
            pause();
        }

        pthread_t consumers[NUM_CONSUMERS];

        for (long i = 0; i < NUM_CONSUMERS; i++) {
            consumer_sums[i] = 0;
            if (pthread_create(&consumers[i], NULL, consumer_thread, (void *)i) != 0) {
                perror("pthread_create (consumer)");
                exit(EXIT_FAILURE);
            }
        }

        for (int i = 0; i < NUM_CONSUMERS; i++) {
            pthread_join(consumers[i], NULL);
        }

        long long total = 0;
        for (int i = 0; i < NUM_CONSUMERS; i++) {
            total += consumer_sums[i];
        }

        double average = (double)total / NUM_CONSUMERS;

        printf("Average of consumer sums = %.2f\n", average);

        close(read_fd);

        return 0;
    }
}

