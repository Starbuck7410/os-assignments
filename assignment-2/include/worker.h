#pragma once
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#define MAX_THREADS 4096
#define MAX_COUNTERS 100
#define MAX_LINE 1024


typedef enum operation_T{
    CMD_MSLEEP,
    CMD_INCREMENT,
    CMD_DECREMENT,
    CMD_REPEAT,
    CMD_INVALID
} operation_T;

typedef struct command_T {
    operation_T operation;
    int argument;
} command_T;


typedef struct job_T {
    command_T * commands;
    size_t length;
    long long read_time;
    char * original_line;
    struct job_T * next;
} job_T;

typedef struct queue_T {
    job_T * head;
    job_T * tail;
    int size;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
} queue_T;


typedef struct context_T{
    int num_threads;
    int num_counters;
    int log_enabled;
    long long start_time;
    pthread_mutex_t counter_mutexes[MAX_COUNTERS];

    pthread_mutex_t stats_lock;
    long long total_turnaround;
    long long min_turnaround;
    long long max_turnaround;
    long long jobs_completed; 
    
    int pending_jobs; 
    pthread_cond_t all_jobs_done; 
    int shutdown;

    FILE * dispatcher_log;
} context_T;


void * worker_function(void * arg);
long long get_current_time_ms();
void free_job(job_T * job);
long long get_time_diff();
job_T * parse_job_line(char * line);