#include "config.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Variables externas del Clock y Globales
extern pthread_mutex_t mutex_erlojua;
extern pthread_cond_t hari_cond; // Asegúrate de usar la correcta
extern int global_tick; 
extern int bukatu;

// Variables propias del Timer
pthread_mutex_t mutex_timer = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_timer_interrupt = PTHREAD_COND_INITIALIZER;

typedef struct {
    int id;         
    int maiztasuna; 
} timer_args;

void *timer_funtzioa(void *arg){
    timer_args *args = (timer_args*)arg;
    int tick_kontagailua = 0; // Variable bien declarada

    printf("Timer %d aktibatuta (%d tickero)\n", args->id, args->maiztasuna);

    while(1){
        // 1. ERLOJUAREN ZAIN EGON
        pthread_mutex_lock(&mutex_erlojua);
        pthread_cond_wait(&hari_cond, &mutex_erlojua);
        
        if(bukatu){
            pthread_mutex_unlock(&mutex_erlojua);
            break;
        }
        pthread_mutex_unlock(&mutex_erlojua);

        // 2. KUDEATU
        tick_kontagailua++; // Incremento correcto

        // AQUI ES DONDE TE DABA EL ERROR ANTES
        if(tick_kontagailua >= args->maiztasuna){ 
            
            pthread_mutex_lock(&mutex_timer);
            pthread_cond_broadcast(&cond_timer_interrupt); // Avisar al Scheduler
            pthread_mutex_unlock(&mutex_timer);
            
            tick_kontagailua = 0; 
        }
    } // Cierre del while
    
    free(args);
    return NULL;
} // Cierre de la funcion

int timer_hasi(pthread_t *haria, int id, int maiztasuna){
    timer_args *args = malloc(sizeof(timer_args));
    args->id = id;
    args->maiztasuna = maiztasuna;
    return pthread_create(haria, NULL, timer_funtzioa, args);
}