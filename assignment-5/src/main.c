#include "../include/blocks.h"  
#include "../include/functions.h"
#include <stdio.h>
#include <string.h>

typedef enum {
    CMD_ERROR,
    CMD_LS,
    CMD_CP // Cyberpunk???
} command_T;




int main(int argc, char ** argv){
    command_T command = CMD_ERROR;
    if(argc == 3){
        if(strcmp("ls", argv[2]) == 0){
            command = CMD_LS;
        }
    }
    if(argc == 5){
        if(strcmp("cp", argv[2]) == 0){
            command = CMD_CP;
        }
    }
    if(command == CMD_ERROR) {
        printf("ERROR: Wrong paramters.\n");
        printf("Usage: hw5 <image file> ls\n");
        printf("OR\n");
        printf("Usage: hw5 <image file> cp <file in image> <name in filesystem>\n");
        return 1;
    }

    FILE * fs_file = fopen(argv[1], "r");
    if(!fs_file){
        perror("Can't open source filesystem file");
        return 1;
    }
    file_blocks_T blocks = {0};
    fseek(fs_file, BSIZE, SEEK_SET);
    fread(&blocks.super, 1, sizeof(struct superblock), fs_file);
    int error = 0;
    if(command == CMD_LS){
        if(find_files(&blocks, fs_file, NULL) == -1) error = 1;
        
    }
    if(command == CMD_CP){
        if(cp(&blocks, fs_file, argv[4], argv[3]) == -1) error = 1;
    }


    return error;
    
}
