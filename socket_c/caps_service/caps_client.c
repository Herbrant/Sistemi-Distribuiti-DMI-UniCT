#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 1024


int main(int argc, char* argv[]){
    int sockfd, retcode;
    struct sockaddr_in addr;
    socklen_t addrL = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];

    //Check args
    if(argc < 3){
        fprintf(stderr, "USE: %s [SERVER_IP] [PORT] [MESSAGE]\n", argv[0]);
        exit(-1);
    }

    //Socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("creating socket");
        exit(-1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &(addr.sin_addr));

    //Connection
    if(connect(sockfd, (struct sockaddr*) &addr, addrL) == -1){
        perror("connecting");
        exit(-3);
    }

    sprintf(buffer, argv[3], BUFFER_SIZE);

    if(write(sockfd, buffer, BUFFER_SIZE) == -1){
        perror("write to socket");
        exit(-4);
    }

    while((retcode = read(sockfd, buffer, BUFFER_SIZE-1)) != 0){
        if(retcode == -1){
            perror("accept");
            exit(-5);
        }

        buffer[retcode] = '\0';
        printf("Server answer: %s\n", buffer);
    }

    printf("\n>>>Exiting: server closed connection\n");
    close(sockfd);
    exit(0);
}
