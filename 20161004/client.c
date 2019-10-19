#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define BUFFERSIZE 1024



int main(int argc, char* argv[]){
    if(argc < 3){
        fprintf(stderr, "USE: %s [SERVER_IP] [PORT] [MESSGE]\n", argv[0]);
    }
    int sockfd, retcode;
    struct sockaddr_in addr;
    socklen_t addrL = sizeof(struct sockaddr_in);
    char buffer[BUFFERSIZE], msg[BUFFERSIZE];

    //Creazione socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("creazione socket");
        exit(-1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &(addr.sin_addr));
    
    if(connect(sockfd, (struct sockaddr*) &addr, addrL) == -1){
            perror("connessione");
            exit(-2);
        }

        
    write(sockfd, argv[3], BUFFERSIZE);

    if((retcode = read(sockfd, msg, BUFFERSIZE-1)) > 0){
        printf("\t>>> %s\n", msg);
    }
    
    close(sockfd);
    exit(0);
}
