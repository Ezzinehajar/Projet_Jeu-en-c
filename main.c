
#include "fonctions_fichiers.h"



int main() {
	char** tableauTerrain = lire_fichier("terrain.txt");

    // Afficher le terrain avec la SDL
    afficher_graphe_sdl(tableauTerrain, 10, 25);
    
    
    // Libérer la mémoire allouée pour le tableau
    desallouer_tab_2D(tableauTerrain, 10);

    return 0;



}


