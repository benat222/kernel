#include "config.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// ==================== EXTERNOAK ====================
// Clock
extern void denb_zehaztu(int argc, char *argv[]);
extern int clock_hasi(pthread_t *erloju_haria);
extern void clock_itxaron(pthread_t erloju_haria);
extern int clock_denbora_totala();

// Timer
extern int timer_hasi(pthread_t *timer_haria, int freq, int max_ticks);
extern void timer_itxaron(pthread_t timer_haria);

// ==================== PROGRAMA NAGUSIA ====================
int main(int argc, char *argv[]) {
    printf("CLOCK + TIMER SISTEMA - HASIERA\n");

    // --- Configurar y arrancar el clock ---
    denb_zehaztu(argc, argv);

    pthread_t clock_haria;
    if(clock_hasi(&clock_haria) != 0){
        return 1;
    }
    printf("Clock martxan...\n");

    // --- Timer 1 ---
    pthread_t timer1_haria;
    int freq_timer1 = 5;
    int max_ticks_timer1 = 5;
    printf("Timer 1 martxan...\n");
    timer_hasi(&timer1_haria, freq_timer1, max_ticks_timer1);
    timer_itxaron(timer1_haria);
    printf("Timer 1 amaitu da.\n");

    // --- Timer 2 ---
    pthread_t timer2_haria;
    int freq_timer2 = 5;
    int max_ticks_timer2 = 5;
    printf("Timer 2 martxan...\n");
    timer_hasi(&timer2_haria, freq_timer2, max_ticks_timer2);
    timer_itxaron(timer2_haria);
    printf("Timer 2 amaitu da.\n");

    // --- Esperar a que termine el clock ---
    clock_itxaron(clock_haria);
    printf("Clock amaitu da.\n");

    // --- Estado final ---
    printf("\nPROGRAMA AMAITUTA\n");
    printf("Denbora totala: %d tick\n", clock_denbora_totala());

    return 0;
}