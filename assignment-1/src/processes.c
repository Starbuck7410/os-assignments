#include "../include/processes.h"

void ripple_processes(processes_T * procs){
    size_t ripple_amoount = 0;
    for(size_t i = 1; i < procs->queue_idx; i++){
        procs->pids[i - ripple_amoount] = procs->pids[i];
        if(procs->pids[i] == 0) ripple_amoount++;
    }
    procs->queue_idx -= ripple_amoount;
}