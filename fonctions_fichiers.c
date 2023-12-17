#include "fonctions_fichiers.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
SDL_Texture* charger_image_transparente(const char* nomfichier,SDL_Renderer* renderer,Uint8 r, Uint8 g, Uint8 b){
    SDL_Texture *texture = NULL ;
    SDL_Surface *surface = NULL;
    surface = SDL_LoadBMP(nomfichier);
    // Récupérer la valeur (RGB) du pixel au format donné.
    //SDL_MapRGB utilisée pour convertir une couleur au format RGB (Rouge, Vert, Bleu) en une valeur entière (Uint32) qui est compatible avec le format de pixel de la surface SDL. Cela permet de représenter une couleur dans un format qui peut être utilisé efficacement pour le rendu graphique.
    const SDL_PixelFormat* format = surface->format ;
    Uint32 couleur = SDL_MapRGB(format,r,g,b);
    // Définir la couleur (pixel transparent) dans une surface.
    //SDL_SetColorKey est utilisée pour spécifier une couleur de transparence (color key) dans une surface SDL. Lorsque vous définissez une couleur de transparence, toutes les zones de la surface qui correspondent à cette couleur spécifiée seront rendues transparentes lors du rendu
    SDL_SetColorKey(surface,SDL_TRUE,couleur);
    // Convertir la surface de l’image au format texture avant de l’appliquer
    texture = SDL_CreateTextureFromSurface(renderer,surface) ;
     //liberer la surface
    SDL_FreeSurface(surface);
    return texture;
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
        perror("Erreur lors de l'ouverture du fichier");
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

SDL_Texture* charger_texte(const char* texte, SDL_Renderer* renderer, TTF_Font* police, SDL_Color couleurTexte) {
    // Charger le texte dans une surface
    SDL_Surface* surfaceTexte = TTF_RenderText_Solid(police, texte, couleurTexte);
    if (surfaceTexte == NULL) {
        fprintf(stderr, "Erreur lors du rendu du texte : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    // Convertir la surface en texture
    SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    if (textureTexte == NULL) {
        fprintf(stderr, "Erreur lors de la création de la texture : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Libérer la surface, elle n'est plus nécessaire
    SDL_FreeSurface(surfaceTexte);

    return textureTexte;
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
    SDL_Texture* textureSprite = charger_image_transparente("la.bmp", renderer, 0, 0, 0);

    // Position du personnage
    int posX = 0;
    int posY = 0;

    Bouton boutonJouer;
    Bouton boutonQuitter;

// Initialisation des boutons
int boutonLargeur = 150;
int boutonHauteur = 50;

int centreX = 700 / 2; // Centre horizontal de la fenêtre
int centreY = 320 / 2; // Centre vertical de la fenêtre

boutonJouer.rect = (SDL_Rect){centreX - boutonLargeur / 2, centreY - boutonHauteur, boutonLargeur, boutonHauteur};
boutonJouer.action = JOUER;

boutonQuitter.rect = (SDL_Rect){centreX - boutonLargeur / 2, centreY, boutonLargeur, boutonHauteur};
boutonQuitter.action = QUITTER;


    int dansMenu = 1;
    SDL_Event evenements;
    int terminer = 0;
    int TAILLE_SPRITE = 30;

    TTF_Init();  // Initialiser SDL_ttf

    // Charger la police (assurez-vous que le fichier de police existe)
    TTF_Font* police = TTF_OpenFont("arial.ttf", 24);
    if (police == NULL) {
        fprintf(stderr, "Erreur lors du chargement de la police : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Color couleurTexte = {255, 255, 255, 255};

    while (!terminer) {
        while (SDL_PollEvent(&evenements)) {
            switch (evenements.type) {
                case SDL_QUIT:
                    terminer = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (evenements.key.keysym.sym) {
                        case SDLK_UP:
                            if (posY > 0 && tab[(posY - 1) / 30][posX / 30] != '6' && posY - 30 >= 0) {
                                posY -= 30;
                            }
                            break;
                        case SDLK_DOWN:
                            if (posY + TAILLE_SPRITE < 320 && tab[(posY + TAILLE_SPRITE) / 30][posX / 30] != '6') {
                                posY += 30;
                            }
                            break;
                        case SDLK_LEFT:
                            if (posX > 0 && tab[posY / 30][(posX - 30) / 30] != '6') {
                                posX -= 30;
                            }
                            break;
                        case SDLK_RIGHT:
                            if (posX + TAILLE_SPRITE < 700 && tab[posY / 30][(posX + TAILLE_SPRITE) / 30] != '6') {
                                posX += 30;
                            }
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    // Vérifier si le clic est dans un bouton du menu
                    if (evenements.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = evenements.button.x;
                        int mouseY = evenements.button.y;


                      SDL_Point pointJouer = { mouseX, mouseY };
                            if (SDL_PointInRect(&pointJouer, &boutonJouer.rect)) {
                            // Action lorsque le bouton "Jouer" est cliqué
                            printf("Bouton Jouer cliqué!\n");
                            dansMenu = 0;
                       // Quitter le menu
                        } else if (SDL_PointInRect(&pointJouer, &boutonQuitter.rect)) {
                            // Action lorsque le bouton "Quitter" est cliqué
                            printf("Bouton Quitter cliqué!\n");
                            terminer = 1; // Quitter l'application
                        }
                    }
                    break;
            }
        }

        SDL_RenderClear(renderer);

        if (dansMenu) {
            // Afficher le menu avec les boutons
            SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);            
            SDL_RenderFillRect(renderer, &boutonJouer.rect);
            SDL_RenderFillRect(renderer, &boutonQuitter.rect);

            // Afficher du texte sur les boutons
            SDL_Texture* textureTexteJouer = charger_texte("Jouer", renderer, police, couleurTexte);
            SDL_Texture* textureTexteQuitter = charger_texte("Quitter", renderer, police, couleurTexte);

            SDL_Rect destRectJouer = {boutonJouer.rect.x + 10, boutonJouer.rect.y + 10, boutonJouer.rect.w - 20, boutonJouer.rect.h - 20};
            SDL_Rect destRectQuitter = {boutonQuitter.rect.x + 10, boutonQuitter.rect.y + 10, boutonQuitter.rect.w - 20, boutonQuitter.rect.h - 20};

            SDL_RenderCopy(renderer, textureTexteJouer,            NULL, &destRectJouer);
            SDL_RenderCopy(renderer, textureTexteQuitter, NULL, &destRectQuitter);

            SDL_DestroyTexture(textureTexteJouer);
            SDL_DestroyTexture(textureTexteQuitter);
        } else {
            // Afficher le terrain
            
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    char caractere = tab[i][j];
                    int indiceSprite = (caractere >= '0' && caractere <= '9') ? caractere - '0' : 0;

                    SDL_Rect destRect = {j * 32, i * 32, 32, 32};
                    SDL_Rect srcRect = {indiceSprite * 32, 0, 32, 32};
                    SDL_RenderCopy(renderer, spriteTexture, &srcRect, &destRect);
                }
            }

            // Afficher le personnage
            SDL_Rect srcRectPerso = {0, 0, TAILLE_SPRITE, TAILLE_SPRITE};
            SDL_Rect destRectPerso = {posX, posY, TAILLE_SPRITE, TAILLE_SPRITE};
            SDL_RenderCopy(renderer, textureSprite, &srcRectPerso, &destRectPerso);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }

    // Libération des ressources
    TTF_CloseFont(police);
    TTF_Quit();  // Quitter SDL_ttf

    SDL_DestroyTexture(spriteTexture);
    SDL_DestroyTexture(textureSprite);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

