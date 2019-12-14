#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#define PORT 7777
#define LOCAL_IP "127.0.0.1"
#define MAXQ 80
#define N 5
#define BUFFERSIZE 1024