#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void shell_loop(void) {
    History history;
    char input[MAX_INPUT];

    init_history(&history);

    while (1) {
        print_prompt();

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nExiting shell...\n");
            break;
        }

        trim_newline(input);

        if (strlen(input) == 0) {
            continue;
        }

        add_history(&history, input);

        ParsedInput parsed;
        if (!parse_input(input, &parsed)) {
            fprintf(stderr, "Parse error: command is invalid.\n");
            continue;
        }

        if (parsed.has_pipe) {
            execute_piped_command(&parsed.left, &parsed.right);
            continue;
        }

        if (execute_command(&parsed.left, &history) == -1) {
            break;
        }
    }

    free_history(&history);
}
