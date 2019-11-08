#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#define BUFFER_SIZE 1024



int main(int argc, char* argv[]){
    if(argc < 3){
        fprintf(stderr, "USE: %s [SERVER_IP] [PORT] [Title]\n");
        exit(0);
    } 

    int sockfd, retcode;
    struct sockaddr_in addr;
    socklen_t addrL = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("creazione socket");
        exit(-1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &(addr.sin_addr));

    if(connect(sockfd, (struct sockaddr*) &addr, addrL) == -1){
        perror("connect");
        exit(-2);
    }

    strcpy(buffer, argv[3]);
    write(sockfd, buffer, strlen(buffer));

    while((retcode = read(sockfd, buffer, addrL)) > 0){
        buffer[retcode] = '\0';
        printf("NetSixServer: %s", buffer);
    }

    close(sockfd);
    exit(0);
}