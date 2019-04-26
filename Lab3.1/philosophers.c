#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

int chopsticks = 5;
pthread_mutex_t lock, lock1, lock2;
pthread_barrier_t barr;
int t = 0, finished = 0;
int ate = 0;

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

        if(mychop == 2){
            printf("El filosofo %d esta comiendo\n", i);
            rolls--;
            pthread_mutex_lock(&lock);
            ate = 1;
            pthread_mutex_unlock(&lock);
        }
        else printf("El filosofo %d esta hablando.\n", i);
        pthread_barrier_wait(&barr);
        if(i == 1){
            chopsticks = 5;
            if(ate) t += 4;
            ate = 0;
        }
        mychop = 0;
        pthread_barrier_wait(&barr);
    }
    
    pthread_mutex_lock(&lock2);
    finished++;
    pthread_mutex_unlock(&lock2);

    do{
        pthread_barrier_wait(&barr);
    }while(finished < 5);

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
    int num[] ={1,2,3,4,5};

	for(int i = 0; i<5; i++){
		rc = pthread_create(threads + i, NULL, philosophers,(void *)(num+i));
		if (rc){
			printf("ERROR, codigo %d \n",rc);
 			exit(-1);
 		}
	}

    for (int i=0; i<5; i++){
		rc = pthread_join(threads[i],NULL);
		if (rc){
			printf("ERROR, codigo %d \n",rc);
		 	exit(-1);
		}
	}

    printf("Tomaron %d segundos en terminar de comer.\n", t);
    
    return 0;
}
