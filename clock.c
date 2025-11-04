#include "config.h"
#include <pthread.h>

pthread_mutex_t mutex_erlojua = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hari_cond = PTHREAD_COND_INITIALIZER;

#define denb_tot = 0;
#define ITXAROTE_DENB = 1;

if (argv[0]!=NULL){
    ITXAROTE_DENB = argv[0];
}

void *erlojua (void *arg){
    while(1){
        sleep(ITXAROTE_DENB); //itxoin segundu batez
        pthread_mutex_lock(&mutex_erlojua);
        denb_tot++;
        printf("(Erlojuan) tick %d\n", denb_tot);
        pthread_cond_broadcast(&hari_cond);
        pthread_mutex_unlock(&mutex_erlojua);
    }

}