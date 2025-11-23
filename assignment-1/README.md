# hw1shell

`hw1shell` is a small Unix-like command interpreter written in C.\
It supports basic job control, background processes, prompt
customization (in fun mode), built-in commands, and execution of
external programs.

## Features

### Core functionality

-   Reads input and parses it into commands / arguments
-   Executes external commands via fork / exec
-   Supports background execution using `&`
-   Built-in commands:
    -   `cd <dir>`: change directory
    -   `exit`: exit the shell
    -   `jobs`: show running background jobs

### Process tracking

-   Maintains a process list for background tasks
-   Prints notifications when background jobs finish
-   Cleans up zombie processes using `waitpid(..., WNOHANG)`

### Clean design

-   Self-documenting variable and function names
-   Error checking via `check_errors`
-   Clear separation of parsing, execution, and cleanup logic

## Fun Mode (optional)

If compiled with `-DFUNMODE`, extra features become available:

### Extra commands

-   `chprompt <new_prompt>`: change the shell prompt
-   `fg <n>`: bring background job *n* to the foreground

### Extra behavior

-   A custom SIGINT (Ctrl+C) handler that kills the active foreground
    job
-   Fancier output for the `jobs` command
-   Ability to set `$CWD` in `chprompt` to show the current working directory

## Building

### Standard build
```
make
```
### Build and run
```
make run
```

### Fun mode build and run
```
make fun
```

`make fun` simply compiles with the `-DFUNMODE` flag and runs the shell program.

## Running
```
./hw1shell
```
To exit:
```
    exit
```
Or, alternatively you can hit ctrl + D like in bash.

## Project Structure
```
├── (hw1shell)
├── makefile
├── README.md
├── include
│   ├── processes.h
│   └── readline.h
└── src
    ├── processes.c
    ├── readline.c
    └── shell.c
```
## Known Limitations

-   No command history
-   No piping or redirection
-   No aliases
-   No .hw1shellrc / `source` internal command

## Notes

This shell was developed for an academic assignment and focuses on: -
Readable, self-documenting C code
- Understanding job control, processes, and signals
- Minimal comments where naming and structure suffice
