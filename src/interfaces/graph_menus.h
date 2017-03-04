#ifndef GRAPH_MENUS_H_INCLUDED
#define GRAPH_MENUS_H_INCLUDED

#include "graph_core.h"

/* DEFINES MENUS*/

#define LARGEUR_MENU_PRINCIPAL 380
#define HAUTEUR_MENU_PRINCIPAL 110
#define ABSCISSE_COIN_PRINCIPAL 450
#define ORDONNEE_COIN_PRINCIPAL 235

#define ORDONNEE_COIN_PRINCIPAL2 395

#define ORDONNEE_COIN_PRINCIPAL3 560

#define ABSCISSE_COIN_TYPE 503
#define ORDONNEE_COIN_TYPE 367
#define LARGEUR_MENU_TYPE 277
#define HAUTEUR_MENU_TYPE 56

#define ORDONNEE_COIN_TYPE2 459
#define LARGEUR_MENU_TYPE2 277

#define ORDONNEE_COIN_TYPE3 546
#define LARGEUR_MENU_TYPE3 277

#define ABSCISSE_COIN_TYPE4 540
#define ORDONNEE_COIN_TYPE4 710
#define LARGEUR_MENU_TYPE4 202
#define HAUTEUR_MENU_TYPE4 45

#define ABSCISSE_COIN_IA 503
#define ORDONNEE_COIN_IA 367
#define LARGEUR_MENU_IA 277
#define HAUTEUR_MENU_IA 56

#define ABSCISSE_COIN_IA2 503
#define ORDONNEE_COIN_IA2 458
#define LARGEUR_MENU_IA2 277
#define HAUTEUR_MENU_IA2 56

/*!
 *	\file	graph_menus.h
 *	\brief	Prototypes des menus de configuration
 *	\author	Lucas Dessaignes, Alexis Brisset
 *
 *	Prototypes des fonctions de menus pour la configuration des parties
 */

#define ABSCISSE_COIN_IA3 503
#define ORDONNEE_COIN_IA3 546
#define LARGEUR_MENU_IA3 277
#define HAUTEUR_MENU_IA3 56

#define ABSCISSE_COIN_IA4 540
#define ORDONNEE_COIN_IA4 710
#define LARGEUR_MENU_IA4 202
#define HAUTEUR_MENU_IA4 45

#define ABSCISSE_COIN_DIFFICULTE 500
#define ORDONNEE_COIN_DIFFICULTE 363
#define LARGEUR_MENU_DIFFICULTE 277
#define HAUTEUR_MENU_DIFFICULTE 56

#define ABSCISSE_COIN_DIFFICULTE2 500
#define ORDONNEE_COIN_DIFFICULTE2 431
#define LARGEUR_MENU_DIFFICULTE2 277
#define HAUTEUR_MENU_DIFFICULTE2 56

#define ABSCISSE_COIN_DIFFICULTE3 500
#define ORDONNEE_COIN_DIFFICULTE3 498
#define LARGEUR_MENU_DIFFICULTE3 277
#define HAUTEUR_MENU_DIFFICULTE3 56

#define ABSCISSE_COIN_DIFFICULTE4 500
#define ORDONNEE_COIN_DIFFICULTE4 566
#define LARGEUR_MENU_DIFFICULTE4 277
#define HAUTEUR_MENU_DIFFICULTE4 56

#define ABSCISSE_COIN_DIFFICULTE5 500
#define ORDONNEE_COIN_DIFFICULTE5 634
#define LARGEUR_MENU_DIFFICULTE5 277
#define HAUTEUR_MENU_DIFFICULTE5 56

#define ABSCISSE_COIN_DIFFICULTE6 537
#define ORDONNEE_COIN_DIFFICULTE6 706
#define LARGEUR_MENU_DIFFICULTE6 202
#define HAUTEUR_MENU_DIFFICULTE6 45

