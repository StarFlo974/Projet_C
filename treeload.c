#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "lib/tree.h"

volatile sig_atomic_t flag = 0;
char search_filename[256]; // Assurez-vous que la taille est suffisante

void sighup_handler(int signo __attribute__((unused))) {
    flag = 1;
    printf("SIGHUP received in treeload\n");
}

void setup(struct config *cfg) {
    // Charge la configuration depuis tree.conf
    load_config(cfg, "tree.conf");

    // Attache le gestionnaire de signal SIGHUP
    if (signal(SIGHUP, sighup_handler) == SIG_ERR) {
        perror("Error setting up signal handler");
        exit(EXIT_FAILURE);
    }
}

void search_file(const char *filename, const struct struct_dossier *dossier, const char *current_path) {
    if (dossier == NULL) {
        return;
    }

    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", current_path, dossier->nom);

    // Vérifier si le fichier recherché est présent dans ce dossier
    if (strcmp(dossier->nom, filename) == 0) {
        printf("File found: %s\n", full_path);
    }

    // Recherche récursive dans tous les sous-dossiers
    for (int i = 0; i < dossier->nb_sousdossier; i++) {
        search_file(filename, dossier->sous_dossiers[i], full_path);
    }
}

void perform_search(const char *filename, const char *rootdir) {
    // Charger la structure du dossier
    struct struct_dossier root;
    loadrecur(rootdir, &root);

    // Effectuer la recherche
    search_file(filename, &root, "");
}

void treeload(struct config *cfg) {
    while (1) {
        if (flag) {
            // Lire le fichier spécifié dans tree.conf (treeload.data)
            FILE *datafile = fopen(cfg->datafile, "r");
            if (datafile != NULL) {
                if (fscanf(datafile, "%255s", search_filename) == 1) {
                    // Effectuez la recherche dans l'arborescence en utilisant le nom de fichier obtenu
                    perform_search(search_filename, cfg->rootdir);
                }
                fclose(datafile);
            } else {
                perror("Error opening datafile");
            }

            flag = 0;
        }
        pause();
    }
}

int main() {
    struct config cfg;
    setup(&cfg);

    // Écrire le PID dans le fichier PID
    FILE *pidfile = fopen(cfg.pidfile, "w");
    if (pidfile != NULL) {
        fprintf(pidfile, "%d", getpid());
        fclose(pidfile);
    } else {
        perror("Error opening pidfile for writing");
        exit(EXIT_FAILURE);
    }

    treeload(&cfg);
    return 0;
}


