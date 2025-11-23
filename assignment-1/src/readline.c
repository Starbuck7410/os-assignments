#include "../include/readline.h"
#include "../include/processes.h"
#include <stdlib.h>
#include <string.h>


line_T read_line(char * prompt){
    int eof = 0;
    #ifdef FUNMODE
    char cwd[MAX_PROMPT];
    char replaced_prompt[MAX_PROMPT];
    strcpy(replaced_prompt, prompt);
    getcwd(cwd, MAX_PROMPT);
    replace_substring(replaced_prompt, "$CWD", cwd);
    printf("%s", replaced_prompt);
    #else
    printf("%s", prompt);
    #endif
    fflush(stdout);
    char * line_buffer = malloc(MAX_BUFFER);
    size_t length = read(STDIN_FILENO, line_buffer, MAX_BUFFER);
    check_errors("read");
    if(length > MAX_BUFFER){
        printf("Warning: line is too long\n");
        length = 1024;
    }
    if(length < 1) {
        eof = 1;
    }else{
        if(line_buffer[length - 1] != '\n') eof = 1;
        length--; // Length refers to the length of the line, not the terminated string 
        line_buffer[length] = 0;
    }
    line_T final_line = {
        .length = length,
        .text = line_buffer,
        .eof = eof
    };
    return final_line;
}


void line_to_command(command_T * command, line_T line){
    command->args = malloc(MAX_ARGS * sizeof(char *));
    int arg = 0;
    int j = 0;
    char buffer[MAX_BUFFER];
    for(size_t i = 0; i < line.length + 1; i++){
        char c = line.text[i];
        if(c != ' ' && c != 0 && c != '&'){
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


void clear_command(command_T * command){
    for(size_t i = 0; i < command->length; i++){
        free(command->args[i]);
        command->args[i] = NULL;
    }
    free(command->args);
    command->args = NULL;
    command->background = 0;
    command->length = 0;
}


void clear_line(line_T * line){
    free(line->text);
    line->length = 0;
}


int string_to_pos_int(char * string){
    if(string == NULL) return -1;
    char c = string[0];
    int value = 0;
    int i = 0;
    while(48 <= c && c <= 57){
        value *= 10;
        value += c - '0';
        i++;
        c = string[i];
    }
    return value;
}

// There is a bug with this function, 'replacereplace' only replaces one instance
int replace_substring(char * origin, char * replace, char * target){ // replace 'replace' with 'target'
    size_t origin_length = strlen(origin);
    size_t replace_length = strlen(replace);
    size_t target_length = strlen(target);
    int count = 0;
    for(size_t i = 0; i < origin_length - replace_length + 1; i++){
        if(strncmp(origin + i, replace, replace_length) == 0){
            size_t delta_length = target_length - replace_length;       
            for(size_t j = origin_length; j > i; j--){
                origin[j + delta_length] = origin[j];  
            }
            for(size_t j = 0; j < target_length; j++){
                origin[i + j] = target[j]; 
            }
            count++;
        }
    }
    return count;
}