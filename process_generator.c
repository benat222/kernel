#include "config.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int global_tick;

void *generator_funtzioa(void *arg){
    int id = 1;
    srand(time(NULL));

    while(1){
        usleep(2500000); 
        if(bukatu) break;

        PCB *nuevo = malloc(sizeof(PCB));
        nuevo->pid = id++;
        nuevo->state = 1; 
        nuevo->pc = 0;
        
        nuevo->arrival_tick = global_tick;
        // Burst aleatorio
        nuevo->burst_estimation = (rand() % 12) + 4; // Entre 4 y 16
        nuevo->ticks_consumed = 0;
        nuevo->inertia_counter = 0;

        printf("[GEN] Nuevo Proceso PID %d (Estimacion: %d ticks)\n", nuevo->pid, nuevo->burst_estimation);
        queue_gehitu(nuevo);
    }
    return NULL;
}
int generator_hasi(pthread_t *h){ return pthread_create(h, NULL, generator_funtzioa, NULL); }