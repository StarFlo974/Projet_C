#define MAXBUF 1024
#define DELIM "="

struct config
{
    char rootdir[MAXBUF];
    char datafile[MAXBUF];  // Nouvelle variable
    char pidfile[MAXBUF];   // Nouvelle variable
};

struct struct_dossier
{
    char nom[256];
    struct struct_dossier **sous_dossiers;
    int nb_sousdossier;
};

void load_config(struct config *cfg, char *filename);
void loadrecur(const char *path, struct struct_dossier *dossier);
void searchrecur(struct struct_dossier *dossier, const char *recherche, const char *current_path);
