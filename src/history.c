#include "../include/shell.h"
#include <readline/history.h>

char *shell_history[HISTORY_SIZE]; /* istoria interna (pentru comanda "history") */
int history_count = 0;

void init_history() {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        shell_history[i] = NULL;
    }
    /* initializam si istoricul readline */
    using_history();
}

void add_history_entry(const char *cmd) {
    if (cmd == NULL || *cmd == '\0' || *cmd == '\n') return;

    /* copie curata fara '\n' de la final */
    char *clean = strdup(cmd);
    if (!clean) return;

    size_t len = strlen(clean);
    if (len > 0 && clean[len - 1] == '\n')
        clean[len - 1] = '\0';

    if (*clean == '\0') { free(clean); return; }

    /* lista interna */
    if (history_count < HISTORY_SIZE) {
        shell_history[history_count++] = clean;
    } else {
        free(shell_history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++)
            shell_history[i - 1] = shell_history[i];
        shell_history[HISTORY_SIZE - 1] = clean;
    }

    /* lista readline — permite navigare cu sageti Sus/Jos */
    add_history(clean);
}

void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d %s\n", i + 1, shell_history[i]);
    }
}