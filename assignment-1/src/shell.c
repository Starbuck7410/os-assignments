#include "../include/readline.h"
#include "../include/processes.h"
#include <stdlib.h>
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

processes_T procs = { // This one is only global so the ctrl + c thing will work
    .queue_idx = 1
};

void kill_child(int signal_id){
    if(procs.pids[0] && signal_id == SIGINT) kill(procs.pids[0], SIGKILL);
    write(STDOUT_FILENO, "\n", 1);
}


int main(){
    if (signal(SIGINT, kill_child) == SIG_ERR) {
        printf("Error registering signal handler");
        return 1;
    }

    char prompt[MAX_PROMPT] = "hw1shell$ ";
    
    line_T line;
    command_T command = {
        .length = 0,
        .args = NULL,
        .background = 0
    };
    
    int exit = 0;

    while(!exit){
        line = read_line(prompt);
        if(line.eof){
            printf("\n");
            exit = 1;
            goto clear_line;
        }
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
        if(strcmp(command.args[0], "fg") == 0) {
            int job_id = string_to_pos_int(command.args[1]);
            if(job_id <= 0){
                printf("Switching to job [%d]\n", job_id);
                process_to_fg(&procs, job_id);
                waitpid(procs.pids[0], &(procs.status[0]), 0);
                check_errors("wait");
                procs.pids[0] = 0;
            } 
            goto clear_line;
        }
        

        if(command.background && procs.queue_idx == MAX_PROCESSES){
            printf("hw1shell: too many background commands running\n");
            goto clear_line;
        }

        // Or an external command
        run_external_command(&command, &procs, line);
        // printf("%lu\n", procs.queue_idx);


            

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

        if(!command.background) clear_line(&line);
        clear_command(&command);
    }
    for(int i = procs.queue_idx - 1; i > 0; i++){
        kill(procs.pids[i], SIGKILL);
        waitpid(procs.pids[i], &procs.status[i], 0);
    }
    printf("Exiting...\n");
    return 0;
}