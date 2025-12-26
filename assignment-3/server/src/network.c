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
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1) return -1;
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(bind(listen_fd, (sockaddr_T *) &server_sockaddr, INET_ADDRSTRLEN) == -1) {
        close(listen_fd);
        return -1;
    }
    listen(listen_fd, QUEUE_LENGTH);
    return listen_fd;
}

