#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>

#define MAX_INPUT 1024
#define MAX_ARGS 128
#define MAX_HISTORY 100

typedef struct {
    char *commands[MAX_HISTORY];
    int count;
} History;

typedef struct {
    char *argv[MAX_ARGS];
    int argc;
    bool background;
} Command;

typedef struct {
    Command left;
    Command right;
    bool has_pipe;
} ParsedInput;

void shell_loop(void);

void init_history(History *history);
void add_history(History *history, const char *line);
void show_history(const History *history);
void free_history(History *history);

bool parse_input(char *line, ParsedInput *parsed);

int execute_command(const Command *cmd, History *history);
int execute_piped_command(const Command *left, const Command *right);

#endif
