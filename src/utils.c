#include "utils.h"

#include <stdio.h>
#include <string.h>

void trim_newline(char *text) {
    text[strcspn(text, "\n")] = '\0';
}

void print_prompt(void) {
    printf("mini-shell$ ");
    fflush(stdout);
}
