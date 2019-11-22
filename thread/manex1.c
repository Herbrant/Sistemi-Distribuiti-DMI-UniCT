//Man example 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#define MAX_THREAD 7

int x, y;
int state[MAX_THREAD+1];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t xGTy = PTHREAD_COND_INITIALIZER;

void initializeIndx(int vett[]){
    for(int i = 0; i < MAX_THREAD+1; i++)
        vett[i] = i;
}

void threadPrint(int thrdindx, char * msg){
    int i;

    for (i = 0; i < thrdindx; i++)
	    printf("%c        ", state[i]);
    printf("%s", msg);
	for (i = thrdindx+1; i < MAX_THREAD+1; i++)
	    printf("%c        ", state[i]);
	printf("\n");
}

void producer(void* arg){
    state[0] = '.'; //In esecuzione

    while(1){
        sleep(rand() % (MAX_THREAD + 2) + 1);

        pthread_mutex_lock(&mutex);
        threadPrint(0, "[        ");
        x = rand();
        y = rand();

        if(x > y){
            threadPrint(0, "xGTy-> ");
            pthread_cond_broadcast(&xGTy);
        }else{
            threadPrint(0, "xLEy-> ");
            threadPrint(0, "]        ");
            pthread_mutex_unlock(&mutex);
        }
    }
}

void consumer(void* arg){
    state[*((int*)arg)] = '.'; //In esecuzione
    int n = *((int*)arg);

    while(1){
        pthread_mutex_lock(&mutex);
        threadPrint(n, "[        ");

        while(x <= y){
            threadPrint(n, "]w       ");
            state[n] = 'w';
            pthread_cond_wait(&xGTy, &mutex);
            state[n] = '.';
            threadPrint(n, "[        ");
        }

        threadPrint(n, "->xGTy   ");
        threadPrint(n, "]        ");
        pthread_mutex_unlock(&mutex);

        sleep(rand() % n + n);
    }
}

int main(){
    pthread_t thread[MAX_THREAD+1];
    int i;
    int indx[MAX_THREAD+1];

    srand(time(NULL));
    initializeIndx(indx);

    //Creo il thread produttore
    pthread_create(&thread[0], NULL, (void*) &producer, (void*) &indx[0]);
    
    //Creo i consumatori
    for(int i = 1; i < MAX_THREAD+1; i++)
        pthread_create(&thread[i], NULL, (void*) &consumer, (void*) &indx[i]);

    pthread_exit(NULL);
}