#include "fonctions_fichiers.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

char** allouer_tab_2D(int n, int m) {
    char** tab = (char**)malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        tab[i] = (char*)malloc(m * sizeof(char));
        for (int j = 0; j < m; j++) {
            tab[i][j] = ' ';
        }
    }
    return tab;
}

void desallouer_tab_2D(char** tab, int n) {
    for (int i = 0; i < n; i++) {
        free(tab[i]);
    }
    free(tab);
}

void afficher_tab_2D(char** tab, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%c ", tab[i][j]);
        }
        printf("\n");
    }
}

void taille_fichier(const char* nomFichier, int* nbLig, int* nbCol) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur ");
        exit(EXIT_FAILURE);
    }
    int c;
    *nbLig = 0;
    *nbCol = 0;
    int col_temp = 0;
    while ((c = fgetc(fichier)) != EOF) {
        if (c == '\n') {
            (*nbLig)++;
            if (col_temp > *nbCol) {
                *nbCol = col_temp;
            }
            col_temp = 0;
        } else {
            col_temp++;
        }
    }
    fclose(fichier);
}

char** lire_fichier(const char* nomFichier) {
    int nbLig, nbCol;
    
    taille_fichier(nomFichier, &nbLig, &nbCol);
    
    FILE* fichier = fopen(nomFichier, "r");
    
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }
    char** tab = allouer_tab_2D(nbLig, nbCol);
    for (int i = 0; i < nbLig; i++) {
        fscanf(fichier, "%s", tab[i]);
    }
    fclose(fichier);
    return tab;
}
//Exercice 2 
char** modifier_caractere(char** tab, int n, int m, char ancien, char nouveau) {
    char** nouveauTab = allouer_tab_2D(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (tab[i][j] == ancien) {
                nouveauTab[i][j] = nouveau;
            } else {
                nouveauTab[i][j] = tab[i][j];
            }
        }
    }
    return nouveauTab;
}


void ecrire_fichier(const char* nomFichier, char** tab, int n, int m) {
	FILE* fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fprintf(fichier, "%c ", tab[i][j]);
        }
        fprintf(fichier, "\n");
    }
    fclose(fichier);
}




void afficher_graphe_sdl(char** tab, int n, int m) {
    // Initialiser SDL et créer une fenêtre
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Terrain", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 320, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Charger les sprites depuis le fichier pavage.bmp
    SDL_Surface* spriteSurface = SDL_LoadBMP("pavage.bmp");
    SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    SDL_FreeSurface(spriteSurface);


   
      // Chargement des sprites
    SDL_Surface* surfaceSprite = SDL_LoadBMP("photo.bmp");
    SDL_Texture* textureSprite = SDL_CreateTextureFromSurface(renderer, surfaceSprite);
    SDL_FreeSurface(surfaceSprite);


// Position du personnage
    int posX = 0;
    int posY = 0;

SDL_Event evenements;
    int terminer = 0;
    int TAILLE_SPRITE = 61;
    
     while (!terminer) {
        while (SDL_PollEvent(&evenements)) {
            switch (evenements.type) {
            case SDL_QUIT:
                terminer = 1;
                break;
            case SDL_KEYDOWN:
                switch (evenements.key.keysym.sym) {
                case SDLK_UP:
                if (posY > 0) {
                     posY -= 10;
                 }
                 
                   //SDL_WaitEvent(&evenements);
                  //  printf("haut \n ");
                    break;
                case SDLK_DOWN:
                    if (posY+TAILLE_SPRITE < 320 ) { 
                    posY += 10;
                }
                    break;
                case SDLK_LEFT:
                    if(posX > 0){
                    posX -= 10;

                }

                
                    break;
                case SDLK_RIGHT:
                     if( posX+TAILLE_SPRITE < 700){
                     posX += 10;
                }
                    break;
                }
                break;

            }

        }
         // Boucle pour afficher le terrain
        for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char caractere = tab[i][j];
            int indiceSprite = (caractere >= '0' && caractere <= '9') ? caractere - '0' : 0;

            SDL_Rect destRect = { j * 32, i * 32, 32, 32 };
            SDL_Rect srcRect = { indiceSprite * 32, 0, 32, 32 };

            SDL_RenderCopy(renderer, spriteTexture, &srcRect, &destRect);
           
        }
    }


        // Afficher le personnage
       SDL_Rect srcRect = { 0, 0, TAILLE_SPRITE, TAILLE_SPRITE };
       SDL_Rect destRect = { posX, posY, TAILLE_SPRITE, TAILLE_SPRITE };
       SDL_RenderCopy(renderer, textureSprite, &srcRect, &destRect);


        // Afficher la fenêtre et attendre la fermeture
        SDL_RenderPresent(renderer);
        SDL_Delay(50); // Attendre 10 secondes avant de fermer la fenêtre
       
       
    }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
}






