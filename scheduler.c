#include "config.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

extern pthread_mutex_t mutex_timer; // Ojo: definelo en timer.c si no esta
extern pthread_cond_t cond_timer_interrupt;

void dispatcher() {
    
    // ==========================================================
    // POLITICA HRRN (Highest Response Ratio Next)
    // ==========================================================
    if (current_policy == HRRN) {
        // FALLO DE DISEÑO:
        // No comprobamos si el nuevo proceso tiene mejor ratio.
        // Simplemente dejamos al actual terminar. (Non-Preemptive a lo bruto).
        if (current_process != NULL) {
            if (current_process->ticks_consumed >= current_process->burst_estimation) {
                printf("   [HRRN] PID %d Fin.\n", current_process->pid);
                free(current_process);
                current_process = NULL;
            } else {
                // Aqui esta el "fallo": Ignoramos el timer.
                // printf("   [HRRN] PID %d sigue (No molestar)...\n", current_process->pid);
                return; 
            }
        }

        // Si estamos libres, buscamos
        PCB *best = queue_find_hrrn();
        if (best) {
            queue_remove_node(best);
            current_process = best;
            current_process->state = 2; // RUNNING
            
            // Restore context
            CPU_PC = current_process->pc;
            for(int i=0; i<4; i++) CPU_REGS[i] = current_process->regs[i];
            
            printf("   [HRRN] Entra PID %d (Ratio Mayor)\n", current_process->pid);
        }
        return;
    }

    // ==========================================================
    // POLITICA EQUALIZER (Fair Share + Inercia)
    // ==========================================================
    if (current_policy == EQUALIZER) {
        if (current_process != NULL) {
            // Check fin
            if (current_process->ticks_consumed >= current_process->burst_estimation) {
                printf("   [EQ] PID %d Fin.\n", current_process->pid);
                free(current_process);
                current_process = NULL;
            } 
            else {
                // FALLO DE DISEÑO:
                // Numero magico "4" hardcodeado. 
                // Esto fuerza a que un proceso corra 4 ticks si o si, bloqueando a otros.
                if (current_process->inertia_counter < 4) { 
                    printf("   [EQ] PID %d protegido por inercia (%d/4).\n", 
                           current_process->pid, current_process->inertia_counter);
                    return; // No cambiamos
                }
                
                // Si pasa de 4, lo echamos
                printf("   [EQ] PID %d pierde turno.\n", current_process->pid);
                current_process->pc = CPU_PC;
                for(int i=0; i<4; i++) current_process->regs[i] = CPU_REGS[i];
                current_process->state = 1;
                current_process->inertia_counter = 0; // Reset racha
                
                queue_gehitu(current_process);
                current_process = NULL;
            }
        }

        PCB *best = queue_find_equalizer();
        if (best) {
            queue_remove_node(best);
            current_process = best;
            current_process->state = 2;
            current_process->inertia_counter = 0; 
            
            CPU_PC = current_process->pc;
            for(int i=0; i<4; i++) CPU_REGS[i] = current_process->regs[i];
            
            printf("   [EQ] Entra PID %d (Consumo: %d)\n", current_process->pid, current_process->ticks_consumed);
        }
        return;
    }
}

void *scheduler_funtzioa(void *arg){
    while(1){
        // Esperar al timer (que salta cada X ticks)
        pthread_mutex_lock(&mutex_timer);
        pthread_cond_wait(&cond_timer_interrupt, &mutex_timer);
        pthread_mutex_unlock(&mutex_timer);
        if(bukatu) break;
        dispatcher();
    }
    return NULL;
}
int scheduler_hasi(pthread_t *h){ return pthread_create(h, NULL, scheduler_funtzioa, NULL); }