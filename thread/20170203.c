#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#define WAIT_TIME 2000000
#define MAX_VALUE 200

int n;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void thread_function(void* arg){
    srand(time(NULL));
    int type = atoi((char*)arg);
    int value;
    int counter = 0, exit = 0;

    while(1){
        usleep(WAIT_TIME);
        pthread_mutex_lock(&mutex);
        
        value = rand() % MAX_VALUE +1;

        if(type == 1 && value % 2 != 0)
            value--;
        else if(type == 2 && value % 2 == 0)
            value--;
        
        n += value;
        printf("Thread%d valore di n: %d\n", type, n);
        counter++;

        
        if((type == 1 && counter > 9) && n%2 == 0)
            exit = 1;
        if((type == 2 && counter > 9) && n%2 != 0)
            exit = 1;
        if(counter > 999)
            exit = 1;

        pthread_mutex_unlock(&mutex);

        if(exit)
            break;
    }

    printf("Thread%d terminato.\n", type);
    pthread_exit(NULL);
}


int main(void){
    n = 0;
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, (void*) &thread_function, (void*) "1");
    pthread_create(&thread2, NULL, (void*) &thread_function, (void*) "2");

    pthread_detach(thread1);
    pthread_detach(thread2);

    pthread_exit(NULL);
    exit(0);
}