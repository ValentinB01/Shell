#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64
#define HISTORY_SIZE 100

struct Command
{
    char *argv[MAX_ARGS];
    int argc;

    char *input_file;
    char *output_file;
    int append_mode;

    int background;

    struct Command *next;
};

void parse_input(char *input, struct Command **root_cmd);
int execute_pipeline(struct Command *root_cmd);
void free_commands(struct Command *root_cmd);

void init_history();
void add_history_entry(const char *cmd);
void print_history();

void setup_signals();

int  alias_set(const char *name, const char *value);
int  alias_unset(const char *name);
void alias_print(const char *name);
void alias_expand(const char *line, char *out_buf, size_t out_size);

#endif