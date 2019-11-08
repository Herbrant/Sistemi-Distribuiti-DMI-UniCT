#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define PORT 3333
#define LOCAL_ADDR "127.0.0.1"
#define MAXQ 80
#define BUFFERSIZE 1024

char* AND(char* buffer){
    for(int i = 0; i < strlen(buffer); i++)
        if(buffer[i] != 'V')
            return "FALSO";
    
    return "VERO";
}

int main(){
    int sockfd, s1, retcode;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    localAddrL = farAddrL = sizeof(struct sockaddr_in);
    char buffer[BUFFERSIZE], *msg;

    //Creazione socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("creazione socket");
        exit(-1);
    }

    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, LOCAL_ADDR, &(localAddr.sin_addr));


    //Binding
    if(bind(sockfd, (struct sockaddr*) &localAddr, localAddrL) == -1){
        perror("binding");
        exit(-2);
    }

    //Listen
    listen(sockfd, MAXQ);

    while((s1 = accept(sockfd, (struct sockaddr*) &farAddr, &farAddrL)) != -1){
        printf("Connected %s:%d\n",
            inet_ntoa(farAddr.sin_addr),
            ntohs(farAddr.sin_port));
    
        if((retcode = read(s1, buffer, BUFFERSIZE)) > 0){
            buffer[retcode-1] = '\0';
            printf("Client asks %s\n", buffer);
        }

        msg = AND(buffer); 
        write(s1, msg, strlen(msg));
        
        printf("Connection closed.\n");
        close(s1);
    }

    close(sockfd);
    exit(0);
}