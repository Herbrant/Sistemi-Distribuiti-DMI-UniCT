#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>

unsigned int gettid(){
    return (unsigned int) syscall(SYS_gettid);
}

void print_message_fn(void* ptr){
    sleep((toupper(((char*)ptr)[0])) - 'A');
    printf("\npid/tid=%d/%d: %s\n\n", getpid(), gettid(), (char*)ptr);
    pthread_exit(NULL);
}

int main(int argc, char** argv){
    pthread_t thread1, thread2;
    char message1[] = "Ciao\n";
    char message2[100];

    if (argc == 1) {
		fprintf(stdout, "Usage %s STRING\n", argv[0]);
		fprintf(stdout, "    has \"Ciao\" and STRING printed by two distinct threads\n");
		fprintf(stdout, "    STRING[0] provides delay\n");
		exit(5);
	}

    sprintf(message2, "%s\n", argv[1]);

    pthread_create(&thread1, NULL, (void*) &print_message_fn, (void*) message1);
    pthread_create(&thread2, NULL, (void*) print_message_fn, (void*) message2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}