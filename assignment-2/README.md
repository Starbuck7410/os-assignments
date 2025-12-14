# HW2: Dispatcher/Worker Model Implementation

## Features

### Core functionality
This project implements a multi-threaded Dispatcher/Worker model in C using `pthread` library. The program simulates a workload distribution system where a main dispatcher thread reads commands from an input file and either executes them locally or places them into a synchronized queue for a pool of worker threads to process.


### The Dispatcher
The main thread acts as the Dispatcher. Its responsibilities are:
1.  Initialization: Allocates memory, initializes mutexes/condition variables, creates counter files, and spawns worker threads.
2.  Parsing: Reads the command file line-by-line.
3.  Scheduling:
    * Local Commands: Executes `dispatcher_msleep` and `dispatcher_wait` immediately.
    * Job Offloading: Wraps `worker` commands into a `job_T` structure and pushes them into the shared `job_queue`.
4.  Cleanup: Signals threads to exit upon completion and frees resources.

### The Workers
A configurable number of worker threads pull jobs from the shared queue. Their lifecycle involves:
1.  Waiting on a condition variable (`not_empty`) until a job is available.
2.  Dequeuing the job and parsing the specific command string
3.  Executing the logic (File I/O, sleep, or loops).
4.  Updating global statistics and signaling the dispatcher if it is waiting for completion.


### Synchronization
To ensure thread safety and efficiency (no busy waiting), the following primitives are used:

* Job Queue Protection: A mutex (`job_queue.lock`) protects the linked list. A condition variable (`job_queue.not_empty`) is used by workers to sleep when the queue is empty and by the dispatcher to wake them up when a job is added.

* Dispatcher Waiting: A specific condition variable (`ctx.all_jobs_done`) allows the dispatcher to sleep during a `dispatcher_wait` command. It wakes up only when the `pending_jobs` counter reaches zero.
* Statistics Protection: A mutex (`ctx.stats_lock`) ensures that the calculation of min/max/average turnaround times is atomic.

### Mutexes
The program supports up to 100 shared counter files (`count00.txt` to `count99.txt`).
To prevent race conditions without creating a bottleneck, an array of mutexes (`ctx.counter_mutexes`) is used.
* When a thread writes to `count05.txt`, it locks only `counter_mutexes[5]`.
* This allows other threads to simultaneously write to `count02.txt` or `count99.txt`, maximizing concurrency while maintaining data integrity.

### The "Repeat" Logic
The `repeat` command is implemented to repeat the *subsequent* sequence of commands in the job line.
* Logic: When `repeat N` is encountered, the worker enters a loop that iterates `N` times. Inside this loop, it executes all commands remaining in the `job->commands` array.

## Compilation and Usage

### Building
A makefile is provided. To compile the project:
```bash
make
```

To run the project, you can either run the executable:
```bash
./hw2 cmdfile.txt num_threads num_counters log_enabled
```

Or, alternatively, you can run directly with Make:
```bash
make run ARGS="cmdfile.txt num_threads num_counters log_enabled"
```

Sadly, this assignment was too boring for me to implement a fun mode, so there is not fun mode in this one.