#pragma once
#include "../include/blocks.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cp(file_blocks_T * blocks, FILE * fs_file, char * dest_name, char * src_name);
int find_files(file_blocks_T * blocks, FILE * fs_file, char * search_name);