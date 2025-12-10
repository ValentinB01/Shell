#include "../include/shell.h"

void handle_redirections(struct Command *cmd)
{
    if(cmd->input_file) // redirectarea operatiei input "<"
    {
        int fd = open(cmd->input_file, O_RDONLY);
        if(fd < 0)
        {
            perror("Eroare la input!");
            exit(1);
        }
        dup2(fd, STDIN_FILENO); // face ca atunci cand programul citeste de la tastatura sa citeasca defapt din fisier
        close(fd);
    }
    if (cmd->output_file) { // redirectarea operatiilor append ">>" si overwrite ">"
        int flags = O_WRONLY | O_CREAT;
        if (cmd->append_mode) 
            flags |= O_APPEND;
        else 
            flags |= O_TRUNC;
        int fd = open(cmd->output_file, flags, 0644); // 0644 -> doar userul poate citi si scrie
        if (fd < 0) 
        { 
            perror("Eroare la output!"); exit(1); 
        }
        dup2(fd, STDOUT_FILENO); // face ca orice printf sa mearga in fisier nu sa apara pe ecran
        close(fd);
    }
}


int execute_pipeline(struct Command *root_cmd)
{
    if(root_cmd == NULL)
    {
        return 0;
    }
    // verificarea comenzilor built-in
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
                    return 1; 
                }
            }
            return 0;
        }
        if(strcmp(root_cmd->argv[0], "exit") == 0)
        {
            printf("Iesim din shell ...\n");
            exit(0);
        }
        if (strcmp(root_cmd->argv[0], "history") == 0) {
            print_history();
            return 0;
        }
    } 
    // logica pipe si comenzi externe
    struct Command *cmd = root_cmd;
    int last_pid = 0;
    int input_fd = STDIN_FILENO; // input initial de la tastatura
    int pipe_fd[2]; //intializare pipe
    pid_t pid;

    while(cmd != NULL) // iteram prin fiecare comanda separata de |
    {
        if(cmd->next != NULL)
        {
            if(pipe(pipe_fd) < 0)
            {
                perror("Pipe-ul a esuat!");
                return 1; 
            }
        }

        pid = fork();
        if(pid < 0)
        {
            perror("Fork-ul a esuat!");
            return 1; 
        }
        else if (pid == 0) // executia comenzii(in procesul copil)
        {
            if(input_fd != STDIN_FILENO) // daca input nu e de la tastatura => vine dintr-un pipe anterior
            {
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if(cmd->next != NULL) // urmeaza comanda => scriem in pipe
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
                close(pipe_fd[0]);
            }
            
            handle_redirections(cmd); // redirectarile catre fisiere au prioritate fata de pipes
            
            execvp(cmd->argv[0], cmd->argv); // execvp cauta programul in variabila de mediu $PATH si inlocuieste procesul curent cu cel din $PATH
            perror("Eroare la executare!");
            exit(1);
        }
        else
        {
            last_pid = pid;

            if(input_fd != STDIN_FILENO)
            {
                close(input_fd);
            }
            if(cmd->next != NULL) // pregatim inputul pentru urmatoarea comanda
            {
                close(pipe_fd[1]);
                input_fd = pipe_fd[0];
            }
        }
        cmd = cmd->next;
    }

    int status;
    int final_exit_code = 0;
    pid_t wpid;
    
    while ((wpid = wait(&status)) > 0) { //asteptam terminarea tuturor proceselor copil
        if (wpid == last_pid) { // daca 
            if (WIFEXITED(status)) { // statusul ultimei comenzi, daca nu a dat exit normal => pipe nu a functionat
                final_exit_code = WEXITSTATUS(status);
            } else {
                final_exit_code = 1; 
            }
        }
    }
    
    return final_exit_code;
}