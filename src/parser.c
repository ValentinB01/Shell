//sparge textul in tokeni si structuri
#include "../include/shell.h"

void parse_input(char *input, struct Command *cmd)
{
    input[strcspn(input, "\n")] = 0;

    cmd->argc = 0;

    char *token = strtok(input, " \t");
    while(token != NULL && cmd->argc < MAX_ARGS - 1)
    {
        cmd->argv[cmd->argc] = token;
        cmd->argc++;
        token = strtok(NULL, " \t");
    }
    cmd->argv[cmd->argc] = NULL;
}