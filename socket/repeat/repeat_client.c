#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFFER_SIZE 1024



int main(int argc, char* argv[]){
    int s, retcode;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE], msg[BUFFER_SIZE];

    if(argc < 3){
        fprintf(stderr, "USE: %s [server_ip] [port] [message]\n");
        exit(-1);
    }

    if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("opening socket");
        exit(-2);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(s, (struct sockaddr*) &addr, sizeof(addr))){
        perror("error connecting");
        exit(-3);
    }

    sprintf(buffer, "%s\n", argv[3]);
    if(write(s, buffer, BUFFER_SIZE) == -1){
        perror("writing to socket");
        exit(-1);
    }

    while((retcode = read(s, buffer, BUFFER_SIZE-1)) != 0){
        if(retcode == -1){
            perror("reading from socket");
            exit(-4);
        }

        buffer[retcode] = '\0';
        printf("\t Server received: %s\n", buffer);
    }

    printf("\n>>>Exiting: server closed connection\n");
    close(s);
    exit(0);
}
