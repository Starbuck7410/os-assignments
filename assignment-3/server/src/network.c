#include "../include/network.h"

int bind_to_port(int port){
    port = htons(port);
    int address; 
    inet_pton(AF_INET, GLOBAL_IP_ADDRESS, &address);
    sockaddr_in_T server_sockaddr = {
        .sin_port = port,
        .sin_addr = {.s_addr = address},
        .sin_family = AF_INET, 
        .sin_zero = {0}
    };
    int socket_file = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_file == -1) return -1;
    if(bind(socket_file, (sockaddr_T *) &server_sockaddr, INET_ADDRSTRLEN) == -1) {
        close(socket_file);
        return -1;
    }
    listen(socket_file, QUEUE_LENGTH);
    return socket_file;
}

