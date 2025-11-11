#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#define MAX_BUFFER 1024
#define MAX_ARGS 64

typedef struct command_T{
    size_t length;
    int background;
    char ** args;
} command_T;


typedef struct line_T{
    size_t length;
    char * text;
} line_T;

line_T read_line(char * prompt);
void dbg_print_command(command_T * command);
void line_to_command(command_T * command, line_T line);
void initalize_command(command_T * command);