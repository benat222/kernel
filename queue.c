#include <stdio.h>
#include <stdlib.h>
#include "config.h"

PCB *head = NULL;
pthread_mutex_t mutex_queue = PTHREAD_MUTEX_INITIALIZER;

void queue_gehitu(PCB *p) {
    pthread_mutex_lock(&mutex_queue);
    // Añadir al principio (Más rápido y fácil de programar)
    p->next = head; 
    head = p;
    pthread_mutex_unlock(&mutex_queue);
}

void queue_remove_node(PCB *target) {
    pthread_mutex_lock(&mutex_queue);
    if (!head) { pthread_mutex_unlock(&mutex_queue); return; }
    
    if (head == target) {
        head = head->next;
    } else {
        PCB *temp = head;
        while (temp->next && temp->next != target) temp = temp->next;
        if (temp->next == target) temp->next = target->next;
    }
    pthread_mutex_unlock(&mutex_queue);
}

// HRRN: Formula (Wait + Burst) / Burst
PCB* queue_find_hrrn() {
    pthread_mutex_lock(&mutex_queue);
    PCB *best = NULL;
    double max_r = -1.0;
    PCB *curr = head;
    
    while(curr) {
        int w = global_tick - curr->arrival_tick;
        int s = curr->burst_estimation; 
        if (s <= 0) s = 1; 
        
        double ratio = (double)(w + s) / s;
        if (ratio > max_r) {
            max_r = ratio;
            best = curr;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&mutex_queue);
    return best;
}

// Equalizer: Busca el mínimo consumo
PCB* queue_find_equalizer() {
    pthread_mutex_lock(&mutex_queue);
    PCB *best = NULL;
    int min_c = 999999;
    PCB *curr = head;
    
    while(curr) {
        if (curr->ticks_consumed < min_c) {
            min_c = curr->ticks_consumed;
            best = curr;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&mutex_queue);
    return best;
}