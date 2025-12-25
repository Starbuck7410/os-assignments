#pragma once
#define MAX_MESSAGE 200
#define MAX_USERNAME 100

typedef struct message_T{
    int id;
    char sender[MAX_USERNAME];
    char target[MAX_USERNAME];
    char content[MAX_MESSAGE];
    // Maybe add a timestamp later on?
} message_T;