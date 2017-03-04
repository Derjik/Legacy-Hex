/*!
 *	\file	eval_functions.c
 *	\brief	Fonctions du module d'évaluation de plateau
 *	\author	Julien Laurent, Lucas Dessaignes, Alexis Brisset
 *
 *	Ce fichier contient les définitions de fonctions d'évaluation utilisées par les
 *	fonctions de l'intelligence artificielle pour construire les arbres de jeu.
 */

#include "eval_functions.h"

/*!
 *	\author Julien Laurent
 *	\param a Premier entier à comparer
 *	\param b Deuxième entier à comparer
 *	\return Le plus petit des deux entiers passés en paramètre
 *
 *	Cette fonction renvoie le minimum des deux valeurs passées en paramètre.
 */
int low(int a, int b)
{
	return (a<b)?a:b;
}

/*!
 *	\author Julien Laurent
 *	\param a Premier entier à comparer
 *	\param b Deuxième entier à comparer
 *	\return Le plus grand des deux entiers passés en paramètre
 *
 *	Cette fonction renvoie le maximum des deux valeurs passées en paramètre.
 */
int high(int a, int b)
{
	return (a>b)?a:b;
}


/*!
 *	\author	Julien Laurent, Lucas Dessaignes, Alexis Brisset
 *	\param	p Pointeur sur le plateau à analyser
 *	\param	pion Couleur du pion à considérer
 *	\return	Valeur indicative sur l'avantage du joueur de la couleur \a pion dans l'état actuel du plateau
 *
 *	Cette fonction analyse le plateau dont l'adresse est passée en paramètre, et renvoie un "indice d'avantage"
 *	pour le joueur dont la couleur est passée en paramètre (via \a pion). *
 *	Le modèle d'analyse utilisé est basé sur le décompte des "losanges" (ou \a bridges) possédés par le joueur
 *	considéré sur le plateau dans son état actuel.
 */
int eval_losanges(plateau *p, char pion)
{
	int force_losanges = 0;
	int i,j;

	for ( i = 0 ; i < p->dim ; i++ )
	{
		for ( j = 0 ; j < p->dim ; j++ )
		{
			if(p->tab[i][j] == pion)
			{
				force_losanges += 2*compte_losanges(p, j, i);
				force_losanges += compte_connexions(p, j, i);
			}
			else if(p->tab[i][j] == couleur_opposee(pion))
			{
				force_losanges -= 4*compte_losanges(p, j, i);
				force_losanges -= 2*compte_connexions(p, j, i);
			}
		}
	}

	return force_losanges;
}

/*!
 *	\author	Julien Laurent
 *	\pre	La case passée en paramètre doit être jouée, sinon la fonction renvoie -1.
 *			De plus, si la case passée en paramètre n'existe pas, la fonction renvoie -2.
 *	\param	p Pointeur sur le plateau à analyser
 *	\param	x Abscisse de la case considérée
 *	\param	y Ordonnée de la case considérée
 *	\return	Score (intelligent) de la case, basé sur le nombre et le type de losanges
 *
 *	Cette fonction renvoie un "score intelligent" associé à une case du plateau, en comptant les
 *	losanges qui l'entourent, et en prenant en compte la couleur du pion présent sur la case considérée
 */
int compte_losanges(plateau *p, int x, int y)
{
	if(!coordExiste(x,y,p))
	{
		return -2;
	}
	char pion=p->tab[y][x];
	int nb_losanges=0;

	switch(pion)
	{
		case 'N':
			if(coordExiste(x-1, y-1, p))
			{
				if(p->tab[y-1][x-1]==pion)
				{
					nb_losanges += 2;
				}
			}

			if(coordExiste(x+1, y-2, p))
			{
				if(p->tab[y-2][x+1]==pion)
				{
					nb_losanges += 3;
				}
			}

			if(coordExiste(x+2, y-1, p))
			{
				if(p->tab[y-1][x+2]==pion)
				{
					nb_losanges += 1;
				}
			}

			if(coordExiste(x+1, y+1, p))
			{
				if(p->tab[y+1][x+1]==pion)
				{
					nb_losanges += 2;
				}
			}

			if(coordExiste(x-1, y+2, p))
			{
				if(p->tab[y+2][x-1]==pion)
				{
					nb_losanges += 3;
				}
			}

			if(coordExiste(x-2, y+1, p))
			{
				if(p->tab[y+1][x-2]==pion)
				{
					nb_losanges += 1;
				}
			}
		break;

		case 'B':
			if(coordExiste(x-1, y-1, p))
			{
				if(p->tab[y-1][x-1]==pion)
				{
					nb_losanges += 2;
				}
			}

			if(coordExiste(x+1, y-2, p))
			{
				if(p->tab[y-2][x+1]==pion)
				{
					nb_losanges += 1;
				}
			}

			if(coordExiste(x+2, y-1, p))
			{
				if(p->tab[y-1][x+2]==pion)
				{
					nb_losanges += 3;
				}
			}

			if(coordExiste(x+1, y+1, p))
			{
				if(p->tab[y+1][x+1]==pion)
				{
					nb_losanges += 2;
				}
			}

			if(coordExiste(x-1, y+2, p))
			{
				if(p->tab[y+2][x-1]==pion)
				{
					nb_losanges += 1;
				}
			}

			if(coordExiste(x-2, y+1, p))
			{
				if(p->tab[y+1][x-2]==pion)
				{
					nb_losanges += 3;
				}
			}

		break;

		default:
			nb_losanges = -1;
		break;
	}

	return nb_losanges;
}

