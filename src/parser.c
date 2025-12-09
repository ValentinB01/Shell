#include "../include/shell.h"
#include <ctype.h> 

struct Command* create_command() {
    struct Command *cmd = malloc(sizeof(struct Command));
    if (!cmd) { perror("malloc"); exit(1); }
    cmd->argc = 0;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_mode = 0;
    cmd->next = NULL;
    return cmd;
}

void parse_args_manual(char *line, struct Command *cmd) {
    char *p = line;
    
    while (*p) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;

        char *token_start;
        int in_quotes = 0;

        if (*p == '"') {
            in_quotes = 1;
            p++; 
            token_start = p; 
            
            while (*p && *p != '"') {
                p++;
            }
            
            if (*p == '"') {
                *p = '\0'; 
                p++;
            }
        } else {
            token_start = p;
            while (*p && !isspace(*p)) {
                p++;
            }
            
            if (*p) {
                *p = '\0'; 
                p++;
            }
        }

        if (strcmp(token_start, "<") == 0) {
            while (*p && isspace(*p)) p++;
            if (*p) {
                char *file_start = p;
                while (*p && !isspace(*p)) p++;
                if (*p) { *p = '\0'; p++; }
                cmd->input_file = strdup(file_start);
            }
        } 
        else if (strcmp(token_start, ">") == 0) {
            while (*p && isspace(*p)) p++;
            if (*p) {
                char *file_start = p;
                while (*p && !isspace(*p)) p++;
                if (*p) { *p = '\0'; p++; }
                cmd->output_file = strdup(file_start);
                cmd->append_mode = 0;
            }
        }
        else if (strcmp(token_start, ">>") == 0) {
            while (*p && isspace(*p)) p++;
            if (*p) {
                char *file_start = p;
                while (*p && !isspace(*p)) p++;
                if (*p) { *p = '\0'; p++; }
                cmd->output_file = strdup(file_start);
                cmd->append_mode = 1;
            }
        }
        else {
            if (cmd->argc < MAX_ARGS - 1) {
                cmd->argv[cmd->argc++] = token_start;
            }
        }
    }
    cmd->argv[cmd->argc] = NULL;
}

void parse_input(char *input, struct Command **root_cmd) {
    input[strcspn(input, "\n")] = 0;
    
    char *pipe_segment;
    char *rest_pipeline = input;
    
    struct Command *current = NULL;
    struct Command *last = NULL;

    while ((pipe_segment = strtok_r(rest_pipeline, "|", &rest_pipeline))) {
        current = create_command();
        
        if (*root_cmd == NULL) {
            *root_cmd = current;
        } else {
            last->next = current;
        }
        last = current;

        parse_args_manual(pipe_segment, current);
    }
}
