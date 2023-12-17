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

SDL_Texture* charger_image_transparente(const char* nomfichier, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b) {
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = NULL;
    surface = SDL_LoadBMP(nomfichier);

    const SDL_PixelFormat* format = surface->format;
    Uint32 couleur = SDL_MapRGB(format, r, g, b);

    SDL_SetColorKey(surface, SDL_TRUE, couleur);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

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
    SDL_Surface* surfaceTexte = TTF_RenderText_Solid(police, texte, couleurTexte);
    if (surfaceTexte == NULL) {
        fprintf(stderr, "Erreur lors du rendu du texte : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Texture* textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    if (textureTexte == NULL) {
        fprintf(stderr, "Erreur lors de la création de la texture : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_FreeSurface(surfaceTexte);
    return textureTexte;
}

void afficher_graphe_sdl(char** tab, int n, int m) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Jeu Labyrinthe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 320, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* spriteSurface = SDL_LoadBMP("pavage.bmp");
    SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    SDL_FreeSurface(spriteSurface);

    SDL_Texture* textureSprite = charger_image_transparente("la.bmp", renderer, 0, 0, 0);

    int posX = 0;
    int posY = 0;

    Bouton boutonJouer;
    Bouton boutonQuitter;

    int boutonLargeur = 150;
    int boutonHauteur = 50;

    int centreX = 700 / 2;
    int centreY = 320 / 2;

    boutonJouer.rect = (SDL_Rect){centreX - boutonLargeur / 2, centreY - boutonHauteur, boutonLargeur, boutonHauteur};
    boutonJouer.action = JOUER;

    boutonQuitter.rect = (SDL_Rect){centreX - boutonLargeur / 2, centreY, boutonLargeur, boutonHauteur};
    boutonQuitter.action = QUITTER;

    int dansMenu = 1;
    SDL_Event evenements;
    int terminer = 0;
    int TAILLE_SPRITE = 30;
    int timer = 0;

    TTF_Init();
    TTF_Font* police = TTF_OpenFont("Messages.ttf", 24);
    if (police == NULL) {
        fprintf(stderr, "Erreur lors du chargement de la police : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
     TTF_Font* policeMessage = TTF_OpenFont("Arcane Nine.otf", 15);
    if (policeMessage == NULL) {
        fprintf(stderr, "Erreur lors du chargement de la police : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
     TTF_Font* policeTemps = TTF_OpenFont("arial.ttf", 15);
    if (policeTemps == NULL) {
        fprintf(stderr, "Erreur lors du chargement de la police : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Color couleurTextebouton = {245, 245, 220, 255};
    SDL_Color couleurTexteMessage = {139, 117, 94, 255};;

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
                    if (evenements.button.button == SDL_BUTTON_LEFT) {
                        int mouseX = evenements.button.x;
                        int mouseY = evenements.button.y;

                        SDL_Point pointJouer = {mouseX, mouseY};
                        if (SDL_PointInRect(&pointJouer, &boutonJouer.rect)) {
                            printf("Bouton Jouer cliqué!\n");
                            dansMenu = 0;
                        } else if (SDL_PointInRect(&pointJouer, &boutonQuitter.rect)) {
                            printf("Bouton Quitter cliqué!\n");
                            terminer = 1;
                        }
                    }
                    break;
            }
        }

        SDL_RenderClear(renderer);

        if (dansMenu) {
            SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
            SDL_RenderFillRect(renderer, &boutonJouer.rect);
            SDL_RenderFillRect(renderer, &boutonQuitter.rect);

            SDL_Texture* textureTexteJouer = charger_texte("Jouer", renderer, police, couleurTextebouton);
            SDL_Texture* textureTexteQuitter = charger_texte("Quitter", renderer, police, couleurTextebouton);

            SDL_Rect destRectJouer = {boutonJouer.rect.x + 10, boutonJouer.rect.y + 10, boutonJouer.rect.w - 20, boutonJouer.rect.h - 20};
            SDL_Rect destRectQuitter = {boutonQuitter.rect.x + 10, boutonQuitter.rect.y + 10, boutonQuitter.rect.w - 20, boutonQuitter.rect.h - 20};

            SDL_RenderCopy(renderer, textureTexteJouer, NULL, &destRectJouer);
            SDL_RenderCopy(renderer, textureTexteQuitter, NULL, &destRectQuitter);

            SDL_DestroyTexture(textureTexteJouer);
            SDL_DestroyTexture(textureTexteQuitter);

            // Afficher le message sur le temps
            SDL_Texture* textureMessage = charger_texte("Trouvez le chemin en 20 secondes pour gagner !", renderer, policeMessage,couleurTexteMessage);
            SDL_Rect destRectMessage = {20, 20, 650, 30};  
            SDL_RenderCopy(renderer, textureMessage, NULL, &destRectMessage);
            SDL_DestroyTexture(textureMessage);

        } else {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    char caractere = tab[i][j];
                    int indiceSprite = (caractere >= '0' && caractere <= '9') ? caractere - '0' : 0;

                    SDL_Rect destRect = {j * 32, i * 32, 32, 32};
                    SDL_Rect srcRect = {indiceSprite * 32, 0, 32, 32};
                    SDL_RenderCopy(renderer, spriteTexture, &srcRect, &destRect);
                }
            }

            SDL_Rect srcRectPerso = {0, 0, TAILLE_SPRITE, TAILLE_SPRITE};
            SDL_Rect destRectPerso = {posX, posY, TAILLE_SPRITE, TAILLE_SPRITE};
            SDL_RenderCopy(renderer, textureSprite, &srcRectPerso, &destRectPerso);

            if (!dansMenu) {
                timer = SDL_GetTicks() / 1000;

                char timerTexte[50];
                sprintf(timerTexte, "Temps : %d", timer);

                SDL_Texture* textureTimer = charger_texte(timerTexte, renderer, policeTemps, couleurTextebouton);

                int timerPosX = 700 - 67;
                int timerPosY = 10;
                int timerWidth = 60;
                int timerHeight = 30;

                SDL_Rect destRectTimer = {timerPosX, timerPosY, timerWidth, timerHeight};
                SDL_RenderCopy(renderer, textureTimer, NULL, &destRectTimer);

                SDL_DestroyTexture(textureTimer);
            }

        }

        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }

    TTF_CloseFont(police);
    TTF_Quit();
    SDL_DestroyTexture(spriteTexture);
    SDL_DestroyTexture(textureSprite);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

