// marimi de buffer, coduri erroare
#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

struct Command
{
    char *argv[MAX_ARGS];
    int argc;
};

void parse_input(char *input, struct Command *cmd);
void execute_command(struct Command *cmd);

#endif