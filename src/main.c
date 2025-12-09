//Bucla infinita ce constituie shell-ul
#include "../include/shell.h"

int main()
{
    char input[MAX_CMD_LEN];
    struct Command *root_cmd = NULL;

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

        parse_input(input, &root_cmd);

        if(root_cmd != NULL)
        {
            execute_pipeline(root_cmd);
            root_cmd = NULL;
        }
    }
    return 0;
}