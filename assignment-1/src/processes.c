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
    for(size_t i = 1; i < procs->queue_idx; i++){
        printf("[%lu] %d\t%s\n", i, procs->pids[i], procs->lines[i].text);
    }
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
    procs->queue_idx--;
    procs->pids[job_id] = 0;
    ripple_processes(procs);
}