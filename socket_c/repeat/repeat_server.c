#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFFER_SIZE 1024
#define PORT 2000
#define LOCAL_IP "127.0.0.1"
#define DELAY 1
#define MAXQ 20

int main(){
    int s, s1, retcode;
    struct sockaddr_in locAddr, farAddr;
    socklen_t farAddrL, locAddrL;
    char buffer[BUFFER_SIZE], msg[BUFFER_SIZE];

    //Creazione socket
    if((s = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(-1);
    }

    locAddr.sin_family = AF_INET;
    locAddr.sin_port = htons(PORT);
    locAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    farAddrL = locAddrL = sizeof(struct sockaddr_in);

    if(bind(s, (struct sockaddr*) &locAddr, locAddrL) != 0){
        perror("trying to bind");
        exit(-2);
    }

    listen(s, MAXQ);

    while((s1 = accept(s, (struct sockaddr*) &farAddr, &farAddrL)) != -1){
        printf("Client %s/%d connected\n",
            inet_ntoa(farAddr.sin_addr),
            ntohs(farAddr.sin_port));

        if((retcode = read(s1, buffer, BUFFER_SIZE)) > 0){
            buffer[retcode-1] = '\0';
            printf("Client asks: %s\n", buffer);
            buffer[retcode-1] = '\n';
        }

        sprintf(msg, "%s\n", buffer, BUFFER_SIZE);
        write(s1, msg, strlen(msg));
        sleep(DELAY);
        close(s1);
        printf("Connection with client closed\n\n");
    }

    exit(0);
}
