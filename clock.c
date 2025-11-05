#include "config.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex_erlojua = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hari_cond = PTHREAD_COND_INITIALIZER;

int denb_tot = 0;
int ITXAROTE_DENB = 1;
int denb_max = -1;
int bukatu = 0;

void denb_zehaztu (int argc, char *argv[]){
    if(argc > 1){//tick maiztasuna
        ITXAROTE_DENB = atoi(argv[1]);
    }
    if(argc > 2){//denbora maximoa
        denb_max = atoi(argv[2]);
    }

}
//x tick segundura aldatu. jarri zuzenean h-n o bertzela eskuz sartu
void *erlojua (void *arg){
    while(1){
        //aldatu sleep-> usleep
        usleep(1000000/ITXAROTE_DENB); //itxoin zehazatatutako maiztasunakin
        pthread_mutex_lock(&mutex_erlojua);//mutex-a blokeatu
        denb_tot+=ITXAROTE_DENB;
        printf("(Erlojuan) tick %d\n", denb_tot);
        if(denb_tot >= denb_max){ //denbora maximoa pasa bada
            bukatu = 1;
        }
        pthread_cond_broadcast(&hari_cond);//hariak esnatu
        pthread_mutex_unlock(&mutex_erlojua);//mutex-a desblokeatu
        if(bukatu){
            break;
        }
    }
return NULL;
}

int main(int argc, char *argv[]) {
    
    denb_zehaztu(argc, argv);

    pthread_t erloju_haria;
    int ondo_joan_da = pthread_create(&erloju_haria, NULL, erlojua, NULL);
    if(ondo_joan_da == 0){
        printf("Erloju hariak hasiberri da \n");
    }else{
        printf("Errore bat gauzatu da %d\n", ondo_joan_da);
    }
    pthread_join(erloju_haria, NULL);
    printf("Erloju hariak bukatu du \n");
}