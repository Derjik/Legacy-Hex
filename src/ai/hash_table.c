/*!
 *	\file	"hash_table.c"
 *	\author	Julien Laurent
 *	\brief	Définitions des fonctions de hashage pour l'optimisation de l'IA
 *
 *	Ce fichier contient les définitions des fonctions destinées à optimiser l'Intelligence
 *	Artificielle en procédant à une "mise en cache" des plateaux évalués, de manière
 *	à éviter des évaluations répétées d'une même situation (l'ordre dans lequel les coups
 *	sont joués n'ayant pas d'influence sur la "valeur" d'un plateau donné pour un joueur)
 */


#include "hash_table.h"

/*!
 *	\author	Julien Laurent
 *	\param	couleur Couleur à convertir
 *	\return	Entier court non-signé correspondant à la couleur passée en paramètre
 *
 *	Pour faciliter la préparation des condensats dans la fonction calculeHash(),
 *	une conversion est effectuée d'un char parmi {'N','B','V'} vers un entier
 *	court non-signé qui correspond.
 */
unsigned short int numeroCouleur(char couleur)
{
	switch(couleur)
	{
		case 'V':
			return 0;
		break;

		case 'N':
			return 1;
		break;

		case 'B':
			return 2;
		break;

		default:
			return 0;
		break;
	}
}

/*!
 *	\author	Julien Laurent
 *	\param	p Pointeur sur le plateau à hasher
 *	\return	Condensat obtenu lors du hashage du plateau
 *
 *	Cette fonction calcule un condensat (hypothétiquement unique,
 *	avec d'infimes chances de collisions) à partir des valeurs
 *	contenues dans le plateau pointé par le paramètre passé, et
 *	le renvoie.
 */
unsigned short int calculeHash(const plateau *p)
{
	unsigned short int resultat = 0;

	int i,j;


/*
 *	Cette fonction de hachage a une répartition relativement mauvaise (beaucoup de collisions...),
 *	mais est, en contrepartie, assez rapide. Le gain est plutôt ressenti dans les parties en mode
 *	"difficile"
 */
	for (i=0; i < p->dim ; i++)
	{
		for(j=0; j < p->dim ; j++)
		{
			resultat += 4 + numeroCouleur(p->tab[i][j]) * (unsigned short int)(i + j * p->dim);
		}
	}

/*
 *	Cette fonction de hachage a une très bonne répartition, mais consomme beaucoup de ressources.
 */

/*
	for ( i = 0 ; i < p->dim ; i++ )
	{
		for ( j = 0 ; j < p->dim ; j++ )
		{
			resultat = 4*resultat + numeroCouleur(p->tab[i][j]);
		}
	}
*/


	return resultat;
}

/*!
 *	\author	Julien Laurent
 *	\param	p Pointeur sur le plateau à "évaluer"
 *	\param	pion Couleur du joueur que l'on considère dans l'évaluation
 *	\return	Valeur correspondant au plateau reçu
 *
 *	Cette fonction vérifie consulte en premier lieu la table de condensats adaptée
 *	à la couleur passée en paramètre, pour voir si la configuration de plateau passée
 *	en paramètre n'a pas déjà été évaluée. Le cas échéant, la valeur stockée dans
 *	l'entrée correspondante de la table est renvoyée.
 *	Dans le cas contraire (absence de valeur dans la table des condensats), la fonction
 *	procède à une évaluation de cette configuration, puis stocke la valeur obtenue dans
 *	une nouvelle entrée de la table des condensats.
 */
int hashLosanges(plateau *p, char pion)
{
	int valeur = 0;

	int condensat = (int)calculeHash(p);

	switch(pion)
	{
		case 'N':
			if(tableNoire[condensat] != -20000)
			{
				//printf("Trouve %d indice %d\n", tableNoire[condensat], condensat);
				return tableNoire[condensat];
			}
			else
			{
				tableNoire[condensat] = eval_losanges(p,'N');
				//printf("Sauvegarde valeur %d indice %d\n", tableNoire[condensat], condensat);
				return (tableNoire[condensat]);
			}
		break;

		case 'B':
			if(tableBlanche[condensat] != -20000)
			{
				//printf("Trouve %d indice %d\n", tableBlanche[condensat], condensat);
				return tableBlanche[condensat];
			}
			else
			{
				tableBlanche[condensat] = eval_losanges(p,'B');
				//printf("Sauvegarde valeur %d indice %d\n", tableBlanche[condensat], condensat);
				return (tableBlanche[condensat]);
			}
		break;

		default:

		break;
	}

	return valeur;
}

/*!
 *	\author	Julien Laurent
 *
 *	Cette fonction alloue dynamiquement les tables de condensats,
 *	et place une valeur extrême dans chaque case, de manière à
 *	distinguer les cases "intactes" des cases effectivement remplies
 *	par la fonction hashLosanges().
 */
void initTables()
{
	int i;

	tableNoire = malloc(DIM_TABLES * sizeof(int));
	tableBlanche = malloc(DIM_TABLES * sizeof(int));

	for ( i = 0 ; i < DIM_TABLES ; i++ )
	{
		tableNoire[i] = -20000;
		tableBlanche[i] = -20000;
	}
}

/*!
 *	\author	Julien Laurent
 *
 *	Si les tables de condensats existent en mémoire, cette fonction
 *	les détruit, et met leurs pointeurs respectifs à NULL.
 */
void detruisTables()
{
	if(tableNoire != NULL)
	{
		free(tableNoire);
		tableNoire = NULL;
	}
	if(tableBlanche != NULL)
	{
		free(tableBlanche);
		tableBlanche = NULL;
	}
}
