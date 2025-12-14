#include "../include/worker.h"
#include "../include/files.h"

#ifdef DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) ;
#endif

int main(int argc, char ** argv){
    if(argc < 5){
        fprintf(stderr, "Not enough arguments.\n");
        fprintf(stderr, "Correct syntax is \"hw2 cmdfile.txt num_threads num_counters log_enabled\"\n");
        return 1;
    }
    int files_count = (int) strtol(argv[2], NULL, 10);
    FILE ** files = create_counter_files(files_count); 
    DEBUG_PRINT("asd%d\n", 123);
    close_counter_files(files, files_count);
    return 0;

}