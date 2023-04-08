#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Vérification du bon nombre de paramètres
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <programme_a_executer>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Affichage du message sur la sortie standard
    printf("Programme à exécuter : %s\n", argv[1]);

    // Création du processus fils
    pid_t pid = fork();

    // Gestion des erreurs de fork
    if (pid < 0) {
        perror("Erreur de fork");
        exit(EXIT_FAILURE);
    }

    // Code exécuté par le processus fils
    if (pid == 0) {
        printf("PID du fils : %d\n", getpid());

        // Fermeture du descripteur STDOUT
        close(1);

        // Création et ouverture du fichier temporaire
        int fd = open("/tmp/proc-exercise", O_CREAT | O_WRONLY, 0666);
        if (fd < 0) {
            perror("Erreur lors de l'ouverture du fichier");
            exit(EXIT_FAILURE);
        }

        // Duplication du descripteur de fichier pour le rediriger vers STDOUT
        dup2(fd, 1);

        // Affichage du numéro du descripteur du fichier ouvert
        printf("Numéro du descripteur du fichier ouvert : %d\n", fd);

        // Exécution du programme spécifié en argument
        execlp(argv[1], argv[1], NULL);

        // Si exec échoue
        perror("Erreur lors de l'exécution du programme");
        exit(EXIT_FAILURE);
    }
    // Code exécuté par le processus père
    else {
        printf("PID du père : %d\n", getpid());

        // Attend la fin du fils
        wait(NULL);

        // Affiche un message avant de terminer
        printf("That's All Folks!\n");
    }

    return 0;
}
