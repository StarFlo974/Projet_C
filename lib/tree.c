#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "tree.h"

void load_config(struct config *cfg, char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file != NULL)
    {
        char line[MAXBUF];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (line[0] == '#')
            {
                continue;
            }

            char *cfline;
            char *saveptr = NULL;  // Assurez-vous que saveptr est initialisé à NULL
            cfline = strstr((char *)line, DELIM);
            if (cfline != NULL) {
                cfline = strtok_r(cfline + strlen(DELIM), "\"\n", &saveptr);
                
                // Assurez-vous que cfline est valide avant de l'utiliser
                if (cfline != NULL) {
                    if (strstr((char *)line, "rootdir") != NULL)
                    {
                        cfline[strcspn(cfline, "\n")] = '\0';
                        memcpy(cfg->rootdir, cfline, strlen(cfline));
                        cfg->rootdir[strlen(cfline)] = '\0';
                    }
                    else if (strstr((char *)line, "datafile") != NULL)
                    {
                        cfline[strcspn(cfline, "\n")] = '\0';
                        memcpy(cfg->datafile, cfline, strlen(cfline));
                        cfg->datafile[strlen(cfline)] = '\0';
                    }
                    else if (strstr((char *)line, "pidfile") != NULL)
                    {
                        cfline[strcspn(cfline, "\n")] = '\0';
                        memcpy(cfg->pidfile, cfline, strlen(cfline));
                        cfg->pidfile[strlen(cfline)] = '\0';
                    }
                }
            }
        }
        fclose(file);
    }
}


void loadrecur(const char *path, struct struct_dossier *dossier)
{
	DIR *dir;
	struct dirent *dossier_ouvert;

	// On alloue une taille pour la quantité de sous-dossiers à traiter
	dossier->sous_dossiers = malloc(sizeof(struct struct_dossier *) * 256);

	// Cela permet d'allouer une taille à un sous-dossier dans notre dossier parent
	for (int i = 0; i < 256; i++)
	{
		dossier->sous_dossiers[i] = malloc(sizeof(struct struct_dossier));
	}
	dossier->nb_sousdossier = 0;

	if ((dir = opendir(path)) != NULL)
	{
		while ((dossier_ouvert = readdir(dir)) != NULL)
		{
			// On vérifie qu(il s'agisse bien d'un dossier
			if (dossier_ouvert->d_type == DT_DIR)
			{
				
				// Saut des dossiers actuel et parent
				if (strcmp(dossier_ouvert->d_name, ".") != 0 && strcmp(dossier_ouvert->d_name, "..") != 0)
				{
					struct struct_dossier *sous_dossier = malloc(sizeof(struct struct_dossier));

					// On copie le nom du sous-dossier, et son chemin, à partir de ce que readdir nous a fournit (le dossier actuellement ouvert)
					strcpy(sous_dossier->nom, dossier_ouvert->d_name);
					char nouveau_chemin[1024];
					sprintf(nouveau_chemin, "%s/%s", path, dossier_ouvert->d_name);
					// Ici on s'occupe des sous-dossiers du sous-dossiers en rappelant notre fonction
					loadrecur(nouveau_chemin, sous_dossier);
					dossier->sous_dossiers[dossier->nb_sousdossier] = sous_dossier;
					dossier->nb_sousdossier++;
				}
			}
		}
		closedir(dir);
	}
}

void searchrecur(struct struct_dossier *dossier, const char *recherche, const char *chemin_actuel)
{
	if (dossier == NULL)
	{
		return;
	}

	char chemin_maj[1024];
	char nom_avec_slash[1024];

	// Préfixer le nom du dossier avec un slash
	snprintf(nom_avec_slash, sizeof(nom_avec_slash), "/%s", dossier->nom);

	// On verifie si l'on se trouve dans le root ou non puis on concatène le chemin
	if (strcmp(chemin_actuel, "/") == 0)
	{
		snprintf(chemin_maj, sizeof(chemin_maj), "%s%s", chemin_actuel, dossier->nom);
	}
	else
	{
		if (strcmp(chemin_actuel, nom_avec_slash) != 0)
		{
			snprintf(chemin_maj, sizeof(chemin_maj), "%s/%s", chemin_actuel, dossier->nom);
		}
	}

	// On compare l'entrée actuelle à notre recherche
	if (strcmp(dossier->nom, recherche) == 0)
	{
		printf("Trouvé: %s\n", chemin_maj);
	}

	// Recherche récursive dans tous les sous-dossiers
	for (int i = 0; i < dossier->nb_sousdossier; i++)
	{
		searchrecur(dossier->sous_dossiers[i], recherche, chemin_maj);
	}
}


