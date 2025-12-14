#include "../include/worker.h"
#include "../include/files.h"

context_T ctx;
queue_T job_queue;

int main(int argc, char ** argv){
    if(argc < 5){
        fprintf(stderr, "Correct syntax is \"hw2 cmdfile.txt num_threads num_counters log_enabled\"\n");
        return 1;
    }

    char * cmd_filename = argv[1];
    ctx.num_threads = atoi(argv[2]); 
    ctx.num_counters = atoi(argv[3]); 
    ctx.log_enabled = atoi(argv[4]); 
    
    if (ctx.num_threads > MAX_THREADS) ctx.num_threads = MAX_THREADS;
    if (ctx.num_counters > MAX_COUNTERS) ctx.num_counters = MAX_COUNTERS;

   
    ctx.start_time = get_current_time_ms();
    ctx.pending_jobs = 0;
    ctx.shutdown = 0;
    ctx.total_turnaround = 0;
    ctx.min_turnaround = -1;
    ctx.max_turnaround = 0;
    ctx.jobs_completed = 0;

    pthread_mutex_init(&ctx.stats_lock, NULL);
    pthread_cond_init(&ctx.all_jobs_done, NULL);


    for(int i=0; i<ctx.num_counters; i++) {
        pthread_mutex_init(&ctx.counter_mutexes[i], NULL);
    }

    job_queue.head = NULL;
    job_queue.tail = NULL;
    job_queue.size = 0;
    pthread_mutex_init(&job_queue.lock, NULL);
    pthread_cond_init(&job_queue.not_empty, NULL);


    FILE ** files = create_counter_files(ctx.num_counters);

    if (ctx.log_enabled) {
        ctx.dispatcher_log = fopen("dispatcher.txt", "w");
    }


    pthread_t threads[MAX_THREADS];
    for (int i = 0; i < ctx.num_threads; i++) {
        pthread_create(&threads[i], NULL, worker_function, (void*)(long)i);
    }


    FILE * cmd_file = fopen(cmd_filename, "r");
    if (!cmd_file) {
        perror("Error opening command file");
        return 1;
    }

    char line[MAX_LINE];
    while (fgets(line, MAX_LINE, cmd_file)) {
        strip_newline(line);
        if (strlen(line) == 0) continue;

        long long now = get_time_diff();
        
        if (ctx.log_enabled) {
            fprintf(ctx.dispatcher_log, "TIME %lld: read cmd line: %s\n", now, line);
            fflush(ctx.dispatcher_log); 
        }

        char cmd_type[32];
        sscanf(line, "%s", cmd_type);

        if (strcmp(cmd_type, "dispatcher_msleep") == 0) {
            int duration;
            sscanf(line, "%*s %d", &duration);
            usleep(duration * 1000);
        } 
        else if (strcmp(cmd_type, "dispatcher_wait") == 0) {
            pthread_mutex_lock(&ctx.stats_lock);
            while (ctx.pending_jobs > 0) {
                pthread_cond_wait(&ctx.all_jobs_done, &ctx.stats_lock);
            }
            pthread_mutex_unlock(&ctx.stats_lock);
        } 
        else if (strcmp(cmd_type, "worker") == 0) {

            char * job_content = strstr(line, "worker");
            if (job_content) job_content += 7; 
            
            job_T * new_job = parse_job_line(job_content);

            pthread_mutex_lock(&job_queue.lock);
            if (job_queue.tail) {
                job_queue.tail->next = new_job;
                job_queue.tail = new_job;
            } else {
                job_queue.head = new_job;
                job_queue.tail = new_job;
            }
            job_queue.size++;
            
            pthread_mutex_lock(&ctx.stats_lock);
            ctx.pending_jobs++;
            pthread_mutex_unlock(&ctx.stats_lock);
            
            pthread_cond_signal(&job_queue.not_empty); 
            pthread_mutex_unlock(&job_queue.lock);
        }
    }
    fclose(cmd_file);

    pthread_mutex_lock(&ctx.stats_lock);
    while (ctx.pending_jobs > 0) {
        pthread_cond_wait(&ctx.all_jobs_done, &ctx.stats_lock);
    }
    pthread_mutex_unlock(&ctx.stats_lock);

    pthread_mutex_lock(&job_queue.lock);
    ctx.shutdown = 1;
    pthread_cond_broadcast(&job_queue.not_empty);
    pthread_mutex_unlock(&job_queue.lock);

    for (int i = 0; i < ctx.num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    FILE * stats_file = fopen("stats.txt", "w");
    long long total_run_time = get_current_time_ms() - ctx.start_time;
    double avg_turnaround = (ctx.jobs_completed > 0) ? (double)ctx.total_turnaround / ctx.jobs_completed : 0.0;
    
    if (ctx.min_turnaround == -1) ctx.min_turnaround = 0;

    fprintf(stats_file, "total running time: %lld milliseconds\n", total_run_time);
    fprintf(stats_file, "sum of jobs turnaround time: %lld milliseconds\n", ctx.total_turnaround);
    fprintf(stats_file, "min job turnaround time: %lld milliseconds\n", ctx.min_turnaround);
    fprintf(stats_file, "average job turnaround time: %f milliseconds\n", avg_turnaround);
    fprintf(stats_file, "max job turnaround time: %lld milliseconds\n", ctx.max_turnaround);
    fclose(stats_file);

    if (ctx.log_enabled && ctx.dispatcher_log) fclose(ctx.dispatcher_log);
    free(files);
    return 0;   
}