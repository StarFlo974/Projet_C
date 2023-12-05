#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include "lib/tree.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Charge la configuration depuis tree.conf
    struct config cfg;
    load_config(&cfg, "tree.conf");

    // Enregistre le nom de fichier dans le fichier spécifié dans la configuration
    FILE *datafile = fopen(cfg.datafile, "w");
    if (datafile != NULL)
    {
        fprintf(datafile, "%s\n", argv[1]);
        fclose(datafile);
    }
    else
    {
        perror("Error opening datafile");
        return 1;
    }

    // Envoie un signal SIGHUP à treeload
    pid_t treeload_pid;
    FILE *pidfile = fopen(cfg.pidfile, "r");
    if (pidfile != NULL)
    {
        int read_count = fscanf(pidfile, "%d", &treeload_pid);

        if (read_count == 1)
        {
            // La lecture du PID a réussi
            fclose(pidfile);

            if (kill(treeload_pid, SIGHUP) == -1)
            {
                perror("Error sending SIGHUP signal");
                return 1;
            }
        }
        else if (read_count == 0)
        {
            // Aucun entier trouvé dans le fichier
            fclose(pidfile);
            fprintf(stderr, "No valid integer found in pidfile\n");
            return 1;
        }
        else
        {
            // Erreur de lecture du PID
            fclose(pidfile);
            perror("Error reading treeload PID from pidfile");
            return 1;
        }
    }
    else
    {
        perror("Error opening pidfile");
        return 1;
    }

    return 0;
}
