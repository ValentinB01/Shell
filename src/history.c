#include "../include/shell.h"

char *history[HISTORY_SIZE]; //vector ce stocheaza adresa de memorie unde se afla textul(istoricul)
int history_count = 0;

void init_history() { // initializam istoricul pentru a evita erori nedorite
    for (int i = 0; i < HISTORY_SIZE; i++) {
        history[i] = NULL;
    }
}

void add_history(const char *cmd) {
    if (cmd == NULL || strlen(cmd) == 0 || cmd[0] == '\n') return;
    
    if (history_count < HISTORY_SIZE) { // daca istoricul nu e plin adaugam comanda in istoric
        history[history_count++] = strdup(cmd); // strdup: calculeaza lungimea lui cmd si ii aloca spatiu(malloc) si il si copiaza in noua memorie
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) { // daca istoricul e plin, stergem cea mai veche comanda si o adaugam pe cea noua la final
            history[i-1] = history[i]; //aici shiftam la stanga elementele dupa stergerea lui history[0]
        }
        history[HISTORY_SIZE-1] = strdup(cmd);
    }
}

void print_history() { // afisam istoricul
    for (int i = 0; i < history_count; i++) {
        printf("%d %s", i + 1, history[i]);
        if (history[i][strlen(history[i])-1] != '\n') printf("\n");
    }
}