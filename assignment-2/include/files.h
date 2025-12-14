#include <stdio.h>
#include <stdlib.h>
#define MAX_FILENAME 13


FILE ** create_counter_files(int count);
void close_counter_files(FILE ** files, int count);
void strip_newline(char * str);