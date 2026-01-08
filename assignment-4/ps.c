#include "types.h"
#include "stat.h"
#include "user.h"
#include "processInfo.h"
#define MAX_STATE_STR 9
// Clever implementation shamelessly stolen from proc.c
enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

static char *states[] = {
    [UNUSED]    "unused",
    [EMBRYO]    "embryo",
    [SLEEPING]  "sleeping",
    [RUNNABLE]  "runnable",
    [RUNNING]   "running",
    [ZOMBIE]    "zombie"
};

int main(int argc, char *argv[]) {
    struct processInfo info;
    int num_proc = getNumProc();
    int max_pid = getMaxPid();
    char state_str[MAX_STATE_STR];

    printf(FD_STDOUT, "Total number of active processes: %d\n", num_proc);
    printf(FD_STDOUT, "Maximum PID: %d\n", max_pid);

    printf(FD_STDOUT, "PID\tSTATE\t\tPPID\tSZ\tNFD\tNRSWITCH\n");

    for (int i = 1; i <= max_pid; i++) {
        if (getProcInfo(i, &info) >= 0) {
            
            // You have NO idea what we had to go through to use this function
            strncpy(state_str, states[info.state], MAX_STATE_STR); 
            char tab[2] = {0, 0};
            tab[0] = strlen(state_str) < 8 ? (uchar) '\t' : (uchar) '\0';
            printf(FD_STDOUT, "%d\t%s%s\t%d\t%d\t%d\t%d\n", 
                i, 
                state_str, 
                tab, 
                info.ppid, 
                info.sz, 
                info.nfd, 
                info.nrswitch);
        }
    }

    exit();
}