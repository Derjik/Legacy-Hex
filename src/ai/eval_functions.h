/*!
 *	\file	eval_functions.h
 *	\brief	Prototypes du module d'évaluation de plateau
 *	\author	N/A
 *
 *	Ce fichier contient les prototypes de fonctions d'évaluation utilisées par l'intelligence artificielle
 *	pour construire les arbres de jeu, et décider des coups à jouer dans différentes situations.
 */

#ifndef EVAL_FUNCTIONS_H_INCLUDED
#define EVAL_FUNCTIONS_H_INCLUDED

#include "../model/data_models.h"

int low(int,int); ///< Renvoie le plus petit des deux entiers passés en paramètre
int high(int,int); ///< Renvoie le plus grand des deux entiers passés en paramètre

int compte_losanges(plateau *p, int x, int y); ///< Renvoie une valeur indicative sur les bridges formés autour de la case aux coordonnées passées en paramètre
int compte_connexions(plateau *p, int x, int y); ///< Renvoie une valeur indicative sur les connexions directes formées autour de la case aux coordonnées passées en paramètre

int eval_resistance(plateau *p, char pion); ///< Renvoie une valeur indicative d'avantage basée sur la résistance électrique du plateau pour le joueur passé en paramètre
int eval_losanges(plateau *p, char pion); ///< Renvoie une valeur indicative d'avantage basée sur le nombre total de \a bridges possédés par le joueur passé en paramètre
int eval_hasard(plateau *p, char pion); ///< Renvoie une valeur aléatoire dans une fourchette fixe (fonction de test, inutilisée en production)

#endif // EVAL_FUNCTIONS_H_INCLUDED
