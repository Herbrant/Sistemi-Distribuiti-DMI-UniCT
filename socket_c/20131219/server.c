#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define MAXQ 80
#define PORT 7777
#define BUFFER_SIZE 1024

int main(){
    int sockfd, s1, retcode;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    localAddrL = farAddrL = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];
    char v[10][BUFFER_SIZE];
    int count = 0;

    //Creazione socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(-1);
    }

    //localAddr setup
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(PORT);
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);

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

        if((retcode = read(s1, buffer, BUFFER_SIZE)) > 0){
            buffer[retcode-1] = '\0';
            printf("Client asks %s\n", buffer);
        }

        if(count == 10){
            strncpy(v[0], buffer, BUFFER_SIZE);
        }
        else{
            strncpy(v[count], buffer, BUFFER_SIZE);
            count++;
        }

        for(int i = 0; i < 10; i++)
            write(s1, v[i], strlen(v[i]));


        close(s1);
        printf("\t>>>Connection closed.\n");
    }

    exit(0);
}
