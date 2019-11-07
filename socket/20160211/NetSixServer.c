#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define PORT 39999
#define LOCAL_ADDR "127.0.0.1"
#define MAXQ 80
#define N_SERIE 6
#define BUFFER_SIZE 1024

int checkTitle(char** list, int* ep, const char* string){
    int episodio = atoi();
    for(int i = 0; i < N_SERIE; i++)
        if(strcmp(list[i], title) == 0){
            if(ep[i] >= )
        }
    return 0;
}


int main(){
    int sockfd, s1, retcode;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    localAddrL = farAddrL = sizeof(struct sockaddr_in);
    char* list[N_SERIE] = {"DonMatteo", "Unpassodalcielo", "CheDiociaiuti",
        "Unmedicoinfamiglia", "ICesaroni", "Maniac"};
    
    int ep[N_SERIE] = {10, 20, 30, 40, 50, 60};
    
    char buffer[BUFFER_SIZE], msg[BUFFER_SIZE];

    //Creazione socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("crezione socket");
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

    listen(sockfd, MAXQ);

    while((s1 = accept(sockfd, (struct sockaddr*) &farAddr, &farAddrL)) != -1){
        printf("Client %s:%d connected.\n",
            inet_ntoa(farAddr.sin_addr),
            ntohs(farAddr.sin_port));

        if((retcode = read(s1, buffer, BUFFER_SIZE)) > 0){
            if(buffer[strlen(buffer)-1] == '\n')
                buffer[strlen(buffer)-1] == '\0';

            if(checkTitle(list, buffer))
                strcpy(msg, "Disponibile");
            else
                strcpy(msg, "ComingSoon");
            
            write(s1, msg, strlen(msg));
        }

        close(s1);
    }

    printf("\n");
    close(sockfd);
    exit(0);
}