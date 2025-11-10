#include <unistd.h>
#define MAX_PROCESSES 5

typedef struct processes_T {
    pid_t pids[MAX_PROCESSES]; 
    int status[MAX_PROCESSES];
    size_t queue_idx; // 0 is always the foreground process
} processes_T;