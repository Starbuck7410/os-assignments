#include "../include/worker.h"
#include "../include/files.h" 


extern context_T ctx;
extern queue_T job_queue;


long long get_time_diff() {
    return get_current_time_ms() - ctx.start_time;
}


long long get_current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000);
}


void update_counter_file(int counter_id, int value) {
    if(counter_id < 0 || counter_id >= ctx.num_counters) return;

    pthread_mutex_lock(&ctx.counter_mutexes[counter_id]);

    char filename[MAX_FILENAME];
    snprintf(filename, MAX_FILENAME, "count%02d.txt", counter_id);

    FILE * f = fopen(filename, "r+"); 
    if (f) {
        long long current_val = 0;
        if (fscanf(f, "%lld", &current_val) == 1) {
            current_val += value;
            rewind(f);
            fprintf(f, "%lld", current_val);
        }
        fclose(f);
    } else {
        perror("Error updating counter file");
    }

    pthread_mutex_unlock(&ctx.counter_mutexes[counter_id]);
}

void execute_commands(job_T * job) {
    for (size_t i = 0; i < job->length; i++) {
        command_T cmd = job->commands[i];
        
        switch (cmd.operation) {
            case CMD_MSLEEP:
                usleep(cmd.argument * 1000); 
                break;
            case CMD_INCREMENT:
                update_counter_file(cmd.argument, 1); 
                break;
            case CMD_DECREMENT:
                update_counter_file(cmd.argument, -1); 
                break;
            case CMD_REPEAT:
                int repeat_count = cmd.argument;
                for(int r = 0; r < repeat_count; r++) {
                    for(size_t k = i + 1; k < job->length; k++) {
                        command_T sub_cmd = job->commands[k];
                        if(sub_cmd.operation == CMD_MSLEEP) usleep(sub_cmd.argument * 1000);
                        else if(sub_cmd.operation == CMD_INCREMENT) update_counter_file(sub_cmd.argument, 1);
                        else if(sub_cmd.operation == CMD_DECREMENT) update_counter_file(sub_cmd.argument, -1);
                    }
                }
                return; 
            default:
                break;
        }
    }
}


void * worker_function(void * arg) {
    long thread_id = (long)arg;

    FILE * log_file = NULL;
    if (ctx.log_enabled) {
        char log_name[32];
        snprintf(log_name, 32, "thread%02ld.txt", thread_id);
        log_file = fopen(log_name, "w");
    }

    while (1) {
        job_T * job = NULL;

        pthread_mutex_lock(&job_queue.lock);
        while (job_queue.size == 0 && !ctx.shutdown) {
            pthread_cond_wait(&job_queue.not_empty, &job_queue.lock); // 
        }

        if (ctx.shutdown && job_queue.size == 0) {
            pthread_mutex_unlock(&job_queue.lock);
            break;
        }
        job = job_queue.head;
        if (job) {
            job_queue.head = job->next;
            if (!job_queue.head) job_queue.tail = NULL;
            job_queue.size--;
        }
        pthread_mutex_unlock(&job_queue.lock);

        if (!job) continue;

        long long start_ms = get_time_diff();
        if (log_file) {
            fprintf(log_file, "TIME %lld: START job %s\n", start_ms, job->original_line);
            fflush(log_file);
        }
        execute_commands(job);

        long long end_ms = get_time_diff();
        if (log_file) {
            fprintf(log_file, "TIME %lld: END job %s\n", end_ms, job->original_line);
            fflush(log_file);
        }

        long long turnaround = end_ms - (job->read_time - ctx.start_time);

        pthread_mutex_lock(&ctx.stats_lock);
        ctx.total_turnaround += turnaround;
        ctx.jobs_completed++;
        if (turnaround < ctx.min_turnaround || ctx.min_turnaround == -1) ctx.min_turnaround = turnaround;
        if (turnaround > ctx.max_turnaround) ctx.max_turnaround = turnaround;
        
        ctx.pending_jobs--;
        if (ctx.pending_jobs == 0) {
            pthread_cond_signal(&ctx.all_jobs_done);
        }
        pthread_mutex_unlock(&ctx.stats_lock);

        free_job(job);
    }

    if (log_file) fclose(log_file);
    return NULL;
}

void free_job(job_T * job) {
    if (job) {
        if (job->commands) free(job->commands);
        if (job->original_line) free(job->original_line);
        free(job);
    }
}

job_T * parse_job_line(char * line) {
    job_T * job = malloc(sizeof(job_T));
    job->read_time = get_current_time_ms();
    job->original_line = strdup(line);
    job->next = NULL;
    
    int count = 0;
    char * temp_line = strdup(line);
    char * token = strtok(temp_line, ";");
    while (token) {
        count++;
        token = strtok(NULL, ";");
    }
    free(temp_line);

    job->length = count;
    job->commands = malloc(sizeof(command_T) * count);

    temp_line = strdup(line);
    token = strtok(temp_line, ";");
    int i = 0;
    while (token && i < count) {

        char op_str[64];
        int arg = 0;
        sscanf(token, "%s %d", op_str, &arg);

        if (strcmp(op_str, "msleep") == 0) job->commands[i].operation = CMD_MSLEEP;
        else if (strcmp(op_str, "increment") == 0) job->commands[i].operation = CMD_INCREMENT;
        else if (strcmp(op_str, "decrement") == 0) job->commands[i].operation = CMD_DECREMENT;
        else if (strcmp(op_str, "repeat") == 0) job->commands[i].operation = CMD_REPEAT;
        else job->commands[i].operation = CMD_INVALID;

        job->commands[i].argument = arg;
        
        token = strtok(NULL, ";");
        i++;
    }
    free(temp_line);
    return job;
}