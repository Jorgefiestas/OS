#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "atomic_ops.h"
#include "macthread.h"
#include "hrtimer_x86.h"

int sense = 0;
int P = 0;
int count = 0;
tatas_lock_t lock;
pthread_barrier_t barr;
int ac, lc, flag;

void centralized_barrier(){
    int b = 64;
    tatas_acquire(&lock);
    if(ac == 0){
        if(lc == P) flag = 0;
        else{
            tatas_release(&lock);
            while(lc != P) backoff(&b);
            tatas_acquire(&lock);
            flag = 0;
        }
    }
    int arrived = ++ac;
    tatas_release(&lock);
    if(arrived == P){
        ac = 0;
        lc = 1;
        flag = 1;
    }
    else{
        while(flag == 0) backoff(&b);
        tatas_acquire(&lock);
        lc++;
        tatas_release(&lock);
    }
}

void sense_reversing_barrier(){
    int l_sense = 1 - sense;
    tatas_acquire(&lock);
    count--;
    if(count == 0){
        count = P;
        sense = l_sense;
    }
    tatas_release(&lock);
    int b = 64;
    while(sense != l_sense){
        backoff(&b);
    }
}

void* parallel_ptb(void* p){
    long n = (long) p;
    for(int i = 0; i<n; i++){
        pthread_barrier_wait(&barr);
    }
    return NULL;
}

void* parallel_cb(void* p){
    long n = (long) p;
    for(int i = 0; i<n; i++){
        centralized_barrier();
    }
    return NULL;
}

void* parallel_srb(void* p){
    long n = (long) p;
    for(int i = 0; i<n; i++){
        sense_reversing_barrier();
    }
    return NULL;
}

int main(){
    long num[4] = {10, 100, 1000, 10000};
    double start, end;
    for(int j = 0; j<4; j++){
        for(int t = 2; t<=8; t+=t){
            P = t;
            count = t;
            ac = 0;
            lc = t;
            int rc;
            pthread_t threads[t];
            pthread_barrier_init(&barr, NULL, t);
            start = gethrtime_x86();
            for(int i = 0; i<t; i++){
                rc = pthread_create(threads + i, NULL, parallel_ptb,(void *)(num[j]));
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
            printf("pthread_barrier\t\t %d threads %ld calls %lf sec\n",t, num[j], end-start);
            start = gethrtime_x86();
            for(int i = 0; i<t; i++){
                rc = pthread_create(threads + i, NULL, parallel_cb,(void *)(num[j]));
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
            printf("centralized_barrier\t %d threads %ld calls %lf sec\n",t, num[j], end-start);
            start = gethrtime_x86();
            for(int i = 0; i<t; i++){
                rc = pthread_create(threads + i, NULL, parallel_srb,(void *)(num[j]));
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
            printf("sense reverse barrier\t %d threads %ld calls %lf sec\n",t, num[j], end-start);
        }
    }
    return 0;
}