/*!
 *	\author	Julien Laurent
 *	\pre	La case passée en paramètre doit être jouée, sinon la fonction renvoie -1
 *			De plus, si la case passée en paramètre n'existe pas, la fonction renvoie -2.
 *	\param	p Pointeur sur le plateau à analyser
 *	\param	x Abscisse de la case considérée
 *	\param	y Ordonnée de la case considérée
 *	\return	Score (intelligent) de la case, basé sur le nombre et le type de connexions directes
 *
 *	Cette fonction renvoie un "score intelligent" associé à une case du plateau, en comptant les
 *	cases de même couleur qui l'entourent, et en prenant en compte la couleur du pion présent sur
 *	la case considérée
 */
int compte_connexions(plateau *p, int x, int y)
{
	if(!coordExiste(x,y,p))
	{
		return -2;
	}
	char pion = p->tab[y][x];
	int connecs=0;

	switch(pion)
	{
		case 'N':
			if(coordExiste(x,y-1, p))
			{
				if(p->tab[y-1][x] == 'N')
				{
					connecs += 3;
				}
			}

			if(coordExiste(x+1, y-1, p))
			{
				if(p->tab[y-1][x+1] == 'N')
				{
					connecs += 2;
				}
			}

			if(coordExiste(x+1, y, p))
			{
				if(p->tab[y][x+1] == 'N')
				{
					connecs += 1;
				}
			}

			if(coordExiste(x, y+1, p))
			{
				if(p->tab[y+1][x] == 'N')
				{
					connecs += 3;
				}
			}

			if(coordExiste(x-1, y+1, p))
			{
				if(p->tab[y+1][x-1] == 'N')
				{
					connecs += 2;
				}
			}

			if(coordExiste(x-1, y, p))
			{
				if(p->tab[y][x-1] == 'N')
				{
					connecs += 1;
				}
			}
		break;

		case 'B':
			if(coordExiste(x,y-1, p))
			{
				if(p->tab[y-1][x] == 'B')
				{
					connecs += 1;
				}
			}

			if(coordExiste(x+1, y-1, p))
			{
				if(p->tab[y-1][x+1] == 'B')
				{
					connecs += 2;
				}
			}

			if(coordExiste(x+1, y, p))
			{
				if(p->tab[y][x+1] == 'B')
				{
					connecs += 3;
				}
			}

			if(coordExiste(x, y+1, p))
			{
				if(p->tab[y+1][x] == 'B')
				{
					connecs += 1;
				}
			}

			if(coordExiste(x-1, y+1, p))
			{
				if(p->tab[y+1][x-1] == 'B')
				{
					connecs += 2;
				}
			}

			if(coordExiste(x-1, y, p))
			{
				if(p->tab[y][x-1] == 'B')
				{
					connecs += 3;
				}
			}
		break;

		default:
			connecs = -1;
		break;
	}

	return connecs;
}

int eval_hasard(plateau *p, char pion)
{
	(void)p;
	(void)pion;
	return hasard(-40,40);
}

/*!
 *	\author	N/A
 *	\param	p Pointeur sur le plateau à analyser
 *	\param	pion Couleur du pion à considérer
 *	\return	Valeur indicative sur l'avantage du joueur de la couleur \a pion dans l'état actuel du plateau
 *
 *	Cette fonction analyse le plateau dont l'adresse est passée en paramètre, et renvoie un "indice d'avantage"
 *	pour le joueur dont la couleur est passée en paramètre (via \a pion).
 *	Le modèle d'analyse utilisé est basé sur l'analogie avec les systèmes électriques: les différents états
 *	possibles pour une case correspondent alors à des "conductions" différentes, et la conduction totale du
 *	plateau donne une idée de l'avantage du joueur.
 */
int eval_resistance(plateau *p, char pion)
{
	(void)p;
	(void)pion;
	int conductance = 0;



	return conductance;
}
