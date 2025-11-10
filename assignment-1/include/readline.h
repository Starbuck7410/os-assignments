#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#define MAX_BUFFER 1024
#define MAX_ARGS 64

typedef struct command_T{
    size_t length;
    char ** args;
} command_T;


typedef struct line_T{
    size_t length;
    char * line;
} line_T;

line_T read_line(char * prompt);
void dbg_print_command(command_T * command);
void line_to_command(command_T * command, line_T line);
void free_command(command_T * command);