#include "../include/readline.h"
#include <stdlib.h>
#include <errno.h>
#define MAX_PROCESSES 5

typedef struct processes_T {
    pid_t pids[MAX_PROCESSES]; 
    line_T lines[MAX_PROCESSES];
    int status[MAX_PROCESSES];
    size_t queue_idx; // 0 is always the foreground process
} processes_T;

void ripple_processes(processes_T * procs);
void print_processes(processes_T * procs);
void check_errors(char * syscall);