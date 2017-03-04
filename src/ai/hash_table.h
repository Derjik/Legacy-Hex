/*!
 *	\file	"hash_table.h"
 *	\author	Julien Laurent
 *	\brief	Prototypes des fonctions de hashage pour l'optimisation de l'IA
 *
 *	Ce fichier contient les prototypes des fonctions destinées à optimiser l'Intelligence
 *	Artificielle en procédant à une "mise en cache" des plateaux évalués, de manière
 *	à éviter des évaluations répétées d'une même situation (l'ordre dans lequel les coups
 *	sont joués n'ayant pas d'influence sur la "valeur" d'un plateau donné pour un joueur)
 */

#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED

#include "eval_functions.h"

#define DIM_TABLES 65535 ///< Dimension des tables de condensats

/// Convertit une couleur de pion en entier court non-signé (pour le hachage)
unsigned short int numeroCouleur(char couleur);

/// Renvoie un entier court non-signé correspondant au condensat du plateau passé en paramètre
unsigned short int calculeHash(const plateau *p);

/// Fonction de "mise en cache" (écriture et lecture des tables de condensats pour les évaluations)
int hashLosanges(plateau *p, char pion);

/// Fonction d'initialisation des tables de condensats
void initTables();

/// Fonction de destruction (et remise à NULL) des tables de condensats
void detruisTables();

/// Table de condensats pour le joueur noir
int *tableNoire;

/// Table de condensats pour le joueur blanc
int *tableBlanche;

#endif // HASH_TABLE_H_INCLUDED
