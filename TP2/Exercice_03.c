#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    // Création d'un pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Erreur lors de la création du pipe");
        exit(EXIT_FAILURE);
    }

    // Création du processus pour exécuter 'ps eaux'
    pid_t pid_ps = fork();
    if (pid_ps < 0) {
        perror("Erreur de fork");
        exit(EXIT_FAILURE);
    }

    if (pid_ps == 0) {
        // Fermeture de l'entrée du pipe
        close(pipefd[0]);

        // Redirection de la sortie standard vers l'entrée du pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("Erreur lors de la redirection de STDOUT vers le pipe");
            exit(errno);
        }

        // Exécution de 'ps eaux'
        execlp("ps", "ps", "eaux", NULL);

        // Si exec échoue
        perror("Erreur lors de l'exécution de 'ps eaux'");
        exit(errno);
    }

    // Création du processus pour exécuter 'grep "^root "'
    pid_t pid_grep = fork();
    if (pid_grep < 0) {
        perror("Erreur de fork");
        exit(EXIT_FAILURE);
    }

    if (pid_grep == 0) {
        // Fermeture de la sortie du pipe
        close(pipefd[1]);

        // Redirection de l'entrée standard vers la sortie du pipe
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("Erreur lors de la redirection de STDIN vers le pipe");
            exit(errno);
        }

        // Exécution de 'grep "^root "'
        execlp("grep", "grep", "^root ", NULL);

        // Si exec échoue
        perror("Erreur lors de l'exécution de 'grep \"^root \"'");
        exit(errno);
    }

    // Fermeture des deux côtés du pipe dans le processus père
    close(pipefd[0]);
    close(pipefd[1]);

    // Attend la fin du processus 'grep'
    int status;
    waitpid(pid_grep, &status, 0);

    // Si 'grep' a trouvé une correspondance
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        // Affichage final avec la primitive write
        if (write(STDOUT_FILENO, "root est connecté\n", strlen("root est connecté\n")) == -1) {
            perror("Erreur lors de l'affichage final");
            exit(errno);
        }
    }

    return 0;
}
