#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *copy_string(const char *text) {
    size_t len = strlen(text) + 1;
    char *copy = malloc(len);
    if (copy != NULL) {
        memcpy(copy, text, len);
    }
    return copy;
}

void init_history(History *history) {
    history->count = 0;
    for (int i = 0; i < MAX_HISTORY; i++) {
        history->commands[i] = NULL;
    }
}

void add_history(History *history, const char *line) {
    if (history->count == MAX_HISTORY) {
        free(history->commands[0]);
        for (int i = 1; i < MAX_HISTORY; i++) {
            history->commands[i - 1] = history->commands[i];
        }
        history->count--;
    }

    history->commands[history->count] = copy_string(line);
    if (history->commands[history->count] != NULL) {
        history->count++;
    }
}

void show_history(const History *history) {
    for (int i = 0; i < history->count; i++) {
        printf("%d %s\n", i + 1, history->commands[i]);
    }
}

void free_history(History *history) {
    for (int i = 0; i < history->count; i++) {
        free(history->commands[i]);
        history->commands[i] = NULL;
    }
    history->count = 0;
}
