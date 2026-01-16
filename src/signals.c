#define _POSIX_C_SOURCE 200809L // definim POSIX astfel incat sa recunoasca compilatorul C sigaction, deoarece structura sigaction e specifica linux/unix
#include "../include/shell.h"
#include <string.h>

void handle_sigint(int sig) {
    char cwd[1024];
    write(STDOUT_FILENO, "\n", 1); 
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        write(STDOUT_FILENO, "MyShell:", 8);
        write(STDOUT_FILENO, cwd, strlen(cwd));
        write(STDOUT_FILENO, "$ ", 2);
    } else {
        write(STDOUT_FILENO, "MyShell$ ", 9);
    }
}

void handle_sigtstp(int sig) {
    write(STDOUT_FILENO, "\nStopped\n", 9);
}

void setup_signals() {
    struct sigaction sa_int;
    struct sigaction sa_tstp;

    //configurare CTRL+C
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask); // nu blocam alte semnale in timpul procesarii acestuia
    sa_int.sa_flags = SA_RESTART; // previne shell-ul  din a se inchide la CTRL+C
    
    if (sigaction(SIGINT, &sa_int, NULL) == -1) { // inregistram handler-ul in kernel
        perror("Eroare la setarea SIGINT");
        exit(1);
    }

    //configurare CTRL+Z
    sa_tstp.sa_handler = SIG_IGN; // doar procesele copil ale shell-ului pot primi CTRL+Z
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = SA_RESTART;

    if (sigaction(SIGTSTP, &sa_tstp, NULL) == -1) {
        perror("Eroare la setarea SIGTSTP");
        exit(1);
    }
}