#include "../include/processes.h"

void ripple_processes(processes_T * procs){
    size_t ripple_amoount = 0;
    for(size_t i = 1; i < procs->queue_idx; i++){
        procs->pids[i - ripple_amoount] = procs->pids[i];
        procs->lines[i - ripple_amoount] = procs->lines[i];
        procs->status[i - ripple_amoount] = procs->status[i - ripple_amoount];
        if(procs->pids[i] == 0) {
            ripple_amoount++;
            free(procs->lines[i].text);
            procs->status[i] = 0;
        }
    }
    procs->queue_idx -= ripple_amoount;
}

void print_processes(processes_T * procs){
    for(size_t i = 1; i < procs->queue_idx; i++){
        printf("%d\t%s\n", procs->pids[i], procs->lines[i].text);
    }
}