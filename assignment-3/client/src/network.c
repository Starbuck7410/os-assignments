#include "../include/network.h"

server_T connect_to_socket(int port, char * address_text){
    int address; 
    inet_pton(AF_INET, address_text, &address);
    port = htons(port);
    sockaddr_in_T server_sockaddr = {
        .sin_port = port,
        .sin_addr = {.s_addr = address},
        .sin_family = AF_INET, 
        .sin_zero = {0}
    };
    server_T server;
    server.sockaddr = server_sockaddr;
    server.fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sockaddr_length = sizeof(sockaddr_T);

    if(server.fd == -1) {
        printf("Failed to connect to socket.\n");
        return (server_T) {0};
    }

    return server;
}