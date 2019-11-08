#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUFFERSIZE 1024



int main(int argc, char** argv){
    int sockfd, retcode;
    struct sockaddr_in addr;
    socklen_t addrL = sizeof(struct sockaddr_in);
    char buffer[BUFFERSIZE];

    if(argc < 4){
        fprintf(stderr, "USE: %s [SERVER_IP] [SERVER_PORT] [NUMBER]\n", argv[0]);
        exit(0);
    }

    //Creazione socket
    if((sockfd = socket(PF_INET, SOCK_STREAM , 0)) == -1){
        perror("creazione socket");
        exit(-1);
    }

    //Preparazione addr
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &(addr.sin_addr));

    //Connessione
    if(connect(sockfd, (struct sockaddr*) &addr, addrL) == -1){
        perror("connect");
        exit(-2);
    }

    write(sockfd, argv[3], strlen(argv[3]));

    while((retcode = read(sockfd, buffer, BUFFERSIZE-1)) > 0){
        buffer[retcode-1] = '\0';
        printf("Server: %s\n", buffer);
    }
    
    close(sockfd);
    exit(0);
}