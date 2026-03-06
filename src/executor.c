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
            /* Salvam directorul curent in OLDPWD inainte de orice schimbare */
            char oldpwd[1024];
            if (getcwd(oldpwd, sizeof(oldpwd)) != NULL) {
                setenv("OLDPWD", oldpwd, 1);
            }

            const char *target = NULL;

            if (root_cmd->argc < 2) {
                /* cd fara argumente => mergi la $HOME */
                target = getenv("HOME");
            } else if (strcmp(root_cmd->argv[1], "-") == 0) {
                /* cd - => revino la directorul anterior ($OLDPWD) */
                target = getenv("OLDPWD");
                if (target == NULL) {
                    fprintf(stderr, "myshell: cd: OLDPWD nu este setat\n");
                    return 1;
                }
                printf("%s\n", target); /* bash afiseaza noua cale la cd - */
            } else {
                target = root_cmd->argv[1];
            }

            if (target && chdir(target) != 0) {
                perror("myshell: cd");
                return 1;
            }

            /* Actualizam PWD in mediu dupa schimbare */
            char newpwd[1024];
            if (getcwd(newpwd, sizeof(newpwd)) != NULL) {
                setenv("PWD", newpwd, 1);
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
        if (strcmp(root_cmd->argv[0], "export") == 0) {
            if (root_cmd->argc < 2) {
                fprintf(stderr, "export: lipsa argument (ex: export NUME=Valoare)\n");
                return 1;
            }
            for (int i = 1; i < root_cmd->argc; i++) {
                char *pair = root_cmd->argv[i]; /* ex: "NUME=Valentin" */
                char *eq   = strchr(pair, '=');
                if (eq == NULL) {
                    fprintf(stderr, "export: format invalid '%s' (foloseste NUME=VALOARE)\n", pair);
                    return 1;
                }
                *eq = '\0';              /* splitem in NUME si VALOARE */
                char *name  = pair;
                char *value = eq + 1;
                if (setenv(name, value, 1) != 0) {
                    perror("export: setenv");
                    *eq = '=';           /* restauram sirul original */
                    return 1;
                }
                *eq = '=';               /* restauram - nu stricam argv[] original */
            }
            return 0;
        }
        if (strcmp(root_cmd->argv[0], "alias") == 0) {
            if (root_cmd->argc < 2) {
                alias_print(NULL); /* listeaza toate */
                return 0;
            }
            for (int i = 1; i < root_cmd->argc; i++) {
                char *pair = root_cmd->argv[i];
                char *eq   = strchr(pair, '=');
                if (eq == NULL) {
                    /* alias NUME => afiseaza doar acel alias */
                    alias_print(pair);
                } else {
                    *eq = '\0';
                    alias_set(pair, eq + 1);
                    *eq = '=';
                }
            }
            return 0;
        }
        /* unalias NUME — sterge un alias */
        if (strcmp(root_cmd->argv[0], "unalias") == 0) {
            if (root_cmd->argc < 2) {
                fprintf(stderr, "unalias: lipsa nume\n");
                return 1;
            }
            return alias_unset(root_cmd->argv[1]);
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

   if (root_cmd->background) {
        printf("[background] PID %d\n", last_pid);
        return 0;
    }

    int status;
    int final_exit_code = 0;
    pid_t wpid;

    while ((wpid = wait(&status)) > 0) { /* asteptam terminarea tuturor proceselor copil */
        if (wpid == last_pid) {
            if (WIFEXITED(status)) { /* statusul ultimei comenzi */
                final_exit_code = WEXITSTATUS(status);
            } else {
                final_exit_code = 1;
            }
        }
    }

    return final_exit_code;
}