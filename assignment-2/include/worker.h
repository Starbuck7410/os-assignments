#pragma once
#include <pthread.h>

#define MAX_THREADS 4096
#define MAX_LINE 


typedef struct queue_T {

} queue_T;



typedef struct job_T {
    command_T * commands;
    size_t length;
} job_T;

    
typedef enum operation_T{
    DISPATCH_SLEEP,
    DISPATCH_WAIT,
    MSLEEP,
    INCREMENT,
    DECREMENT,
    REPEAT
} operation_T;

typedef struct command_T {
    operation_T operation;
    int argument;
} command_T;

