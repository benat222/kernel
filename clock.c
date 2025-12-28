#include "config.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Definimos las variables globales AQUI
pthread_mutex_t mutex_erlojua = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hari_cond = PTHREAD_COND_INITIALIZER;

int global_tick = 0; // Usamos este nombre para unificar
int ITXAROTE_DENB = 1; 
int denb_max = 60;     
int bukatu = 0; // Se define aqui

// Funcion chapuza para recibir datos del main
void clock_konfiguratu(int maiztasuna, int max_ticks){
    // PARCHE DE ESTUDIANTE:
    // Si me pasan un 0 (porque atoi falló con texto), pongo 1 para no dividir por cero.
    if(maiztasuna <= 0) {
        ITXAROTE_DENB = 1; 
    } else {
        ITXAROTE_DENB = maiztasuna;
    }
    
    if(max_ticks > 0) denb_max = max_ticks;
}

void *erloju_funtzioa(void *arg){
    printf("Clock hasieratzen... (Frec: %d)\n", ITXAROTE_DENB);
    
    while(1){
        // AQUI ESTABA EL FALLO: Si ITXAROTE_DENB era 0, petaba.
        // Con el parche de arriba ya no pasa.
        usleep(1000000 / ITXAROTE_DENB);

        pthread_mutex_lock(&mutex_erlojua);
        
        global_tick++;
        // printf("################ Tick %d ################\n", global_tick);
        
        if(denb_max != -1 && global_tick >= denb_max){
            bukatu = 1;
        }

        pthread_cond_broadcast(&hari_cond); // Despertar a todos
        pthread_mutex_unlock(&mutex_erlojua);

        if(bukatu){
            printf("[CLOCK] Amaitu da.\n");
            break;
        }
    }
    return NULL;
}

int clock_hasi(pthread_t *haria){
    return pthread_create(haria, NULL, erloju_funtzioa, NULL);
}

void clock_itxaron(pthread_t haria){
    pthread_join(haria, NULL);
}