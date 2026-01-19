#pragma once
#include "../include/fs.h"  // This one is straight from XV6
#define FILEITEMSIZE 16



typedef struct {
    struct dirent files[(BSIZE - sizeof(struct dinode))/ FILEITEMSIZE];
} filelist_T;

typedef union {
    char raw_bytes[BSIZE];
    struct superblock super_block;
    struct dinode di_node[IPB];
    filelist_T filelist;
} block_T;