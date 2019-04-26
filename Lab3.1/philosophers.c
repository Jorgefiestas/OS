#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

int chopsticks = 5;
pthread_mutex_t lock;
pthread_barrier_t barr;
int t = 0;

void* philosophers(void* integer){
    int i = *(int*)integer;
    int rolls = 20;
    int mychop = 0;

    while(rolls){
        pthread_mutex_lock(&lock);
        if(chopsticks){
            chopsticks--;
            mychop++;
        }
        pthread_mutex_unlock(&lock);

        pthread_mutex_lock(&lock);
        if(chopsticks){
            chopsticks--;
            mychop++;
        }
        pthread_mutex_unlock(&lock);

        if(mychop == 2) printf("El filosofo %d esta comiendo\n", i);
        else printf("El filosofo %d esta hablando.\n", i);
        pthread_barrier_wait(&barr);
    }
    return (void*)&chopsticks;
}

int main(){
	int rc;
	pthread_t threads[5];
    struct timeval start, end;

    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init failed\n");
        return 1;
    }

    pthread_barrier_init(&barr, NULL,5);

	gettimeofday(&start, NULL);
	for(int i = 0; i<5; i++){
		rc = pthread_create(threads + i, NULL, philosophers,(void *)&i);
		if (rc){
			printf("ERROR, codigo %d \n",rc);
 			exit(-1);
 		}
	}
    gettimeofday(&end, NULL);

    for (int i=0; i<t; i++){
		rc = pthread_join(threads[i],NULL);
		if (rc){
			printf("ERROR, codigo %d \n",rc);
		 	exit(-1);
		}
	}

    printf("finish\n");
    
    return 0;
}
