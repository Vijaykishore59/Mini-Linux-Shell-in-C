#include "shell.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

static void init_command(Command *cmd) {
    cmd->argc = 0;
    cmd->background = false;
    for (int i = 0; i < MAX_ARGS; i++) {
        cmd->argv[i] = NULL;
    }
}

static bool parse_single_command(char *text, Command *cmd) {
    init_command(cmd);

    char *token = strtok(text, " \t");
    while (token != NULL && cmd->argc < MAX_ARGS - 1) {
        cmd->argv[cmd->argc++] = token;
        token = strtok(NULL, " \t");
    }

    if (cmd->argc == 0) {
        return false;
    }

    cmd->argv[cmd->argc] = NULL;

    if (strcmp(cmd->argv[cmd->argc - 1], "&") == 0) {
        cmd->background = true;
        cmd->argv[cmd->argc - 1] = NULL;
        cmd->argc--;
        if (cmd->argc == 0) {
            return false;
        }
    }

    return true;
}

bool parse_input(char *line, ParsedInput *parsed) {
    parsed->has_pipe = false;

    char *pipe_pos = strchr(line, '|');
    if (pipe_pos == NULL) {
        return parse_single_command(line, &parsed->left);
    }

    *pipe_pos = '\0';
    char *left_part = line;
    char *right_part = pipe_pos + 1;

    while (isspace((unsigned char)*left_part)) {
        left_part++;
    }
    while (isspace((unsigned char)*right_part)) {
        right_part++;
    }

    if (strlen(left_part) == 0 || strlen(right_part) == 0) {
        return false;
    }

    parsed->has_pipe = true;
    return parse_single_command(left_part, &parsed->left) &&
           parse_single_command(right_part, &parsed->right);
}
