#include "../include/readline.h"
#include "../include/processes.h"
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAX_PROMPT 64





int main(){
    char prompt[MAX_PROMPT] = "hw1shell$ ";
    processes_T procs = {
        .queue_idx = 1
    };
    line_T line;
    command_T command = {
        .length = 0,
        .args = NULL,
        .background = 0
    };
    
    int exit = 0;

    while(!exit){
        line = read_line(prompt);
        line_to_command(&command, line);
        
        // printf("Line is: %s, With length: %d\n", line.line, line.length);
        // dbg_print_command(&command);
        
        // Handle internal commands first
        if(line.length == 0) goto clear_line;
        if(strcmp(command.args[0], "cd") == 0){
            chdir(command.args[1]);
            goto clear_line;
        }
        if(strcmp(command.args[0], "chprompt") == 0){
            strcpy(prompt, command.args[1]);
            goto clear_line;
        }
        if(strcmp(command.args[0], "exit") == 0) {
            exit = 1;
            goto clear_line;
        }
        if(strcmp(command.args[0], "exit") == 0) {
            exit = 1;
            goto clear_line;
        }

        
        // Or an external command
        

        if(command.background && procs.queue_idx == MAX_PROCESSES - 1){
            printf("hw1shell: too many background commands running\n");
            goto clear_line;
        }

        pid_t pid = fork();
        if(pid > 0){
            if(command.background){
                printf("hw1shell: pid %d started\n", pid);
                procs.pids[procs.queue_idx] = pid;
                procs.queue_idx++;
            }else{
                waitpid(pid, &(procs.status[0]), 0);
            }

            // printf("Program exited with exit code: %d\n", procs.status[0]);
        }else{
            if(execvp(command.args[0], command.args)) printf("hw1shell: invalid command\n");
        }
            

        clear_line:

        for(int i = 1; i < MAX_PROCESSES; i++){
            if(procs.pids[i]){
                if(kill(procs.pids[i], 0)) {
                    printf("hw1shell: pid %d finished.\n", procs.pids[i]);
                    // TODO Ripple child processes
                }
            }
           
            
        }

        free(line.line);
        free_command(&command);
    }
    return 0;
}