# Mini Linux Shell in C

A beginner-friendly yet resume-worthy mini shell built in C for Linux systems. This project demonstrates process management, command parsing, and inter-process communication using core Unix system calls.

## Features

- Runs on Linux terminal
- Executes basic commands (`ls`, `pwd`, `echo`, etc.)
- Supports background processes using `&`
- Supports single piping (`command1 | command2`)
- Maintains in-memory command history (`history` command)
- Built-in commands:
  - `cd <path>`
  - `history`
  - `exit`

## System Calls Used

- `fork()` - creates child process
- `execvp()` - replaces child process image with a command
- `waitpid()` - waits for child process completion
- `pipe()` - creates communication channel between two commands
## Project Structure

```
Mini-Linux-Shell-in-C/
├── include/
│   ├── shell.h
│   └── utils.h
├── src/
│   ├── main.c
│   ├── shell.c
│   ├── parser.c
│   ├── executor.c
│   ├── history.c
│   └── utils.c
├── Makefile
└── README.md
```

## File-by-File Explanation

### `include/shell.h`
- Declares constants (`MAX_INPUT`, `MAX_ARGS`, `MAX_HISTORY`)
- Defines core structs:
  - `History`
  - `Command`
  - `ParsedInput`
- Declares shell loop, parser, history, and executor function prototypes.

### `include/utils.h`
- Declares helper functions for prompt printing and newline trimming.

### `src/main.c`
- Entry point of the program.
- Calls `shell_loop()` to start the shell.

### `src/shell.c`
- Contains `shell_loop()`.
- Reads user input, stores history, calls parser, and dispatches execution.
- Handles EOF (`Ctrl+D`) gracefully.

### `src/parser.c`
- Splits raw input into commands and arguments.
- Detects `|` for piping and `&` for background processes.
- Produces a `ParsedInput` object consumed by the executor.

### `src/executor.c`
- Implements built-in command handling (`cd`, `history`, `exit`).
- Uses `fork()` + `execvp()` + `waitpid()` for normal commands.
- Uses `pipe()` and two child processes for piped commands.

### `src/history.c`
- Manages in-memory history buffer.
- Stores up to `MAX_HISTORY` commands.
- Frees allocated memory on exit.

### `src/utils.c`
- Helper utilities:
  - `trim_newline()`
  - `print_prompt()`

## Function-by-Function Explanation

### `shell_loop()`
Main REPL loop:
1. Print prompt
2. Read line
3. Save to history
4. Parse
5. Execute (single command or pipe)

### `parse_input()`
- Checks if input contains a pipe (`|`).
- Parses either one or two commands.
- Returns `false` on malformed input.

### `execute_command()`
- Handles built-ins.
- For external commands:
  - `fork()` child
  - child: `execvp()`
  - parent: `waitpid()` unless background mode

### `execute_piped_command()`
- Creates pipe.
- Forks left command process and redirects stdout to pipe.
- Forks right command process and redirects stdin from pipe.
- Parent waits for both children.

### `add_history()`
- Appends new command to history.
- If full, shifts old entries and keeps only latest `MAX_HISTORY`.

### `show_history()`
- Prints stored commands with line numbers.

### `free_history()`
- Releases dynamically allocated command strings.

### `trim_newline()`
- Removes trailing newline from `fgets()` input.

### `print_prompt()`
- Displays shell prompt (`mini-shell$ `).

## Build and Run

```bash
make
./mini_shell
```

Optional:
```bash
make run
make clean
```

## Example Session

```bash
mini-shell$ pwd
/home/user/projects/Mini-Linux-Shell-in-C

mini-shell$ ls | wc -l
8

mini-shell$ sleep 5 &
[Background] PID: 12345

mini-shell$ history
1 pwd
2 ls | wc -l
3 sleep 5 &
4 history

mini-shell$ exit
```

## Resume Bullet Ideas

- Built a modular Linux shell in C implementing process creation and management using `fork`, `execvp`, and `waitpid`.
- Implemented command piping via Unix `pipe` and file descriptor redirection (`dup2`).
- Added background job execution and command history subsystem with bounded memory strategy.
- Designed clean project structure with reusable parser/executor/history modules and Makefile-based build.
