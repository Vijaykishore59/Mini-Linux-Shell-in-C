#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int run_builtin(const Command *cmd, History *history) {
    if (strcmp(cmd->argv[0], "cd") == 0) {
        const char *path = (cmd->argc > 1) ? cmd->argv[1] : getenv("HOME");
        if (chdir(path) != 0) {
            perror("cd");
        }
        return 1;
    }

    if (strcmp(cmd->argv[0], "history") == 0) {
        show_history(history);
        return 1;
    }

    if (strcmp(cmd->argv[0], "exit") == 0) {
        return -1;
    }

    return 0;
}

int execute_command(const Command *cmd, History *history) {
    int builtin_result = run_builtin(cmd, history);
    if (builtin_result != 0) {
        return builtin_result;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    if (cmd->background) {
        printf("[Background] PID: %d\n", pid);
        return 1;
    }

    waitpid(pid, NULL, 0);
    return 1;
}

int execute_piped_command(const Command *left, const Command *right) {
    int pipefd[2];

    if (pipe(pipefd) < 0) {
        perror("pipe");
        return 1;
    }

    pid_t left_pid = fork();
    if (left_pid < 0) {
        perror("fork");
        return 1;
    }

    if (left_pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execvp(left->argv[0], left->argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    pid_t right_pid = fork();
    if (right_pid < 0) {
        perror("fork");
        return 1;
    }

    if (right_pid == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        execvp(right->argv[0], right->argv);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);

    return 1;
}
