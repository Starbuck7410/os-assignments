#include "../include/processes.h"



void ripple_processes(processes_T * procs){
    size_t ripple_amoount = 0;
    for(size_t i = 1; i < procs->queue_idx; i++){
        procs->pids[i - ripple_amoount] = procs->pids[i];
        procs->lines[i - ripple_amoount] = procs->lines[i];
        procs->status[i - ripple_amoount] = procs->status[i - ripple_amoount];
        if(procs->pids[i] == 0) {
            ripple_amoount++;
            clear_line(&(procs->lines[i]));
            procs->status[i] = 0;
        }
    }
    procs->queue_idx -= ripple_amoount;
}

void print_processes(processes_T * procs){
    #ifdef FUNMODE
    for(size_t i = 1; i < procs->queue_idx; i++){
        printf("[%lu] %d\t%s\n", i, procs->pids[i], procs->lines[i].text);
    }
    #else
    for(size_t i = 1; i < procs->queue_idx; i++){
        printf("%d\t%s\n", procs->pids[i], procs->lines[i].text);
    }
    #endif
}

void check_errors(char * syscall){
    if(errno){
        printf("hw1shell: %s failed, errno is %d\n", syscall, errno);
    }
    errno = 0;
}

void process_to_fg(processes_T * procs, int job_id){
    procs->pids[0] = procs->pids[job_id];
    procs->status[0] = procs->status[job_id];
    procs->lines[0] = procs->lines[job_id];
    procs->pids[job_id] = 0;
    ripple_processes(procs);
}

void run_external_command(command_T * command, processes_T * procs, line_T line){
    pid_t pid = fork();

    check_errors("fork");
    if(pid > 0){
        if(command->background){
            printf("hw1shell: pid %d started\n", pid);
            procs->lines[procs->queue_idx] = line;
            procs->pids[procs->queue_idx] = pid;
            procs->queue_idx++;
        }else{
            procs->pids[0] = pid;
            waitpid(procs->pids[0], &(procs->status[0]), 0);
            check_errors("wait");
            procs->pids[0] = 0;
        }
    }else{
        #ifdef FUNMODE
        signal(SIGINT, SIG_IGN);
        #endif
        int error = execvp(command->args[0], command->args);
        check_errors("exec");
        if(error) printf("hw1shell: invalid command\n");
        exit(0);
    }
}