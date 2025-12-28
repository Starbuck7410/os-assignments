#include "../include/network.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/select.h>
#define MAX_MESSAGE 100
#define MAX_USERNAME 100


int main(int argc, char ** argv){
    if(argc != 4) {
        printf("Wrong argument count. Expected: \"./hw3client address port username\".\n");
        return 1;
    }

    char username[MAX_USERNAME];
    strcpy(username, argv[3]);

    server_T server = connect_to_socket(atoi(argv[2]), argv[1]);
    if(server.fd == -1) return 1;

    if(connect(server.fd, (sockaddr_T *) &server.sockaddr, server.sockaddr_length) == -1) {
        printf("Failed to connect to server.\n");
        close(server.fd);
        return 1;
    }

    char auth[MAX_USERNAME + 15];

    snprintf(auth, MAX_USERNAME + 15, "@authenticate %s", username);
    send(server.fd, auth, strlen(auth), 0);
    
    int maxfd = server.fd > STDIN_FILENO ? server.fd : STDIN_FILENO;
    char message[MAX_MESSAGE];
    int exit = 0;
    while(!exit){

        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(server.fd, &read_fds);
        if (select(maxfd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }


        if (FD_ISSET(server.fd, &read_fds)) {
            size_t n = recv(server.fd, message, MAX_USERNAME + MAX_MESSAGE + 3, 0);
            if (n <= 0) {
                printf("Server disconnected\n");
                break;
            }
            message[n] = '\0';
            printf("%s\n", message);
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
            send(server.fd, message, MAX_MESSAGE - 1, 0);
        }

        
    }
    close(server.fd);
    return 0;
}


