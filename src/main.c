//Bucla infinita ce constituie shell-ul
#include "../include/shell.h"

int main()
{
    char input[MAX_CMD_LEN];
    struct Command cmd;

    while(1)
    {
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("MyShell:%s$ ", cwd);
        }
        else
        {
            printf("MyShell$ ");
        }

        if(fgets(input, MAX_CMD_LEN, stdin) == NULL)
        {
            break;
        }

        parse_input(input, &cmd);

        execute_command(&cmd);
    }
    return 0;
}