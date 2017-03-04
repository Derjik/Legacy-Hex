/*!
 *	\file	data_models.h
 *	\brief	Définitions des modèles de données utilisés dans l'ensemble du logiciel
 *	\author	Julien Laurent, Lucas Dessaignes, Alexis Brisset
 *
 *	Ce fichier contient les déclarations de modèles de données utilisés dans tout le logiciel,
 *	ainsi que les prototypes de fonction établissant la "logique" du jeu (détection des gains,
 *	correspondance des couleurs, etc.)
 */

#ifndef DATA_MODELS_H_INCLUDED
#define DATA_MODELS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/// Autorise la redirection d'stdin, stdout et stderr sur la console sous Windows malgré l'utilisation de la SDL
#define WIN_DEBUG

/*!
 *	\brief	Modèle du plateau de jeu
 *	\author	Julien Laurent, Lucas Dessaignes, Alexis Brisset
 *
 *	Représente le plateau de jeu, les pions qu'il peut contenir, et des métadonnées
 *	utilisées par différentes fonctions (notamment l'IA, et #check_gain())
 *	Le constructeur de plateau est intitulé #nouveau_plateau(), déclaré dans engine_functions.h
 *	et défini dans engine_functions.c
 *	Le destructeur de plateau est intitulé #detruis_plateau(), déclaré dans engine_functions.h
 *	et défini dans engine_functions.c
 */
struct plateau
{
	int dim; ///< Dimension du plateau, initialisée lors de la construction
	char **tab; ///< Tableau contenant la représentation matricielle du contenu du plateau (N: Noir, B: Blanc, V: Vide)
	bool **checked; ///< Tableau utilisé par les fonctions de parcours (#jumperBlanc() et #jumperNoir()) pour éviter les boucles infinies
	//int **valeurN; ///< Tableau de "valeurs" des cases noires
	//int **valeurB; ///< Tableau de "valeurs" des cases blanches
};
typedef struct plateau plateau; ///< Raccourci d'utilisation du type #plateau

/*!
 *	\brief	Conteneur de coordonnées matricielles
 *	\author	Julien Laurent
 *	\warning	L'utilisation de ce modèle de données dans des fonctions de bas niveau est (particulièrement) nocive,
 *				car elle force les fonctions appelantes à instancier systématiquement un objet coord pratiquement inutile.
 *
 *	Permet la transmission d'un couple {x,y} en paramètre ou valeur de retour à travers une seule entité
 *	(clarifie les notations)
 */
struct coord
{
	int x; ///< Abscisse matricielle représentée
	int y; ///< Ordonnée matricielle représentée
};
typedef struct coord coord; ///< Raccourci d'utilisation du type #coord


/*!
 *	\brief	Modélisation du joueur
 *	\author	Julien Laurent, Lucas Dessaignes, Alexis Brisset
 *
 *	Représente le joueur, c'est à dire sa couleur et sa nature (humain, artificiel...).
 *	La fonction pointée par le membre "joue" correspond à la nature du joueur.
 *	Les joueurs doivent être respectivement instanciés et détruits par #nouveau_joueur() et #detruis_joueur().
 *	Ces fonctions sont déclarées dans engine_functions.h, et définies dans engine_functions.c.
 */
struct joueur
{
	char pion; ///< Correspond au pion associé au joueur ('N' ou 'B'), qui sera placé dans les cases qu'il joue
	coord (*joue) (plateau p, char pion, int level); ///< Pointeur vers la fonction de jeu associée au joueur
};
typedef struct joueur joueur; ///< Raccourci d'utilisation du type #joueur


/// Renvoie la couleur opposée à celle passée en paramètre
char couleur_opposee(char couleur);


/// Renvoie vrai si le dernier pion placé signe une fin de jeu
bool check_gain(int nbtours, int x, int y, plateau *p);

/// Renvoie vrai si au moins un pion de la couleur donnée est posé sur chacune des frontières de cette couleur
bool presenceLignes(char pion, plateau *p);

/// Ce jumper saute de case blanche en case blanche, et indique s'il touche l'une ou l'autre des deux frontières blanches
void jumperBlanc(int x, int y, plateau *p, bool *winning);
/// Ce jumper saute de case noire en case noire, et indique s'il touche l'une ou l'autre des deux frontières noires
void jumperNoir(int x, int y, plateau *p, bool *winning);


/// Renvoie vrai si la case aux coordonnées données existe, a la couleur demandée et n'a pas déjà été visitée par un \a jumper
bool coordValide(int x, int y, plateau *p, char pion);
/// Renvoie vrai si les coordonnées passées en paramètre sont valides, pour le plateau dont l'adresse est passée en paramètre
bool coordExiste(int x, int y, plateau *p);

/// Valeur absolue de l'entier passé en paramètre
int vabs(int v);

/// Renvoie un nombre pseudo-aléatoire dans l'intervalle [minimum, maximum]
int hasard(int minimum, int maximum);

/// Suspend l'exécution du programme pendant la durée passée en paramètre
void latence(int secondes);


#endif // DATA_MODELS_H_INCLUDED
