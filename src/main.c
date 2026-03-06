#include "../include/shell.h"

static char **shell_completion(const char *text, int start, int end) {
    (void)end;
    if (start == 0) {
        /* primul cuvant => completam comenzi din PATH */
        return rl_completion_matches(text, rl_filename_completion_function);
    }
    /* argumentele ulterioare => completam fisiere/directoare */
    return NULL;
}

char* find_next_operator(char *str, int *type) {
    char *and_op = strstr(str, "&&");
    char *or_op  = strstr(str, "||");

    if (and_op != NULL && (or_op == NULL || and_op < or_op)) {
        *type = 1; /* && */
        return and_op;
    }
    if (or_op != NULL) {
        *type = 2; /* || */
        return or_op;
    }
    *type = 0;
    return NULL;
}

int main() {
    struct Command *root_cmd = NULL;

    init_history();
    setup_signals();

    /* Configuram readline */
    rl_attempted_completion_function = shell_completion;
   rl_completion_append_character = '\0';

    while (1) {
        /* Construim promptul dinamic cu calea curenta */
        char cwd[1024];
        char prompt[1100];
        if (getcwd(cwd, sizeof(cwd)))
            snprintf(prompt, sizeof(prompt), "MyShell:%s$ ", cwd);
        else
            snprintf(prompt, sizeof(prompt), "MyShell$ ");

        char *line = readline(prompt);
        if (line == NULL) { /* CTRL+D */
            printf("\n");
            break;
        }

        /* Ignoram liniile goale */
        if (*line == '\0') {
            free(line);
            continue;
        }

        /* Adaugam in istoricul intern si readline */
        add_history_entry(line);

        /* Lucram pe o copie pentru ca find_next_operator scrie '\0' in sir */
        char input[MAX_CMD_LEN];
        strncpy(input, line, MAX_CMD_LEN - 1);
        input[MAX_CMD_LEN - 1] = '\0';
        free(line); /* readline aloca dinamic => eliberam */

        char *cursor = input;
        int should_execute = 1;
        int last_status   = 0;

        while (cursor != NULL && *cursor != '\0') {
            int   op_type = 0;
            char *op_pos  = find_next_operator(cursor, &op_type);

            if (op_pos != NULL)
                *op_pos = '\0';

            if (should_execute) {
                char expanded[MAX_CMD_LEN];
                alias_expand(cursor, expanded, sizeof(expanded));

                parse_input(expanded, &root_cmd);
                if (root_cmd != NULL) {
                    last_status = execute_pipeline(root_cmd);
                    free_commands(root_cmd);
                    root_cmd = NULL;
                }
            }

            if (op_pos == NULL) break;

            if (op_type == 1)       /* && */
                should_execute = (last_status == 0);
            else if (op_type == 2)  /* || */
                should_execute = (last_status != 0);

            cursor = op_pos + 2;
        }
    }

    return 0;
}