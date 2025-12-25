#include "../include/network.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/select.h>
#include "../include/message.h"


int main(int argc, char ** argv){
    if(argc != 2) {
        printf("Wrong argument count. Expected: \"./hw3server port\".\n");
        return 1;
    }
    int socket_file = bind_to_port(atoi(argv[1]));
    if(socket_file == -1) {
        printf("Failed to bind to port.\n");
        return 1;
    }
    client_T temp_client;
    temp_client.sockaddr_length = sizeof(sockaddr_T);
    client_T client [MAX_CLIENTS];
    int client_count = 0;
    char message[MAX_MESSAGE];
    while(1){

        fd_set read_fds;
        int maxfd = 0;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(socket_file, &read_fds);
        maxfd = (socket_file > STDIN_FILENO) ? socket_file : STDIN_FILENO;
        for(int i = 0; i < client_count; i++){
            FD_SET(client[i].fd, &read_fds);
            maxfd = (client[i].fd > maxfd) ? client[i].fd : maxfd;
        }
        if (select(maxfd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }

        for(int i = 0; i < client_count; i++){
            if (FD_ISSET(client[i].fd, &read_fds)) {
                size_t n = recv(client[i].fd, message, MAX_MESSAGE - 1, 0);
                if (n <= 0) {
                    printf("Client disconnected\n");
                    client[i].fd = -1;
                    continue;
                }
                message[n] = '\0';
                printf("%s\n", message);
                for(int i = 0; i < client_count; i++){
                    send(client[i].fd, message, strlen(message), 0);
                }
            }
        }

        if (FD_ISSET(socket_file, &read_fds)) {
            char auth[MAX_MESSAGE];
            char * token;
            temp_client.fd = accept(socket_file, (sockaddr_T *) &temp_client.sockaddr, &temp_client.sockaddr_length);
            if(temp_client.fd != -1){
                recv(temp_client.fd, auth, MAX_MESSAGE - 1, 0);
                token = strtok(auth, " ");
                if(strcmp(token, "@authenticate") != 0){
                    printf("Client tried to connect but authentication failed.\n");
                    close(temp_client.fd);
                }else{
                    token = strtok(NULL, " ");
                    memcpy(temp_client.username, token, strlen(token));
                    printf("%s connected!\n", temp_client.username);
                    memcpy(client + client_count, &temp_client, sizeof(client_T));
                    client[client_count].fd = temp_client.fd;
                    client_count++;
                }
            }
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            size_t n = read(STDIN_FILENO, message, MAX_MESSAGE);
            if (n <= 0) {
                break;
            }
            message[n] = '\0';
            message[strcspn(message, "\n")] = '\0'; 
            if(strcmp(message, "!exit") == 0){
                printf("Exiting...\n");
                break;
            }
        }

        int shift = 0;
        for (int i = 0; i < client_count; i++) {
            if (client[i].fd == -1) {
                shift++;  
            } else if (shift > 0) {
                client[i - shift] = client[i];  
            }
        }
        client_count -= shift;  

    }
    close(socket_file);
    return 0;
}