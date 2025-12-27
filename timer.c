#include "config.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex_timer = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_timer = PTHREAD_COND_INITIALIZER;

static int timer_stop_flag = 0;

// ----------------- HILO DEL TIMER -----------------
typedef struct {
    int freq;       // ticks por segundo
    int max_ticks;  // máximo de ticks
} t_arg;

static void *timer_thread(void *arg) {
    t_arg *a = (t_arg*)arg;
    int tick = 0;
    int usleep_time = 1000000 / a->freq;

    while (!timer_stop_flag && tick < a->max_ticks) {
        usleep(usleep_time);

        tick++;
        printf("Timer tick %d\n", tick);

        // Avisar a hilos que esperan (si es necesario)
        pthread_mutex_lock(&mutex_timer);
        pthread_cond_broadcast(&cond_timer);
        pthread_mutex_unlock(&mutex_timer);
    }

    free(a); // liberar memoria
    return NULL;
}

// ----------------- API DEL TIMER -----------------
int timer_hasi(pthread_t *timer_haria, int freq, int max_ticks) {
    t_arg *arg = malloc(sizeof(t_arg));
    if (!arg) return -1;

    arg->freq = freq;
    arg->max_ticks = max_ticks;

    return pthread_create(timer_haria, NULL, timer_thread, arg);
}

void timer_stop() {
    pthread_mutex_lock(&mutex_timer);
    timer_stop_flag = 1;
    pthread_cond_broadcast(&cond_timer);
    pthread_mutex_unlock(&mutex_timer);
}

void timer_itxaron(pthread_t timer_haria) {
    pthread_join(timer_haria, NULL);
}
