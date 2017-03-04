/*!
 *	\file	graph_core.h
 *	\author	Alexis Brisset, Lucas Dessaignes, Julien Laurent
 *	\brief	Prototypes du cœur de l'interface graphique
 *
 *	Déclarations des fonctions qui constituent le "cœur" de l'interface graphique
 *	(initialisation de la fenêtre du programme, chargement du fond parmi une banque
 *	aléatoire, acquisition d'un simple clic, attente d'une saisie clavier...)
 */

#ifndef GRAPH_CORE_H_INCLUDED
#define GRAPH_CORE_H_INCLUDED

#include "../model/data_models.h"
#include <SDL.h>

#define NOMBRE_FONDS 17 ///< Nombre de fonds disponibles dans les ressources

/// Initialise la fenêtre principale, et renvoie le pointeur qui lui est associé
SDL_Surface *initFenetre();

/// Charge un fond d'écran au hasard et renvoie la SDL_Surface associée
SDL_Surface *initFond();

/// Attend un clic dans la fenêtre, et renvoie les coordonnées-pixel de ce clic
coord get_click();

/// Attend un évènement clavier, puis s'interrompt
void pause();

#endif // GRAPH_CORE_H_INCLUDED
