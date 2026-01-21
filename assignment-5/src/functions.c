#include "../include/functions.h"
#include "../include/fs.h"
#include "../include/blocks.h"
int find_files(file_blocks_T * blocks, FILE * fs_file, char * search_name){ // if search name is null its ls, else it finds the file!

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

int cp(file_blocks_T * blocks, FILE * fs_file, char * dest_name, char * src_name){
    int found_index = find_files(blocks, fs_file, src_name);
    if(found_index < 0){
        printf("File %s not found!\n", src_name);
        return -1;
    }
    int ovf = (blocks->nodes[found_index % IPB].size % BSIZE) ? 1 : 0;
    int size = (blocks->nodes[found_index % IPB].size / BSIZE) + ovf;
    uint block_addresses[size];

    int max_j = ((size < (NDIRECT + 1)) ? size : (NDIRECT + 1));
    for(int j = 0; j < max_j; j++){
        block_addresses[j] = blocks->nodes[found_index % IPB].addrs[j] * BSIZE;
    }

    if(size > NDIRECT){
        uint indirect_nodes[BSIZE / sizeof(uint)];
        uint indirect_address = blocks->nodes[found_index % IPB].addrs[NDIRECT] * BSIZE;
        fseek(fs_file, indirect_address, SEEK_SET);
        fread(indirect_nodes, sizeof(uint), BSIZE / sizeof(uint), fs_file);  
        for(int i = 0; i < size - NDIRECT; i++){
            block_addresses[i + (NDIRECT + 1)] = indirect_nodes[i] * BSIZE;
        }
    }
    char * file_bytes = malloc(BSIZE * size);
    if(!file_bytes){
        perror("Can't allocate memory for the file");
        return -1;
    }
    for(int i = 0; i < size; i++){
        fseek(fs_file, block_addresses[i], SEEK_SET);
        fread(file_bytes + i * BSIZE, 1, BSIZE, fs_file);  
    }
    
   
    FILE * dest_file = fopen(dest_name, "w");
    if(!dest_file){
        perror("Can't open destination file");
        return -1;
    }
    fwrite(file_bytes, 1, blocks->nodes[found_index % IPB].size, dest_file);
    fclose(dest_file);
    free(file_bytes);
    return found_index;
}