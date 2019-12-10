#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 3333
#define MAXQ 80
#define BUFFERSIZE 1024
#define N_SERIE 2
#define LOCALADDR "127.0.0.1"

int checkSerie(char** listaserie, int* listaepisodi, char* serie, char* n){
    for(int i = 0; i < N_SERIE; i++)
        if((strcmp(serie, listaserie[i]) == 0) && (listaepisodi[i] >= atoi(n)))
            return 1;
    return 0;
}

int main(){
    int sockfd, s1, retcode, ok;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    localAddrL = farAddrL = sizeof(struct sockaddr_in);
    
    char* lista_serietv[N_SERIE] = {"friends", "supernatural"};
    int lista_episodi[N_SERIE] = {100, 200};
    char buffer[BUFFERSIZE], message[BUFFERSIZE];
    char* serie, *episodio;

    //Creazione socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(-1);
    }
    
    //Preparazione indirizzo localAddr
    localAddr.sin_family = PF_INET;
    localAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, LOCALADDR, &(localAddr.sin_addr));
    
    //Binding
    if(bind(sockfd, (struct sockaddr*) &localAddr, localAddrL) == -1){
        perror("binding");
        exit(-2);
    }
    
    listen(sockfd, MAXQ);

    while((s1 = accept(sockfd, (struct sockaddr*) &farAddr, &farAddrL)) != -1){
        printf("Client: %s:%d connected.\n",
            inet_ntoa(farAddr.sin_addr),
            ntohs(farAddr.sin_port));
        
        if((retcode = read(s1, buffer, BUFFERSIZE)) > 0){
            buffer[retcode] = '\0';
            printf("Client asks %s\n", buffer);

            if((serie = strtok(buffer, ",")) && (episodio = strtok(NULL, ","))){
                
                if(checkSerie(lista_serietv, lista_episodi, serie, episodio))
                    strcpy(message, "DISPONIBILE\n");
                else
                    strcpy(message, "NON DISPONIBILE\n");
            }
            else
                strcpy(message, "ERRORE NELLA RICHIESTA\n");
        }
        else
            strcpy(message, "ERRORE NELLA RICHIESTA\n");
        
        write(s1, message, strlen(message));
        close(s1);
    }

    close(sockfd);
}