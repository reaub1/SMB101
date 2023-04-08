#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    // Création du processus fils avec la primitive système fork
    pid_t pid = fork();

    // Gestion des erreurs de fork
    if (pid < 0) {
        perror("Erreur de fork");
        exit(EXIT_FAILURE);
    }

    // Code exécuté par le processus fils
    if (pid == 0) {
        // Affichage du PID du processus fils et de son parent
        printf("Je suis le processus fils. Mon PID est %d et celui de mon père est %d.\n", getpid(), getppid());

        // Sortie exit avec un code de retour égal au dernier chiffre du PID
        exit(getpid() % 10);
    }
    // Code exécuté par le processus père
    else {
        int status;
        printf("Je suis le processus père. Mon PID est %d et celui de mon fils est %d.\n", getpid(), pid);

        // Attend la terminaison du processus fils
        waitpid(pid, &status, 0);

        // Affiche le code de retour du processus fils
        if (WIFEXITED(status)) {
            printf("Le processus fils a terminé avec le code de retour %d.\n", WEXITSTATUS(status));
        }
    }

    return 0;
}