#define ABSCISSE_COIN_COULEUR 450
#define ORDONNEE_COIN_COULEUR 402
#define LARGEUR_MENU_COULEUR 378
#define HAUTEUR_MENU_COULEUR 110

#define ABSCISSE_COIN_COULEUR2 450
#define ORDONNEE_COIN_COULEUR2 543
#define LARGEUR_MENU_COULEUR2 378
#define HAUTEUR_MENU_COULEUR2 110

#define ABSCISSE_COIN_COULEUR3 540
#define ORDONNEE_COIN_COULEUR3 710
#define LARGEUR_MENU_COULEUR3 202
#define HAUTEUR_MENU_COULEUR3 45

#define ABSCISSE_COIN_DIM 167
#define ORDONNEE_COIN_DIM 402
#define LARGEUR_CASE_DIM_1 92
#define HAUTEUR_CASE_DIM_1 86

#define ABSCISSE_COIN_DIM2 309
#define ORDONNEE_COIN_DIM2 402

#define ABSCISSE_COIN_DIM3 451
#define ORDONNEE_COIN_DIM3 402

#define ABSCISSE_COIN_DIM4 593
#define ORDONNEE_COIN_DIM4 402

#define ABSCISSE_COIN_DIM5 736
#define ORDONNEE_COIN_DIM5 402

#define ABSCISSE_COIN_DIM6 878
#define ORDONNEE_COIN_DIM6 402

#define ABSCISSE_COIN_DIM7 1020
#define ORDONNEE_COIN_DIM7 402

#define ABSCISSE_COIN_DIM8 450
#define ORDONNEE_COIN_DIM8 583
#define LARGEUR_CASE_DIM_2 378
#define HAUTEUR_CASE_DIM_2 110


/* FIN DEFINES MENUS */

/*!
 *	\brief	Sélecteur d'étape des menus de configuration du jeu
 *	\author	Julien Laurent
 *
 *	Cette énumération augmente la lisibilité de l'aiguillage des menus du jeu.
 */
enum etape_menu
{
	MENU_PRINCIPAL, ///< Sélecteur du menu principal du jeu
	CHOIX_DIMENSION, ///< Sélecteur du menu de choix de dimension de plateau
	CHOIX_TYPE_DE_JEU, ///< Sélecteur du menu de choix de type de partie
	CHOIX_COULEUR, ///< Sélecteur du menu de choix de couleur pour l'humain
	CHOIX_TYPE_IA, ///< Sélecteur du menu de choix de type d'Intelligence Artificielle
	CHOIX_NIVEAU_IA, ///< Sélecteur du menu de choix de niveau de l'Intelligence Artificielle
	CHARGEMENT_JEU, ///< Sélecteur du menu de chargement de partie
	LANCEMENT_PARTIE, ///< Sélecteur de l'étape de lancement d'une partie nouvellement configurée
	QUITTER ///< Sélecteur de sortie du programme
};

/// Raccourci d'utilisation du type #etape_menu
typedef enum etape_menu etape_menu;


/// Renvoie le choix effectué par l'utilisateur dans le menu principal
int choix_menu(SDL_Surface *fond, SDL_Surface *ecran);

/// Demande le type de partie, puis le renvoie
int choix_type(SDL_Surface *fond, SDL_Surface *ecran);

/// Demande la dimension du plateau au joueur, puis la renvoie
int choix_dimension(SDL_Surface *fond, SDL_Surface *ecran);

/// Demande la couleur de pion du joueur, puis la renvoie
char choix_couleur(SDL_Surface *fond, SDL_Surface *ecran);

/// Demande le niveau de difficulté de l'IA, puis le renvoie
int choix_level(SDL_Surface *fond, SDL_Surface *ecran);

/// Demande le type d'Intelligence Artificielle au joueur, puis le renvoie
int choix_ia(SDL_Surface *fond, SDL_Surface *ecran);

#endif // GRAPH_MENUS_H_INCLUDED
