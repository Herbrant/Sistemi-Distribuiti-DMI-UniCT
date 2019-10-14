#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define LOCAL_IP "127.0.0.1"
#define PORT 2000
#define BUFFER_SIZE 1024
#define MAXQ 10
#define DELAY 1


int main(){
    int sockfd, s1, retcode, i;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    char buffer[BUFFER_SIZE], msg[BUFFER_SIZE];

    //Creazione socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("creating socket");
        exit(-1);
    }
    //Setup localAddr structure
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(PORT);
    //localAddr.sin_addr.s_addr = htonl(LOCAL_IP);
    inet_pton(AF_INET, LOCAL_IP, &(localAddr.sin_addr));

    localAddrL = farAddrL = sizeof(struct sockaddr_in);

    //Binding
    if(bind(sockfd, (struct sockaddr*) &localAddr, localAddrL) == -1){
        perror("binding");
        exit(-2);
    }

    listen(sockfd, MAXQ);

    while((s1 = accept(sockfd, (struct sockaddr*) &farAddr, &farAddrL)) != -1){
        printf("Client %s/%d connected.\n",
            inet_ntoa(farAddr.sin_addr),
            ntohs(farAddr.sin_port));

        if((retcode = read(s1, buffer, BUFFER_SIZE)) > 0){
            buffer[retcode-1] = '\0';
            printf("Client asks %s\n", buffer);
            buffer[retcode-1] = '\n';
        }

        memcpy(msg, buffer, BUFFER_SIZE);

        i = 0;
        while(msg[i] != '\0'){
            msg[i] = toupper(msg[i]);
            i++;
        }

        write(s1, msg, strlen(msg));
        sleep(DELAY);
        close(s1);
        printf("Connection closed.\n");
    }
    close(sockfd);
    exit(0);
}
