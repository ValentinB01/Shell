// logica de fork, exec, pipe, redirect
#include "../include/shell.h"

void execute_command(struct Command *cmd)
{
    if(cmd->argc == 0)
        return;
    
    if(strcmp(cmd->argv[0], "exit") == 0)
    {
        printf("Iesim din shell...\n");
        exit(0);
    }
    if(strcmp(cmd->argv[0], "cd") == 0)
    {
        if(cmd->argc < 2)
        {
            chdir(getenv("HOME"));
        }
        else
        {
            if(chdir(cmd->argv[1]) != 0)
            {
                perror("Comanda cd nu a functionat!\n");
            }
        }
        return;
    }
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("Eroare la fork!\n");
    }
    else if (pid == 0)
    {
        if(execvp(cmd->argv[0], cmd->argv) < 0)
        {
            perror("Eroare la executie!");
            exit(1);
        }
    }
    else
    {
        wait(NULL);
    }
}