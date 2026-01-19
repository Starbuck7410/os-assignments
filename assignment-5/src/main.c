#include "../include/fs.h"  // This one is straight from XV6
#include "../include/blocks.h"  // This one is straight from XV6
#include <stdio.h>
#include <string.h>

typedef enum {
    CMD_ERROR,
    CMD_LS,
    CMD_CP // Cyberpunk???
} command_T;

void ls(file_blocks_T * blocks, FILE * fs_file);
void cp(file_blocks_T * blocks, FILE * fs_file, FILE * dest_file);

int main(int argc, char ** argv){
    command_T command = CMD_ERROR;
    if(argc == 3){
        if(strcmp("ls", argv[2]) == 0){
            command = CMD_LS;
        }
    }
    if(argc == 4){
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
        ls(&blocks, fs_file);
    }
    if(command == CMD_CP){
        FILE * dest_file = fopen(argv[3], "w");
        cp(&blocks, fs_file, dest_file);
    }

    return 0;
    
}


void ls(file_blocks_T * blocks, FILE * fs_file){

    uint inode_block_addr = 0;
    uint entry_block_addr = 0;

    uint i = 0;
    char tabs[3] = { '\t', 0, 0 };
    char name[DIRSIZ + 1];
    name[DIRSIZ] = 0;
    uint root_size = BSIZE; // we start with the assumption that the root filelist is contained in one block
    
    while(i < root_size / 16){ 
        if(entry_block_addr != (1 + BBLOCK(i, blocks->super)) * BSIZE) {
            inode_block_addr = (1 + BBLOCK(i, blocks->super)) * BSIZE;
            fseek(fs_file, inode_block_addr, SEEK_SET);
            fread(&blocks->file_list, 1, BSIZE, fs_file);
        }
        if(blocks->file_list.files[i % FILESINBLOCK].inum == 0) break;

        uint index = blocks->file_list.files[i % FILESINBLOCK].inum;
        if(inode_block_addr != IBLOCK(index, blocks->super)) {
            inode_block_addr = (IBLOCK(index, blocks->super)) * BSIZE;
            fseek(fs_file, inode_block_addr, SEEK_SET);
            fread(&blocks->nodes, 1, BSIZE, fs_file);
        }
        
        strncpy(name, blocks->file_list.files[i % FILESINBLOCK].name, DIRSIZ);
        ushort type = blocks->nodes[index % IPB].type;
        uint size = blocks->nodes[index % IPB].size;
        if(index == 1) root_size = size; // and then override this assumption if it's false
        tabs[1] = (strlen(blocks->file_list.files[i % FILESINBLOCK].name) < 8) ? '\t' : 0;
        printf("%s%s%d %d %d\n", name, tabs, type, index, size);
        i++;
    }
}


void cp(file_blocks_T * blocks, FILE * fs_file, FILE * dest_file){

}