#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int m;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t m1_5 = PTHREAD_COND_INITIALIZER;
pthread_cond_t m6_10 = PTHREAD_COND_INITIALIZER;

void producerFunction(void* argv){

    while(1){
        if(strcmp((char*)argv, "1") == 0){
            pthread_mutex_lock(&mutex);

            if(m >= 0 && m <= 5){
                pthread_cond_signal(&m1_5);
                m = rand()%10 +1;
                printf("Thread%s imposta m = %d\n", (char*)argv, m);
            }
            else
                pthread_cond_wait(&m6_10, &mutex);

            pthread_mutex_unlock(&mutex);
        }
        else{
            pthread_mutex_lock(&mutex);

            if(m >= 6 && m <= 10){
                pthread_cond_signal(&m6_10);
                m = rand() % 10 + 1;
                printf("Thread%s imposta m = %d\n", (char*)argv, m);
            }
            else
                pthread_cond_wait(&m1_5, &mutex);

            pthread_mutex_unlock(&mutex);
        }
    }
}

int main(void){
    srand(time(NULL));
    m = rand()%11 + 1;

    pthread_t thread1, thread2;
    

    pthread_create(&thread1, NULL, (void*) &producerFunction, (void*) "1");
    pthread_create(&thread2, NULL, (void*) &producerFunction, (void*) "2");

    pthread_detach(thread1);
    pthread_detach(thread2);
    pthread_exit(NULL);
    exit(0);
}