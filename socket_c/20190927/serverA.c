#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 9999
#define LOCAL_IP "127.0.0.1"
#define BUFFERSIZE 1024
#define MAXQ 80

int main(){
    int sockfd, s1, retcode;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    localAddrL = farAddrL = sizeof(struct sockaddr_in);
    char buffer[BUFFERSIZE], msg[BUFFERSIZE];

    //Creazione socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("creazione socket");
        exit(-1);
    }

    //Preparazione indirizzo localAddr
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, LOCAL_IP, &(localAddr.sin_addr));

    //Binding
    if(bind(sockfd, (struct sockaddr*) &localAddr, localAddrL) == -1){
        perror("binding");
        exit(-2);
    }

    //Listen
    listen(sockfd, MAXQ);

    while((s1 = accept(sockfd, (struct sockaddr*) &farAddr, &farAddrL)) != -1){
        printf("Client %s:%d connected.\n",
            inet_ntoa(farAddr.sin_addr),
            ntohs(farAddr.sin_port));

        while((retcode = read(s1, buffer, BUFFERSIZE)) > 0){
            buffer[retcode-1] = '\0';
            printf("Client asks %s\n", buffer);

        }
        close(s1);
    }
    
    close(sockfd);
    exit(0);
}