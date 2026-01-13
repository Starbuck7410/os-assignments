# Assignment 4 documentation: Operating Systems

## Foreword
Before we begin with the actual documentation, I would like to rant a little bit about our use of the old, x86 version of xv6.\
Here is a note from the README file in the original repository:

```text <!-- Does this even work to remove syntax highlighting? lol -->
NOTE: we have stopped maintaining the x86 version of xv6, and switched
our efforts to the RISC-V version
(https://github.com/mit-pdos/xv6-riscv.git)
```

Considering the RISC-V version is more well written, and actually compiles on modern versions of GCC, we believe it is quite a reasonable idea to switch to that version in the future.

Perhaps, if we feel advenurous, we might even re-implement the required syscalls and program in the RISC-V version. For funsies.

## Description

The primary objective of this assignment was to extend the xv6 kernel's capabilities.
By default, the xv6 operating system provides limited visibility into active processes from the user-space.
This project involved implementing a new system call mechanism that bridges the gap between the kernel's process table and the user's terminal, by implementing new system calls, and a new user side program, written with function that utilize these calls.

Specifically, we focused on:
- Exposing internal process metrics—such as Parent Process ID (PPID), and process state—to user-land
- Implementing a tracking mechanism within the scheduler to count context switches
- Developing a custom `ps` (process status) utility that invokes these new kernel functions to display a human-readable table of system activity.


## Changed files

- `usys.S`: Added syscall definitions
- `user.h`: Added usermode function declarations, and also strncpy for secure string copying
- `ulib.c`: Added implementation of strncpy (simply taken from `string.c`)
- `sysproc.c`: Added syscall implementations
- `syscall.h`: Added syscall numbers
- `syscall.c`: Added syscall definitions to the list of syscalls  
- `README.md`: This file in markdown format (will be converted to PDF and submitted)
- `ps.c`: Implemented the ps command to show the running processes
- `processInfo.h`: Added the processInfo struct
- `proc.h`: Added field nrswitch to struct proc
- `Makefile`: Added ps program, and some minor compatability changes

### Notes

This PDF might look different from previous assignments due to a new process for converting markdown to PDF.\
This assignment was by far the most interesting one, but due to some [(ahem)](#foreword) constraints it also proved to be the hardest one. 