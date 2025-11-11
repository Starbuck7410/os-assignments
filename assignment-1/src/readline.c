#include "../include/readline.h"
#include <stdlib.h>
#include <string.h>


line_T read_line(char * prompt){
    printf("%s", prompt);
    fflush(stdout);
    char * line_buffer = malloc(MAX_BUFFER);
    size_t length = read(STDIN_FILENO, line_buffer, MAX_BUFFER);
    if(length > MAX_BUFFER){
        // TODO error handling
    }
    length--;
    line_buffer[length] = 0;
    line_T final_line = {
        .length = length,
        .text = line_buffer
    };
    return final_line;
}


void line_to_command(command_T * command, line_T line){
    command->args = malloc(MAX_ARGS * sizeof(char *));
    int arg = 0;
    int j = 0;
    // if(line.length < 0 || line.length > MAX_BUFFER) return;
    char buffer[MAX_BUFFER];
    for(size_t i = 0; i < line.length + 1; i++){
        if(line.text[i] != ' ' && line.text[i] != 0 && line.text[i] != '&'){
            buffer[j] = line.text[i];
            j++;
        }else{
            if(j != 0){
                command->args[arg] = calloc(j, sizeof(char));
                command->args[arg][j-1] = 0;
                strncpy(command->args[arg], buffer, j);
                arg++;
                j = 0;    
            }
            if(line.text[i] == '&'){
                command->background = 1;
                break;
            }
        }
    }
    command->length = arg;
    return;
}

void dbg_print_command(command_T * command){
    for(size_t i = 0; i < command->length; i++){
        printf("arg[%d]=\'%s\', ", (int) i, command->args[i]);
    }
    printf("Background: %d\n", command->background);
}


void free_command(command_T * command){
    for(size_t i = 0; i < command->length; i++){
        free(command->args[i]);
        command->args[i] = NULL;
    }
    free(command->args);
    command->args = NULL;
    command->background = 0;
    command->length = 0;
}