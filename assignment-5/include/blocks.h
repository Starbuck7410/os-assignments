#pragma once
#include "../include/fs.h"  // This one is straight from XV6
#define FILEITEMSIZE 16



typedef struct {
    struct dirent files[BSIZE/FILEITEMSIZE];
} filelist_T;

typedef struct {
    struct superblock super;
    struct dinode nodes[IPB];
    filelist_T file_list;
} file_blocks_T;