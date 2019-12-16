#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int value;
int state = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reader_s = PTHREAD_COND_INITIALIZER;
pthread_cond_t writer_s = PTHREAD_COND_INITIALIZER;


void Reader(void* arg){
    FILE* f = (FILE*) arg;
    char c;

    while((c = fgetc(f)) != EOF){
        pthread_mutex_lock(&mutex);
        while(state)
            pthread_cond_wait(&reader_s, &mutex);
        
        value = (int) c;

        state = !state;
        pthread_cond_signal(&writer_s);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    while(state)
        pthread_cond_wait(&reader_s, &mutex);
    
    value = (int) EOF;
    
    state = !state;
    pthread_cond_signal(&writer_s);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void Writer(void* arg){
    char c;
    while(1){
        pthread_mutex_lock(&mutex);

        while(!state)
            pthread_cond_wait(&writer_s, &mutex);
        
        if((char)value == EOF)
            break;
        c = value;
        
        printf("%c\n", c);
        
        state = !state;
        pthread_cond_signal(&reader_s);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}


int main(int argc, char** argv){
    if(argc < 2){
        fprintf(stderr, "USE %s [FILE]\n", argv[0]);
        exit(-1);
    }

    pthread_t reader, writer;
    FILE* f;
    if((f = fopen(argv[1], "r")) == NULL){
        perror("fopen");
        exit(-1);
    }
    
    pthread_create(&reader, NULL, (void*) &Reader, (void*)f);
    pthread_create(&writer, NULL, (void*) &Writer, NULL);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&writer_s);
    pthread_cond_destroy(&reader_s);
}