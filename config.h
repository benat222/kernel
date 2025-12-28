#ifndef CONFIG_H
#define CONFIG_H

#include <pthread.h>

// --- POLITIKAK ---
// Usamos un enum simple
typedef enum {
    HRRN,
    EQUALIZER
} PolicyType;

// Variable global para que el scheduler sepa que hacer
extern PolicyType current_policy; 

// --- PCB (Con los campos necesarios para los calculos) ---
typedef struct PCB {
    int pid;
    int state;      
    int pc;         
    int regs[4];    

    // Datos estadísticos
    int arrival_tick;     // Hora de llegada
    int burst_estimation; // Duracion total estimada
    int ticks_consumed;   // Cuanto lleva
    int inertia_counter;  // Para la 'inercia' del Equalizer

    struct PCB *next; 
} PCB;

// Globales
extern int bukatu; 
extern int global_tick; 
extern PCB *current_process; 
extern int CPU_PC;
extern int CPU_REGS[4];

// Mutex del clock (necesario para el wait)
extern pthread_mutex_t mutex_erlojua;
extern pthread_cond_t hari_cond; // Ojo: tu clock usa cond_erlojua o hari_cond? Unificamos a hari_cond

// Funtzioak
void queue_gehitu(PCB *p);
PCB* queue_find_hrrn();
PCB* queue_find_equalizer();
void queue_remove_node(PCB *target);

int scheduler_hasi(pthread_t *h);
int generator_hasi(pthread_t *h);
int timer_hasi(pthread_t *h, int id, int m);
int clock_hasi(pthread_t *h);
int cpu_hasi(pthread_t *h);

// Funcion para configurar el clock desde el main (el parche)
void clock_konfiguratu(int maiztasuna, int max_ticks);
void clock_itxaron(pthread_t h);

#endif