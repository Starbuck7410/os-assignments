#pragma once
#include "../include/fs.h"
#include "../include/blocks.h"
#include <stdio.h>
#include <string.h>

int ls(file_blocks_T * blocks, FILE * fs_file, char * search_name);