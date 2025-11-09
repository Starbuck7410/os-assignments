#include "../include/readline.h"
#include <stdlib.h>
#include <string.h>


line_T read_line(char * prompt){
    printf("%s", prompt);
    fflush(stdout);
    char * line_buffer = malloc(MAX_BUFFER);
    size_t length = read(STDIN_FILENO, line_buffer, MAX_BUFFER);
    length--;
    line_buffer[length] = 0;
    line_T final_line = {
        .length = length,
        .line = line_buffer
    };
    return final_line;
}


void line_to_command(command_T * command, line_T line){
    command->args = malloc(MAX_ARGS * sizeof(char *));
    int arg = 0;
    int j = 0;
    // if(line.length < 0 || line.length > MAX_BUFFER) return;
    char buffer[MAX_BUFFER];
    for(int i = 0; i < line.length; i++){
        printf("Processing char: %c, argument number %d, index %d\n", line.line[i], arg, j);
        if(line.line[i] != ' '){
            buffer[j] = line.line[i];
            j++;
        }else{
            command->args[arg] = malloc(j + 1);
            command->args[arg][j] = 0;
            strncpy(command->args[arg], buffer, j + 1);
            arg++;
            j = 0;
        }
    }
    command->length = arg;
    return;
}

void print_command(command_T * command){
    for(int i = 0; i < command->length; i++){
        printf("arg[%d] %s, ", i, command->args[i]);
    }
}
