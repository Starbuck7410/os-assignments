#include "../include/readline.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>




int main(){
    line_T line;
    command_T command;

    while(1){
        line = read_line("hw1shell> ");
        if(strcmp(line.line, "exit") == 0) break;
        printf("Line is: %s, With length: %d\n", line.line, line.length);
        line_to_command(&command, line);
        print_command(&command);
        // pid_t pid = fork();
        // if(pid != 0){

        // }else{
            
        // }
        free(line.line);
    }
    return 0;
}