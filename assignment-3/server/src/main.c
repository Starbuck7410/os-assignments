#include "../include/network.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/select.h>
#include "../include/message.h"
#define MAX_FULL_MESSAGE MAX_USERNAME + MAX_MESSAGE + 3 

// Since i did not have much time (Thanks to the computer architecture
// course), this code might be a mess. It's mostly string lengths and lack
// of functions, but I personally do not like this code. Please don't be 
// too harsh on me please.

int main(int argc, char ** argv){
    if(argc != 2) {
        printf("Wrong argument count. Expected: \"./hw3server port\".\n");
        return 1;
    }
    int listen_fd = bind_to_port(atoi(argv[1]));
    if(listen_fd == -1) {
        printf("Failed to bind to port.\n");
        return 1;
    }
    client_T temp_client;
    temp_client.sockaddr_length = sizeof(sockaddr_T);
    client_T client [MAX_CLIENTS];
    int client_count = 0;
    char message[MAX_MESSAGE];
    char full_message[MAX_FULL_MESSAGE];
    while(1){

        fd_set read_fds;
        int max_fd = 0;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(listen_fd, &read_fds);
        max_fd = (listen_fd > STDIN_FILENO) ? listen_fd : STDIN_FILENO;
        for(int i = 0; i < client_count; i++){
            FD_SET(client[i].fd, &read_fds);
            max_fd = (client[i].fd > max_fd) ? client[i].fd : max_fd;
        }
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }

        for(int i = 0; i < client_count; i++){
            if (FD_ISSET(client[i].fd, &read_fds)) {
                size_t n = recv(client[i].fd, message, MAX_MESSAGE - 1, 0);
                if (n <= 0) {
                    printf("%s disconnected\n", client[i].username);
                    close(client[i].fd);
                    client[i].fd = -1;
                    continue;
                }
                message[n] = '\0';

                char friend[MAX_MESSAGE];
                int whisper = 0;
                if(message[0] == '@'){
                    strncpy(friend, message, MAX_MESSAGE);
                    strtok(friend, " ");
                    whisper = 1;
                }else{
                    whisper = 0;
                }       
                snprintf(full_message, MAX_FULL_MESSAGE, "%s: %s", client[i].username, message);
                printf("%s\n", full_message);
                for(int j = 0; j < client_count; j++){
                    if(!whisper || strcmp(friend + 1, client[j].username) == 0){
                        send(client[j].fd, full_message, strlen(full_message), 0);
                    }
                }
            }
        }

        if (FD_ISSET(listen_fd, &read_fds)) {
            char auth[MAX_MESSAGE];
            char * token;
            temp_client.fd = accept(listen_fd, (sockaddr_T *) &temp_client.sockaddr, &temp_client.sockaddr_length);
            if(temp_client.fd != -1){
                size_t n = recv(temp_client.fd, auth, MAX_MESSAGE - 1, 0);
                auth[n] = '\0';
                token = strtok(auth, " ");
                if(strcmp(token, "@authenticate") != 0){
                    printf("Client tried to connect but authentication failed.\n");
                    close(temp_client.fd);
                }else{
                    token = strtok(NULL, " ");
                    strncpy(temp_client.username, token, strlen(token) + 1);
                    memcpy(client + client_count, &temp_client, sizeof(client_T));
                    char address[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &temp_client.sockaddr.sin_addr, address, INET_ADDRSTRLEN);
                    printf("%s connected from address %s!\n", temp_client.username, address);
                    client[client_count].fd = temp_client.fd;
                    client_count++;
                }
            }
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            size_t n = read(STDIN_FILENO, message, MAX_MESSAGE);
            if (n <= 0) {
                perror("read");
                break;
            }
            message[n] = '\0';
            message[strcspn(message, "\n")] = '\0'; 
            if(strcmp(message, "!exit") == 0) break;
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
    printf("Exiting...\n");
    for(int i = 0; i < client_count; i++){
        close(client[i].fd);
    }
    close(listen_fd);
    return 0;
}