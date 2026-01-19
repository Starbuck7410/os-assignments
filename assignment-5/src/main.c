#include "../include/fs.h"  // This one is straight from XV6
#include "../include/blocks.h"  // This one is straight from XV6
#include <stdio.h>
#include <string.h>



int main(int argc, char ** argv){

    FILE * fs_file = fopen("fs.img", "r");
    block_T super_block;
    block_T root_inode_block;
    block_T root_entry_block;
    fseek(fs_file, BSIZE, SEEK_SET);
    fread(&super_block, 1, BSIZE, fs_file);
    
    uint inode_block_addr = 0;
    uint entry_block_addr = 0;

    int i = 0;
    char tabs[3] = { '\t', 0, 0 };
    char name[DIRSIZ];
    uint root_size = BSIZE; // we start with the assumption that the root filelist is contained in one block
    
    while(i < root_size / 16){ 
        if(entry_block_addr != (1 + BBLOCK(i, super_block.super_block)) * BSIZE) {
            inode_block_addr = (1 + BBLOCK(i, super_block.super_block)) * BSIZE;
            fseek(fs_file, inode_block_addr, SEEK_SET);
            fread(&root_entry_block, 1, BSIZE, fs_file);
        }
        if(root_entry_block.filelist.files[i % FILESINBLOCK].inum == 0) break;

        uint index = root_entry_block.filelist.files[i % FILESINBLOCK].inum;
        if(inode_block_addr != IBLOCK(index, super_block.super_block)) {
            inode_block_addr = (IBLOCK(index, super_block.super_block)) * BSIZE;
            fseek(fs_file, inode_block_addr, SEEK_SET);
            fread(&root_inode_block, 1, BSIZE, fs_file);
        }
        
        strncpy(name, root_entry_block.filelist.files[i % FILESINBLOCK].name, DIRSIZ);
        ushort type = root_inode_block.di_node[index % IPB].type;
        uint size = root_inode_block.di_node[index % IPB].size;
        if(index == 1) root_size = size; // and then override this assumption if it's false
        tabs[1] = (strlen(root_entry_block.filelist.files[i].name) < 8) ? '\t' : 0;
        printf("%s%s%d %d %d\n", name, tabs, type, index, size);
        i++;
    }
    
}