#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUFFERSIZE 1024




int main(int argc, char* argv[]){
    if(argc < 4){
        fprintf(stderr, "USE %s [IP_ADDRESS] [PORT] [nome_serie,episodio]\n", argv[0]);
        exit(-1);
    }

    int sockfd, retcode;
    struct sockaddr_in addr;
    socklen_t addrL = sizeof(struct sockaddr_in);
    char buffer[BUFFERSIZE];

    //Creazione socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(-2);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &(addr.sin_addr));

    if(connect(sockfd, (struct sockaddr*) &addr, addrL) != -1){
        write(sockfd, argv[3], strlen(argv[3]));

        if(( retcode = read(sockfd, buffer, BUFFERSIZE)) > 0){
            buffer[retcode-1] = '\0';
            printf("Server: %s\n", buffer);
        }
        else
            fprintf(stderr, "Errore nella read\n");
    } 
    printf("\n");
    close(sockfd);
}