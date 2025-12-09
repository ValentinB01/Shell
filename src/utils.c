// functii ajutatoare (eliberare memorie, erori)
#include "../include/shell.h"

void free_commands(struct Command *root_cmd)
{
    struct Command *curr = root_cmd;
    struct Command *next;
    while(curr != NULL)
    {
        next = curr->next;
        if(curr->input_file)
            free(curr->input_file);
        if(curr->output_file)
            free(curr->output_file);

        free(curr);
        curr = next;
    }
}