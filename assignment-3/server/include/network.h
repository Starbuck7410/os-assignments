#pragma once
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include "../include/message.h"
#define GLOBAL_IP_ADDRESS "0.0.0.0"
#define MAX_CLIENTS 100
#define QUEUE_LENGTH 1


typedef struct sockaddr sockaddr_T;
typedef struct sockaddr_in sockaddr_in_T;

typedef struct client_T {
    char username[MAX_USERNAME];
    sockaddr_in_T sockaddr;
    unsigned int sockaddr_length;
    int fd;
} client_T;

int bind_to_port(int port);
int set_nonblocking(int socket_fd);