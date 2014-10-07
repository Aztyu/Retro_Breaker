#ifndef JEU_H_INCLUDED
#define JEU_H_INCLUDED

typedef struct Vitesse Vitesse;     //Creation de la structure Vitesse
    struct Vitesse                  //On cree une structure qui constituera la vitesse en x et y de laballe
    {
        double x;
        double y;
    };

struct Partie {                     //On cree une structure qui contient les principales variables du jeu
    int vie;
    int brique;
    int nbr_bonus;
    int bonus;
    int lancer;
    int frame;
    int niveau;
    int balleflamme;
    int plateaustick;
    int nbr_balle;
    int vitesse;
    int is_loaded;
    Vitesse Balle;
    SDL_Rect positionBalle;
}Partie;

struct Game_assets{                     //On cree une structure qui contient les principales variables du jeu
    SDL_Surface *ecran;
    SDL_Surface *imageDeFond;
    SDL_Surface *brique1;        //Initialisation des surfaces affichant les diverses briques
    SDL_Surface *brique2;
    SDL_Surface *brique3;
    SDL_Surface *brique4;
    SDL_Surface *brique6;
    SDL_Surface *brique7;
    SDL_Surface *brique8;
    SDL_Surface *brique9;
    SDL_Surface *gameover;
    SDL_Surface *plateau;
    SDL_Surface *casebonus;
    SDL_Surface *fond;
    SDL_Surface *balle;
    SDL_Surface *imagevie;
    SDL_Surface *niveau;
    SDL_Surface *fin;
    SDL_Surface *mur;
}Game_assets;

struct Bonus{                   //Cette structure et le tableau de structure permet de gerer jusqu'a 21 bonus independamment
    SDL_Rect positionBonus;
    SDL_Rect positionSprite;
    int bonustype;
}Bonus;
struct Bonus ListBonus[20];

struct Balle_sup{       //On gere ici les balles suplementaires
    Vitesse Balle;
    SDL_Rect positionBalle_sup;
    int existe;
}Balle_sup;
struct Balle_sup ListBalle_sup[4];

int tab_Collision [64][48];     //Tableau ou seront enregistrer les types de briques presents

void demarrer();
void jeu_deplacement(SDL_Surface * ecran);
void DeplaBalle(SDL_Rect * positionBalle,Vitesse * Balle,SDL_Rect * positionPlateau,SDL_Surface * ecran);
int verifCollision(int x,int y);
void VerifBonus (int * i,int * j);
void casseBrique(int x ,int y,int x2,int y2,SDL_Surface * ecran);
void DeplaBonus(SDL_Rect * positionPlateau,Vitesse * Balle);
int TypeCollision(int * i,int * j);


#endif
