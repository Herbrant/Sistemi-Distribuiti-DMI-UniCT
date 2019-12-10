#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 39939
#define LOCALADDR "127.0.0.1"
#define MAXQ 80
#define BUFFERSIZE 1024
#define N_LIBRI 3

int checkBook(char** booklist, int* booklistN, char* libro, char* n){
    for(int i = 0; i < N_LIBRI; i++)
        if(strcmp(booklist[i], libro) == 0 && booklistN[i] >= atoi(n))
            return 1;
    return 0;
}


int main(void){
    int sockfd, s1, retcode;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    localAddrL = farAddrL = sizeof(struct sockaddr_in);
    char buffer[BUFFERSIZE], msg[BUFFERSIZE];
    char* libro, *n;

    //Lista libri
    char* booklist[N_LIBRI] = {"1984", "la svastica sul sole", "guida galattica per autostoppisti"};
    int booklistN[N_LIBRI] = {3, 5, 10};

    //Creazione socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(-1);
    }

    //Setup localAddr
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, LOCALADDR, &(localAddr.sin_addr));

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
            buffer[retcode] = '\0';

            printf("Client asks %s\n", buffer);

            if((libro = strtok(buffer, ",")) && (n = strtok(NULL, ","))){
                if(checkBook(booklist, booklistN, libro, n))
                    strcpy(msg, "DISPONIBILE");
                else
                    strcpy(msg, "NON DISPONIBILE");
            }
            else
                strcpy(msg, "ERRORE NELLA RICHIESTA DEL CLIENT");
        }
        else
            strcpy(msg, "ERRORE NELLA RICHIESTA DEL CLIENT");

        write(s1, msg, strlen(msg));
        close(s1);
    }

    close(sockfd);
}