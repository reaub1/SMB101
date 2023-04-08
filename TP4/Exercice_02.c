#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

volatile int points_pere = 0;
volatile int points_fils = 0;
pid_t pere_pid;
pid_t fils_pid;

void sig_handler(int sig) {
    if (sig == SIGUSR1) {
        printf("Père: Signal reçu\n");
        sleep(1); // Temporisation
        if (rand() % 2) {
            printf("Père: Renvoie la balle\n");
            kill(fils_pid, SIGUSR2);
        } else {
            printf("Père: Manque la balle\n");
            points_fils++;
        }
    } else if (sig == SIGUSR2) {
        printf("Fils: Signal reçu\n");
        sleep(1); // Temporisation
        if (rand() % 2) {
            printf("Fils: Renvoie la balle\n");
            kill(pere_pid, SIGUSR1);
        } else {
            printf("Fils: Manque la balle\n");
            points_pere++;
        }
    }

    if (points_pere == 13) {
        printf("Père: J'ai gagné !\n");
        kill(fils_pid, SIGTERM);
        exit(EXIT_SUCCESS);
    } else if (points_fils == 13) {
        printf("Fils: J'ai gagné !\n");
        kill(pere_pid, SIGTERM);
        exit(EXIT_SUCCESS);
    }
}

int main() {
    srand(time(NULL));
    pere_pid = getpid();

    fils_pid = fork();
    if (fils_pid < 0) {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    }

    if (fils_pid == 0) {
        // Processus fils
        fils_pid = getpid();
        signal(SIGUSR2, sig_handler);
        while (1) {
            pause();
        }
    } else {
        // Processus père
        signal(SIGUSR1, sig_handler);
        sleep(1);
        printf("Père: Envoie la balle\n");
        kill(fils_pid, SIGUSR2);

        while (1) {
            pause();
        }
    }

    return 0;
}
