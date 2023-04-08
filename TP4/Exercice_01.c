#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile int count_sigint = 0;

void sigint_handler(int sig) {
    count_sigint++;
    printf("Nombre de SIGINT reçus : %d\n", count_sigint);
}

void sigterm_handler(int sig) {
    printf("Signal SIGTERM reçu. Fin du programme.\n");
    exit(0);
}

int main() {
    struct sigaction sigint_action;
    struct sigaction sigterm_action;

    // Configurer le gestionnaire pour SIGINT
    sigint_action.sa_handler = sigint_handler;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;

    // Configurer le gestionnaire pour SIGTERM
    sigterm_action.sa_handler = sigterm_handler;
    sigemptyset(&sigterm_action.sa_mask);
    sigterm_action.sa_flags = 0;

    // Associer les gestionnaires aux signaux
    if (sigaction(SIGINT, &sigint_action, NULL) == -1) {
        perror("Erreur lors de la configuration du gestionnaire pour SIGINT");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGTERM, &sigterm_action, NULL) == -1) {
        perror("Erreur lors de la configuration du gestionnaire pour SIGTERM");
        exit(EXIT_FAILURE);
    }

    // Boucle sans fin pour attendre les signaux
    while (1) {
        pause();
    }

    return 0;
}
