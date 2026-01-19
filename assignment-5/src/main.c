#include "../include/fs.h"  // This one is straight from XV6
#include "../include/blocks.h"  
#include "../include/functions.h"
#include <stdio.h>
#include <string.h>

typedef enum {
    CMD_ERROR,
    CMD_LS,
    CMD_CP // Cyberpunk???
} command_T;


int cp(file_blocks_T * blocks, FILE * fs_file, char * dest_name, char * src_name);

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
        printf("Usage: hw5 <image file> <command> [output file]\n");
        return 1;
    }

    FILE * fs_file = fopen(argv[1], "r");
    file_blocks_T blocks = {0};
    fseek(fs_file, BSIZE, SEEK_SET);
    fread(&blocks.super, 1, sizeof(struct superblock), fs_file);
    
    if(command == CMD_LS){
        ls(&blocks, fs_file, NULL);
    }
    if(command == CMD_CP){
        cp(&blocks, fs_file, argv[4], argv[3]);
    }


    return 0;
    
}





int cp(file_blocks_T * blocks, FILE * fs_file, char * dest_name, char * src_name){
    int found_index = ls(blocks, fs_file, src_name);
    if(found_index >= 0){
        printf("File found!\n");
        for(int j = 0; j < NDIRECT + 1; j++){
            printf("address[%d] = %u\n", j, blocks->nodes[found_index % IPB].addrs[j]);
        }
        printf("nlink = %u\n", blocks->nodes[found_index % IPB].nlink);
        int ovf = (blocks->nodes[found_index % IPB].size % BSIZE) ? 1 : 0;
        int size = (blocks->nodes[found_index % IPB].size / BSIZE) + ovf;
        printf("blocks = %u\n", size);
        FILE * dest_file = fopen(dest_name, "w");
    }else{
        printf("File %s not found!\n", src_name);
    }
    return found_index;
}