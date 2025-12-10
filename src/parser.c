#include "../include/shell.h"
#include <ctype.h> 

struct Command* create_command() { // functia ce initializeaza structura de date
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
    char *p = line; // pointerul ce parcurge linia primita de la tastatura
    
    while (*p) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;

        char *token_start; 
        int in_quotes = 0; // flag pentru ghilimele

        if (*p == '"') { //sarim de prima ghilimea
            in_quotes = 1;
            p++; 
            token_start = p; // pointer de unde incepe cuvantul
            
            while (*p && *p != '"') {
                p++; // citim text-ul pana la urmatoarea ghilimea
            }
            
            if (*p == '"') {
                *p = '\0'; 
                p++; // marcam finalul textului din ghilimele
            }
        } else {
            token_start = p;// altfel fiecare cuvant din input e un argument
            while (*p && !isspace(*p)) {
                p++;
            }
            
            if (*p) {
                *p = '\0'; 
                p++;
            }
        }

        if (strcmp(token_start, "<") == 0) { // cream operatia de input("<") asupra unui fisier
            while (*p && isspace(*p)) p++;
            if (*p) {
                char *file_start = p;
                while (*p && !isspace(*p)) p++;
                if (*p) { *p = '\0'; p++; }
                cmd->input_file = strdup(file_start);
            }
        } 
        else if (strcmp(token_start, ">") == 0) { // cream operatia de overwrite(">") asupra unui fisier
            while (*p && isspace(*p)) p++;
            if (*p) {
                char *file_start = p;
                while (*p && !isspace(*p)) p++;
                if (*p) { *p = '\0'; p++; }
                cmd->output_file = strdup(file_start);
                cmd->append_mode = 0;
            }
        }
        else if (strcmp(token_start, ">>") == 0) { // cream operatia de append(">>") asupra unui fisier
            while (*p && isspace(*p)) p++;
            if (*p) {
                char *file_start = p;
                while (*p && !isspace(*p)) p++;
                if (*p) { *p = '\0'; p++; }
                cmd->output_file = strdup(file_start);
                cmd->append_mode = 1;
            }
        }
        else { // pentru argumente normale
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

    while ((pipe_segment = strtok_r(rest_pipeline, "|", &rest_pipeline))) { // spargem dupa | pentru a procesa separat comenzile separate prin pipeuri
        current = create_command();
        
        if (*root_cmd == NULL) { // cream o lista inlantuita din comenzile separate de |
            *root_cmd = current;
        } else {
            last->next = current;
        }
        last = current;

        parse_args_manual(pipe_segment, current);
    }
}

void free_commands(struct Command *root_cmd) { // elibereaza memoria alocata pentru lista de comenzi
    struct Command *curr = root_cmd;
    struct Command *next;
    
    while (curr != NULL) {
        next = curr->next;
        
        if (curr->input_file) {
            free(curr->input_file);
        }
        if (curr->output_file) {
            free(curr->output_file);
        }
        free(curr);
        curr = next;
    }
}