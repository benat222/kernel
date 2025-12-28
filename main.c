#include "config.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> 

// Variables globales externas
PCB *current_process = NULL;
int CPU_PC = 0;
int CPU_REGS[4] = {0};
PolicyType current_policy = HRRN; // Defecto

int main(int argc, char *argv[]) {
    printf("KERNEL SIMULATZAILEA - Fase 2\n");

    int clock_hz = 1;       
    int max_ticks = 60;     
    
    // --- LECTURA DE ARGUMENTOS (Estilo libre) ---
    // Recorremos los argumentos buscando palabras clave o numeros
    for(int i=1; i<argc; i++) {
        
        // Comprobamos si es la politica
        if (strcasecmp(argv[i], "equ") == 0) {
            current_policy = EQUALIZER;
            printf(" -> Politika aldatuta: EQUALIZER (equ)\n");
        }
        else if (strcasecmp(argv[i], "HRRN") == 0) {
            current_policy = HRRN;
            printf(" -> Politika aldatuta: HRRN\n");
        }
        else {
            // Si no es texto, asumimos que es un numero (Hz)
            int val = atoi(argv[i]);
            if (val > 0) {
                clock_hz = val;
                printf(" -> Maiztasuna: %d Hz\n", clock_hz);
            }
            // Si atoi devuelve 0 (porque era texto basura), el parche del clock lo arreglará
        }
    }

    // Configurar clock
    clock_konfiguratu(clock_hz, max_ticks);

    pthread_t clock_th, timer1_th, gen_th, sched_th, cpu_th;

    // Arrancar hilos
    scheduler_hasi(&sched_th);
    generator_hasi(&gen_th);
    cpu_hasi(&cpu_th);
    
    // Timer para interrumpir cada 3 ticks (aunque HRRN lo ignore por el fallo)
    timer_hasi(&timer1_th, 1, 3); 
    
    clock_hasi(&clock_th);

    clock_itxaron(clock_th);

    // Salida sucia
    return 0;
}