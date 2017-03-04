/*!
 *	\file	ai.h
 *	\brief	Prototypes du module d'intelligence artificielle
 *	\author	Julien Laurent
 *
 *	Ce fichier contient les déclarations des fonctions de calcul de l'intelligence artificielle,
 *	principalement les fonctions de construction d'arbres de jeu. (MinMax et AlphaBeta)
 *	Ces fonctions reposent sur les fonctions d'évaluation déclarées dans engine_functions.h, ainsi
 *	que de la fonction check_gain() de data_models.h.
 */
#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "hash_table.h"

/* * * * * * * * * */
/* Capsules d'IA:  */
/* * * * * * * * * */
coord ia_hasard(plateau p, char pion, int level); ///< Renvoie des coordonnées aléatoires (mais jouables) à jouer. Cette IA est particulièrement idiote, idéale pour les joueurs dépressifs.
coord ia_losanges(plateau p, char pion, int level); ///< Analyse le plateau passé en paramètre avec l'horizon \a level et la fonction #eval_losanges(), et renvoie des coordonnées (valides) à jouer
coord ia_electrique(plateau p, char pion, int level); ///< Analyse le plateau passé en paramètre avec l'horizon \a level et la fonction #eval_resistance(), et renvoie des coordonnées (valides) à jouer


/* * * * * * * * * * * */
/* Algorithme MinMax:  */
/* * * * * * * * * * * */
int minimisation(plateau *p, int x, int y, int iterations, char pion, int (*eval) (plateau *, char)); ///< Fonction de maximisation de l'algorithme MinMax
int maximisation(plateau *p, int x, int y, int iterations, char pion, int (*eval) (plateau *, char)); ///< Fonction de minimisation de l'algorithme MinMax


/* * * * * * * * * * * * */
/* Algorithme AlphaBeta: */
/* * * * * * * * * * * * */
int alphaBetaMax(plateau *p, int x, int y, int alpha, int beta, int iterations, char pion, int (*eval) (plateau *, char)); ///< Fonction de maximisation de l'algorithme AlphaBeta
int alphaBetaMin(plateau *p, int x, int y, int alpha, int beta, int iterations, char pion, int (*eval) (plateau *, char)); ///< Fonction de minimisation de l'algorithme AlphaBeta


#endif // AI_H_INCLUDED
