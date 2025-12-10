#include "../include/shell.h"

char* find_next_operator(char *str, int *type) { // functie helper pentru a gasi urmatorul operator
    char *and_op = strstr(str, "&&");
    char *or_op = strstr(str, "||");

    if (and_op != NULL && (or_op == NULL || and_op < or_op)) { // returnam urmatorul operator din string
        *type = 1; // && = 1
        return and_op;
    }
    if (or_op != NULL) {
        *type = 2; // || = 2
        return or_op;
    }
    *type = 0;
    return NULL;
}

int main() {
    char input[MAX_CMD_LEN];
    struct Command *root_cmd = NULL;

    init_history();
    setup_signals(); //acest apel configureaza cum raspunde Shell-ul(parintele) la intreruperi externe

    while (1) { // loop-ul esential ce  constituie shell-ul
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd))) printf("MyShell:%s$ ", cwd); // afiseaza path-ul curent in care shell-ul executa comenzi 
        else printf("MyShell$ ");
        fflush(stdout);

        if (fgets(input, MAX_CMD_LEN, stdin) == NULL) break; // citim din consola
        
        add_history(input); // adaugam linia in istoric

        char *cursor = input; // pointer ce trece prin sirul input
        int should_execute = 1; 
        int last_status = 0;   
        while (cursor != NULL && *cursor != '\0') {
            int op_type = 0;
            char *op_pos = find_next_operator(cursor, &op_type); // cautam && sau ||

            if (op_pos != NULL) { // daca gasim && sa || luam string-ul pana la ele
                *op_pos = '\0';
            }

            if (should_execute) {   
                parse_input(cursor, &root_cmd);
                
                if (root_cmd != NULL) {
                    last_status = execute_pipeline(root_cmd); // parsam string-ul curent
                    free_commands(root_cmd);
                    root_cmd = NULL;
                }
            }

            if (op_pos == NULL) break; // nu mai exista operatori => iesim

            if (op_type == 1) { // executam restul liniei dupa && daca comanda actuala a avut succes
                should_execute = (last_status == 0);
            } else if (op_type == 2) { // executam restul liniei dupa || daca comanda actuala a esuat
                should_execute = (last_status != 0);
            }

            cursor = op_pos + 2; // sarim operatorul
        }
    }

    return 0;
}