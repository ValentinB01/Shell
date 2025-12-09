// logica de fork, exec, pipe, redirect
#include "../include/shell.h"

void handle_redirections(struct Command *cmd)
{
    if(cmd->input_file)
    {
        int fd = open(cmd->input_file, O_RDONLY);
        if(fd < 0)
        {
            perror("Eroare la input!");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (cmd->output_file) {
        int flags = O_WRONLY | O_CREAT;
        if (cmd->append_mode) 
            flags |= O_APPEND;
        else 
            flags |= O_TRUNC;
        int fd = open(cmd->output_file, flags, 0644);
        if (fd < 0) 
        { 
            perror("Eroare la output!"); exit(1); 
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void execute_pipeline(struct Command *root_cmd)
{
    if(root_cmd == NULL)
    {
        return;
    }
    if(root_cmd->next == NULL && root_cmd->argv[0] != NULL)
    {
        if (strcmp(root_cmd->argv[0], "cd") == 0)
        {
            if(root_cmd->argc < 2)
            {
                const char *home = getenv("HOME");
                if(home) chdir(home);
            }
            else
            {
                if(chdir(root_cmd->argv[1]) != 0)
                {
                perror("myshell: cd");
                }
            }
        return;
        }
        if(strcmp(root_cmd->argv[0], "exit") == 0)
        {
            printf("Iesim din shell ...\n");
            exit(0);
        }
    } 

    struct Command *cmd = root_cmd;
    int input_fd = STDIN_FILENO;
    int pipe_fd[2];
    pid_t pid;
    while(cmd != NULL)
    {
        if(cmd->next != NULL)
        {
            if(pipe(pipe_fd) < 0)
            {
                perror("Pipe-ul a esuat!");
                return;
            }
        }
        pid = fork();
        if(pid < 0)
        {
            perror("Fork-ul a esuat!");
            return;
        }
        else if (pid == 0)
        {
            if(input_fd != STDIN_FILENO)
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if(cmd->next != NULL)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
                close(pipe_fd[0]);
            }
            handle_redirections(cmd);
            execvp(cmd->argv[0], cmd->argv);
            perror("Eroare la executare!");
            exit(1);
        }
        else
        {
            if(input_fd != STDIN_FILENO)
            {
                close(input_fd);
            }
            if(cmd-> next != NULL)
            {
                close(pipe_fd[1]);
                input_fd = pipe_fd[0];
            }
        }
        cmd = cmd->next;
    }
    while(wait(NULL) > 0);
}