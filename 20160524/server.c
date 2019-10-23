#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define PORT 3333
#define LOCAL_IP "127.0.0.1"
#define MAXQ 80
#define BUFFERSIZE 1024

int cubo(int n){return n*n*n;}  //La libreria math.h è sopravvalutata :)


int main(){
    int sockfd, s1, retcode, value;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    char buffer[BUFFERSIZE], msg[BUFFERSIZE];

    //Creazione socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("creazione socket");
        exit(-1);
    }

    //Preparazione indirizzo locale
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, LOCAL_IP, &(localAddr.sin_addr));

    localAddrL = farAddrL = sizeof(struct sockaddr_in);

    //Binding
    if(bind(sockfd, (struct sockaddr*) &localAddr, localAddrL) == -1){
        perror("binding");
        exit(-2);
    }

    listen(sockfd, MAXQ);

    while((s1 = accept(sockfd, (struct sockaddr*) &farAddr, &farAddrL)) != -1){
        printf("Client %s:%d connected.\n",
            inet_ntoa(farAddr.sin_addr),
            ntohs(farAddr.sin_port));
        
        if((retcode = read(s1, buffer, BUFFERSIZE)) > 0){
            //buffer[retcode-1] = '\0';
            printf("Client asks %s\n", buffer);
        }

        value = atoi(buffer);
        sprintf(msg, "%d", cubo(value));
        
        write(s1, msg, BUFFERSIZE-1);
        close(s1);
    }

    close(sockfd);
    exit(0);
}