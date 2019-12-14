#include "serverpoll.h"

int main(void){
    int sockfd, s1, retcode, k;
    struct sockaddr_in localAddr, farAddr;
    socklen_t localAddrL, farAddrL;
    localAddrL = farAddrL = sizeof(struct sockaddr_in);
    char* animali[N] = {"cane", "gatto", "formica", "leone", "asino"};
    char buffer[BUFFERSIZE], msg[BUFFERSIZE];
    char* tmp;

    //Creazione socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(-1);
    }

    //Configurazione localAddr
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(PORT);
    inet_pton(PF_INET, LOCAL_IP, &(localAddr.sin_addr));

    //Binding
    if(bind(sockfd, (struct sockaddr*) &localAddr, localAddrL) == -1){
        perror("binding");
        exit(-1);
    }

    listen(sockfd, MAXQ);

    while((s1 = accept(sockfd, (struct sockaddr*) &farAddr, &farAddrL)) != -1){
        printf("Client %s:%d connected.\n",
            inet_ntoa(farAddr.sin_addr),
            ntohs(farAddr.sin_port));
        
        if((retcode = read(s1, buffer, BUFFERSIZE)) > 0){
            //buffer[retcode] = '\0';

            printf("Client asks %s\n", buffer);

            if(buffer[0] == 'G'){
                memset(msg, 0, sizeof(msg));

                for(int i = 0; i < N-1; i++){
                    strcat(msg, animali[i]);
                    strcat(msg, ", ");
                }
                strcat(msg, animali[N-1]);
                msg[strlen(msg)-1] = '\n';
            }
            else if(buffer[0] == 'V'){
                
                strcpy(tmp, buffer+1);
                k = atoi(tmp);

                if(k < N){
                    strcpy(msg, animali[k]);
                    strcat(msg, "\n");
                }
                else
                    strcpy(msg, "Non presente\n");
                
            }
            else
                strcpy(msg, "Error\n");
            
        }
        else
            strcpy(msg, "Error\n");   

        write(s1, msg, strlen(msg));
        close(s1);
    }
    free(tmp);
    close(sockfd);
}