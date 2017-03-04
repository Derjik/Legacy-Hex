/*!
 *	\file	engine_functions.h
 *	\brief	Prototypes du moteur de jeu
 *	\author	Julien Laurent, Alexis Brisset, Lucas Dessaignes
 *
 *	Ce fichier contient l'ensemble des prototypes de fonctions liées aux moteur de jeu (création et destruction
 *	d'instances de plateaux et joueurs, lancement de partie)
 */

#ifndef ENGINE_FUNCTIONS_H_INCLUDED
#define ENGINE_FUNCTIONS_H_INCLUDED

#include "../interfaces/graph_game_ui.h"
#include "../interfaces/graph_menus.h"
//#include "console_display.h" // Inusité (migration vers SDL!)
#include "../ai/ai.h"

plateau * nouveau_plateau(int dim); ///< Crée un nouveau plateau, de la dimension passée en paramètre
void detruis_plateau(plateau **p_det); ///< Détruit le plateau pointé par le pointeur dont l'adresse est passée en paramètre

joueur * nouveau_joueur(char couleur, int type); ///< Crée un nouveau joueur, avec la couleur de pion demandée, et la fonction de jeu correspondante
void detruis_joueur(joueur **J_det); ///< Détruit le joueur pointé par le pointeur dont l'adresse est passée en paramètre
int type_joueur(joueur *J_type); ///< Renvoie l'entier correspondant au type du joueur dont le pointeur est passé en paramètre

char lance_partie(plateau *jeu, joueur *J1, joueur *J2, int difficulte, int *numero_tour, joueur **joueur_courant, SDL_Surface *imageFond, SDL_Surface *ecran); ///< Lance une partie avec les paramètres transmis

#endif // ENGINE_FUNCTIONS_H_INCLUDED
