#ifndef FONCTIONS_FICHIERS_H
#define FONCTIONS_FICHIERS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define JOUER 1
#define QUITTER 2
extern int timer;

extern int etatJeu;



typedef struct {
    SDL_Rect rect;  // Rectangle de position et de taille du bouton
    int action;      // Action associée au bouton (par exemple, JOUER, QUITTER)
} Bouton;


// Fonction pour créer une texture à partir d'un texte
SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer,TTF_Font *font, SDL_Color color);


//fonction pour charger une image dans une texture
SDL_Texture* charger_image(const char* nomfichier, SDL_Renderer *renderer);





char** allouer_tab_2D(int n, int m);

void desallouer_tab_2D(char** tab, int n);
void afficher_tab_2D(char** tab, int n, int m);
void taille_fichier(const char* nomFichier, int* nbLig, int* nbCol);
char** lire_fichier(const char* nomFichier);
char** modifier_caractere(char** tab, int n, int m, char ancien, char
nouveau);
void ecrire_fichier(const char* nomFichier, char** tab, int n, int m) ;

void afficher_graphe_sdl(char** tab, int n, int m); 
// SDL_Texture* charger_image_transparente(const char* nomfichier,SDL_Renderer* renderer,Uint8 r, Uint8 g, Uint8 b);

#endif // FONCTIONS_FICHIERS_H