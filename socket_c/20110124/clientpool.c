#include "clientpool.h"


int main(int argc, char** argv){
    if(argc < 3){
        fprintf(stderr, "USE %s [SERVER_IP] [PORT] [COMMAND]\n", argv[0]);
        exit(-1);
    }
    int sockfd, retcode;
    struct sockaddr_in addr;
    socklen_t addrL = sizeof(struct sockaddr_in);
    char buffer[BUFFERSIZE];

    //Creazione socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(-1);
    }

    //Configurazione addr
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &(addr.sin_addr));

    //Connect
    if(connect(sockfd, (struct sockaddr*) &addr, addrL) != -1){
        write(sockfd, argv[3], strlen(argv[3]));

        if((retcode = read(sockfd, buffer, BUFFERSIZE)) > 0){
            printf("%s\n", buffer);
        }
    }

    close(sockfd);
}
