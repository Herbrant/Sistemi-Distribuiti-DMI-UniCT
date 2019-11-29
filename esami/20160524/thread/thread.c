#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int val = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void thread_function(void* arg){
    int counter = 0;
    while(1){
        usleep(rand() % 10000);
        pthread_mutex_lock(&mutex);

        if(val > 300){
            pthread_mutex_unlock(&mutex);
            printf("THREAD %s: %d\n",(char*)arg, counter);
            pthread_exit(NULL);
        }else{
            val++;
            counter++;
        }

        pthread_mutex_unlock(&mutex);
    }
}

int main(){
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, (void*) &thread_function, (void*) "1");
    pthread_create(&thread2, NULL, (void*) &thread_function, (void*)"2");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

}