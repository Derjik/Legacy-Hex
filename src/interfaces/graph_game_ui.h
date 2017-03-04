/*!
 *	\file	graph_game_ui.h
 *	\brief	Prototypes de l'interface de jeu
 *	\author	Lucas Dessaignes, Alexis Brisset, Julien Laurent
 *
 *	Prototypes des fonctions de l'interface de jeu (gestion du placement des pions,
 *	interception des actions du joueur durant la partie, affichage des informations
 *	de jeu)
 */

#ifndef GRAPH_DISPLAY_H_INCLUDED
#define GRAPH_DISPLAY_H_INCLUDED ///< Sécurité contre les boucles d'inclusions


#define L_PION 64 ///< Largeur d'un pion (en pixels).
#define H_PION 53 ///< Hauteur d'un pion (en pixels).
#define L_CASE 64 ///< Largeur d'une case du plateau (en pixels).
#define H_CASE 53 ///< Hauteur du rectangle contenu dans une case du plateau (en pixels).

#define DISTANCE_X_5 417 ///< Décalage d'abscisse pour le plateau 5x5.
#define DISTANCE_Y_5 252 ///< Décalage d'ordonnée pour le plateau 5x5.
#define DISTANCE_X_7 321 ///< Décalage d'abscisse pour le plateau 5x5.
#define DISTANCE_Y_7 199 ///< Décalage d'ordonnée pour le plateau 5x5.
#define DISTANCE_X_8 273 ///< Décalage d'abscisse pour le plateau 8x8.
#define DISTANCE_Y_8 172 ///< Décalage d'ordonnée pour le plateau 8x8.
#define DISTANCE_X_9 225 ///< Décalage d'abscisse pour le plateau 8x8.
#define DISTANCE_Y_9 146 ///< Décalage d'ordonnée pour le plateau 8x8.
#define DISTANCE_X_10 177 ///< Décalage d'abscisse pour le plateau 8x8.
#define DISTANCE_Y_10 119 ///< Décalage d'ordonnée pour le plateau 8x8.
#define DISTANCE_X_11 129 ///< Décalage d'abscisse pour le plateau 11x11.
#define DISTANCE_Y_11 94 ///< Décalage d'ordonnée pour le plateau 11x11.
#define DISTANCE_X_13 33 ///< Décalage d'abscisse pour le plateau 13x13.
#define DISTANCE_Y_13 40 ///< Décalage d'ordonnée pour le plateau 13x13.


#include "graph_core.h"


/* * * * * * * * * * * * * * * * */
/* Fonctions de configuration:   */
/* * * * * * * * * * * * * * * * */

/// Calibre l'interface de jeu en fonction de la dimension du plateau, pour le placement des pions et la gestion des clics
void calibrageGrille(int *origine_x, int *origine_y, int dim);

/// Charge le plateau adéquat dans une SDL_Surface, et renvoie le pointeur correspondant
SDL_Surface * chargePlateau(int dim);


/* * * * * * * * * * * * * */
/* Fonctions d'affichage:  */
/* * * * * * * * * * * * * */

/// Affiche l'état actuel du plateau à l'écran
int affiche_plateau(plateau *p, SDL_Surface *imageFond, SDL_Surface *ecran);

/// Ajoute l'image du pion demandé aux coordonnées-pixel demandées sur l'écran
int affiche_pion(int x_pixel, int y_pixel, SDL_Surface *ecran, char pion);

/// Ajoute un pion sur le plateau, en utilisant les coordonnées matricielles passées en paramètre
int ajoute_pion(int x_mat, int y_mat, SDL_Surface *ecran, char pion, int dimension);

/// Affiche un panneau indiquant le joueur dont c'est le tour
void couleurJoue(char couleur, SDL_Surface *ecran);

/// Affiche un panneau indiquant le joueur qui vient de gagner la partie
void couleurGagnant(char couleur, SDL_Surface *ecran);


/* * * * * * * * * * * * * * */
/* Fonctions d'acquisition:  */
/* * * * * * * * * * * * * * */

/// Appelle #get_click() jusqu'à  obtenir des coordonnées jouables à renvoyer
coord jeu_humain(plateau p, char pion, int level);

/// Attend une action de l'utilisateur et la traduit en coordonnées à jouer (ou signal de sauvegarde)
coord get_coord(int dim);


#endif // GRAPH_DISPLAY_H_INCLUDED
