#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "atomic_ops.h"
#include "hrtimer_x86.h"

int cuenta;
pthread_mutex_t pthread_lock;
unsigned long tas_lock;
tatas_lock_t tatas_lock, tatas_backoff_lock;
ticket_lock_t ticket_lock;

void tas_unlock(unsigned long *l){
    *l = 0;
}

void* no_synchronization(void *p){
    for(int i = 0; i<50000; i++)
        cuenta++;
    return NULL;
}

void* pthread(void *p){
    for(int i = 0; i<50000; i++){
        pthread_mutex_lock(&pthread_lock);
        cuenta++; 
        pthread_mutex_unlock(&pthread_lock);
    }
    return NULL;
}

void* tas(void *p){
    for(int i = 0; i<50000; i++){
        tas(&tas_lock);
        cuenta++; 
        tas_unlock(&tas_lock);
    }
    return NULL;
}

void* tatas(void *p){
    for(int i = 0; i<50000; i++){
        tatas_acquire(&tatas_lock);
        cuenta++; 
        tatas_release(&tatas_lock);
    }
    return NULL;
}

void* tatas_backoff(void *p){
    for(int i = 0; i<50000; i++){
        tatas_backoff_acquire(&tatas_backoff_lock);
        cuenta++; 
        tatas_release(&tatas_backoff_lock);
    }
    return NULL;
}

void* ticket(void *p){
    for(int i = 0; i<50000; i++){
        ticket_acquire(&ticket_lock);
        cuenta++; 
        ticket_release(&ticket_lock);
    }
    return NULL;
}

int main(){
    void* (*functions[6])(void*) = {no_synchronization, pthread, tas, tatas, tatas_backoff, ticket};
    double start, end;
    for(int t = 2; t<=8; t += t){
        int rc;
        pthread_t threads[t];
        for(int f = 0; f<6; f++){
            start = gethrtime_x86();
            for(int i = 0; i<t; i++){
                rc = pthread_create(threads + i, NULL, functions[f],(void *)(i));
                if (rc){
                    printf("ERROR, codigo %d \n",rc);
                    exit(-1);
                }
            }
            for (int i=0; i<t; i++){
                rc = pthread_join(threads[i],NULL);
                if (rc){
                    printf("ERROR, codigo %d \n",rc);
                    exit(-1);
                }
            }
            end = gethrtime_x86();
            printf("%d %d %lf\n",t, f, end-start);
        }
    }
    return 0;
}
