/*!
 *	\file	data_storage.h
 *	\brief	Prototypes du module d'accès à la mémoire morte
 *	\author	Julien Laurent
 *
 *	Contient les prototypes des fonctions de sauvegarde et chargement de fichiers.
 */

#ifndef DATA_STORAGE_H_INCLUDED
#define DATA_STORAGE_H_INCLUDED

#include "engine_functions.h"


int sauve_partie(plateau *p, joueur *J1, joueur *J2, int difficulte, int numero_tour, joueur *current); ///< Sauve une partie avec les paramètres passés
int charge_partie(plateau **p, joueur **J1, joueur **J2, int *difficulte, int *numero_tour, joueur **current); ///< Charge une partie dans les pointeurs dont les addresses sont passées


#endif // DATA_STORAGE_H_INCLUDED
