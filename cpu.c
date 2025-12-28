#include "config.h"
#include <pthread.h>
#include <stdio.h>

// Variables externas del Clock
extern pthread_mutex_t mutex_erlojua;
extern pthread_cond_t hari_cond;
extern int bukatu;

// Variables globales del sistema
extern PCB *current_process;
extern int CPU_PC;
extern int CPU_REGS[4];

void *cpu_funtzioa(void *arg){
    while(1){
        // 1. Esperar al Clock
        pthread_mutex_lock(&mutex_erlojua);
        pthread_cond_wait(&hari_cond, &mutex_erlojua); 
        pthread_mutex_unlock(&mutex_erlojua);

        if(bukatu) break;

        // 2. Ejecutar ciclo si hay proceso
        if (current_process != NULL) {
            CPU_PC++;
            CPU_REGS[0]++;
            
            // Actualizar estadísticas del proceso
            current_process->ticks_consumed++;
            current_process->inertia_counter++; // Aquí se cortaba antes tu código
            
        } // Cierre del if (current_process)
    } // Cierre del while
    return NULL;
} // Cierre de la funcion

int cpu_hasi(pthread_t *h){ 
    return pthread_create(h, NULL, cpu_funtzioa, NULL); 
}