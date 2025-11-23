#pragma once
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define MAX_PROMPT 64

typedef struct command_T{
    size_t length;
    int background;
    char ** args;
} command_T;


typedef struct line_T{
    size_t length;
    char * text;
    int eof;
} line_T;

line_T read_line(char * prompt);
void line_to_command(command_T * command, line_T line);
void clear_command(command_T * command);
int string_to_pos_int(char * string);
void clear_line(line_T * line);
int replace_substring(char * origin, char * replace, char * target);