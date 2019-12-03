#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define PORT 3233
#define MAXQ 80
#define BUFFERSIZE 1024


int main(void){
    int sockfd, s1, retcode;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    char buffer[BUFFERSIZE], msg[BUFFERSIZE];
    time_t rawtime;
    struct tm* timeinfo;

    localAddrL = farAddrL = sizeof(struct sockaddr_in);

    //Creazione socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(-1);
    }

    //Configurazione localAddr
    localAddr.sin_family = PF_INET;
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
        printf("Client %s:%d connected.\n",
            inet_ntoa(farAddr.sin_addr),
            ntohs(farAddr.sin_port));
        
        if((retcode = read(s1, buffer, BUFFERSIZE)) > 0){
            buffer[retcode-1] = '\0';
            printf("Client asks %s\n", buffer);

            
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            printf("Server reply: %s", asctime(timeinfo));
            strcpy(msg, asctime(timeinfo));
        }
        else
            strcpy(msg, "ERROR");
        
        write(s1, msg, strlen(msg));
        close(s1);
    }
    close(sockfd);
}