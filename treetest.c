#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "lib/tree.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <directory>\n", argv[0]);
		return 1;
	}

	struct config cfg;
	struct struct_dossier *root = malloc(sizeof(struct struct_dossier));
	strcpy(root->nom, argv[1]);

	load_config(&cfg, "tree.conf");
	loadrecur(cfg.rootdir, root);
	searchrecur(root, argv[1], cfg.rootdir);

	return 0;
}