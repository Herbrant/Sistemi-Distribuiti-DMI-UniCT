#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 3334
#define MAXQ 80
#define BUFFERSIZE 1024

int cubo(int x){
    return x*x*x;
}


int main(){
    int sockfd, s1, retcode, val;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    char buffer[1024], msg[1024];

    localAddrL = farAddrL = sizeof(struct sockaddr_in);


    //Creazione socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(-1);
    }

    //Configurazione indirizzo
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(PORT);
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //Binding
    if(bind(sockfd, (struct sockaddr*) &localAddr, localAddrL) == -1){
        perror("binding");
        exit(-2);
    }

    listen(sockfd, MAXQ);

    while((s1 = accept(sockfd, (struct sockaddr*) &farAddr, &farAddrL)) != -1){
        printf("Connected %s:%d\n",
            inet_ntoa(farAddr.sin_addr),
            ntohs(farAddr.sin_port));
        
        if((retcode = read(s1, buffer, BUFFERSIZE)) > 0){
            buffer[retcode-1] = '\0';
            printf("Client asks %s\n",
            buffer);
        
            val = atoi(buffer);
            printf("%d\n", cubo(val));
            sprintf(msg, "Il cubo di %d è %d", val, cubo(val));
            write(s1, msg, strlen(msg));
        }

        close(s1);
    }
    close(sockfd);
}