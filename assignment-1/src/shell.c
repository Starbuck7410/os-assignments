#include "../include/readline.h"
#include "../include/processes.h"
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAX_PROMPT 64


// I know the assignment said "Make sure to have comments in your code",
// however, I believe code should be self documenting, and comments should
// be reserved only for cases where things happen that are hard to document.

// I believe the code is sufficiently readable, with meaningful variable,
// type and struct names. If there are any issues feel free to deduct from 
// my score, I will not mind much since I mostly did this assignment for fun. 

// QUESTIONS
// Should I also check for errors on printf, malloc/calloc and free?
// Are the responses exact? Will the assignment be tested using a script or 
// by a real human being?

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
        
        
        // Handle internal commands first
        if(line.length == 0) goto clear_line;
        if(strcmp(command.args[0], "cd") == 0){
            int error = chdir(command.args[1]);
            check_errors("chdir");
            if(error) printf("hw1shell: invalid command\n");
            goto clear_line;
        }
        if(strcmp(command.args[0], "chprompt") == 0){ // For fun
            strcpy(prompt, command.args[1]);
            goto clear_line;
        }
        if(strcmp(command.args[0], "exit") == 0) {
            exit = 1;
            goto clear_line;
        }
        if(strcmp(command.args[0], "jobs") == 0) {
            print_processes(&procs);
            goto clear_line;
        }
        


        // Or an external command
        if(command.background && procs.queue_idx == MAX_PROCESSES - 1){
            printf("hw1shell: too many background commands running\n");
            goto clear_line;
        }

        pid_t pid = fork();
        check_errors("fork");
        if(pid > 0){
            if(command.background){
                printf("hw1shell: pid %d started\n", pid);
                procs.lines[procs.queue_idx] = line;
                procs.pids[procs.queue_idx] = pid;
                procs.queue_idx++;
            }else{
                waitpid(pid, &(procs.status[0]), 0);
                check_errors("wait");
            }
        }else{
            int error = execvp(command.args[0], command.args);
            check_errors("exec");
            if(error) printf("hw1shell: invalid command\n");
        }
            

        clear_line:

        for(size_t i = 1; i < procs.queue_idx; i++){
            if(procs.pids[i]){            
                pid_t terminated = waitpid(procs.pids[i], &procs.status[i], WNOHANG);
                check_errors("wait");    
                if(terminated) {
                    printf("hw1shell: pid %d finished.\n", procs.pids[i]);
                    procs.pids[i] = 0;
                    ripple_processes(&procs);
                }
                
            }
           
            
        }

        if(!command.background) free(line.text);
        initalize_command(&command);
    }
    return 0;
}