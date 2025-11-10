#include "../include/readline.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>




int main(){
    line_T line;
    command_T command;

    while(1){
        line = read_line("hw1shell$ ");
        if(strcmp(line.line, "exit") == 0) break;
        // printf("Line is: %s, With length: %d\n", line.line, line.length);
        line_to_command(&command, line);
        // dbg_print_command(&command);
        
        if(strcmp(command.args[0], "cd") == 0){
            chdir(command.args[1]);
            goto finish;
        }

        pid_t pid = fork();

        if(pid != 0){
            int status;
            waitpid(pid, &status, 0);
            // printf("Program exited with exit code: %d\n", status);
        }else{
            execvp(command.args[0], command.args);
        }


        finish:
        free(line.line);
        free_command(&command);
    }
    return 0;
}