#include "../include/files.h"
#include "../include/worker.h"

FILE ** create_counter_files(int count){
    if(count < 1 || count > 100){
        fprintf(stderr, "You are trying to create more than 100 files!\n");
        perror("create_counter_files()");
        return NULL;
    }
    FILE ** files = malloc(count * sizeof(FILE *));
    char file_name[MAX_FILENAME];
    for(int i = 0; i < count; i++){
        snprintf(file_name, MAX_FILENAME, "count%02d.txt", i);
        files[i] = fopen(file_name, "w");
        if (!files[i]){
            fprintf(stderr, "Can't open file: %s\n", file_name);
            perror("create_counter_files()");
            break;
        }

        fprintf(files[i],"%d", 0);
    }
    for(int i = 0; i < count; i++){
       fclose(files[i]);
    }
    return files;
}

void strip_newline(char * str) {
    str[strcspn(str, "\r\n")] = 0;
}

