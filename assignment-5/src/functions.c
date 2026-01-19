#include "../include/functions.h"

int ls(file_blocks_T * blocks, FILE * fs_file, char * search_name){ // if search name is null its ls, else it finds the file!

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
        if(search_name == NULL){
            tabs[1] = (strlen(blocks->file_list.files[i % FILESINBLOCK].name) < 8) ? '\t' : 0;
            printf("%s%s%d %d %d\n", name, tabs, type, index, size);
        }else if(strcmp(name, search_name) == 0){
            return (int) index;
        }
        i++;
    }
    return -1;
}