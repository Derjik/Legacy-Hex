/*!
 *	\file	data_models.c
 *	\brief	Fonctions régissant la logique du jeu
 *	\author	Julien Laurent, Lucas Dessaignes, Alexis Brisset
 *
 *	Les fonctions définies dans ce fichier forment les "axiomes" du jeu: elles décident
 *	du "sens" des couleurs (comme #couleur_opposee() qui renvoie la couleur "adversaire"
 *	de celle qui lui est passée en paramètre), ou encore d'à quel moment une partie est
 *	ou non (c'est le cas de #check_gain())
 */

#include "data_models.h"

/*!
 *	\author	Julien Laurent
 *	\param	couleur La couleur considérée
 *	\return	La couleur "opposée" du paramètre dans le cas de \a N ou \a B, dans les autres cas, la fonction renvoie \a V.
 *
 *	Cette fonction renvoie la couleur (de pion) opposée à celle qu'on lui passe en paramètre. Ainsi, un paramètre à \a N
 *	donnera une valeur de retour à \a B, un paramètre à \a B donnera une valeur de retour à \a N, et tout autre caractère
 *	donnera une valeur de retour à \a V (vide, dans le doute)
 */
char couleur_opposee(char couleur)
{
	switch(couleur)
	{
		case 'N':
			return 'B';
		break;

		case 'B':
			return 'N';
		break;

		default:
			return 'V';
		break;
	}
}

/*!
 *	\author	Lucas Dessaignes
 *	\param	x Abscisse matricielle de la case à vérifier
 *	\param	y Ordonnée matricielle de la case à vérifier
 *	\param	p Pointeur sur le plateau concerné
 *	\param	pion Couleur du pion demandé
 *	\return	Vrai si la case existe ET est occupée par un pion de la couleur demandée, faux si l'une des deux conditions n'est pas vérifiée.
 *
 *	Cette fonction vérifie si les coordonnées passées en paramètre correspondent à une case existante (et ne risquent donc pas de provoquer une
 *	erreur lors de l'accès au tableau p->tab du #plateau), et dans le cas où la case existe, si elle est occupée par un pion de la couleur
 *	passée en paramètre. Elle ne renvoie vrai que si ces deux conditions sont vérifiées. (dans le bon ordre, pour éviter les erreurs de
 *	segmentation)
 *
 */
bool coordValide(int x, int y, plateau *p, char pion)
{
	if(coordExiste(x,y,p))
	{
		return (!p->checked[y][x]) && (p->tab[y][x] == pion);
	}
	else
	{
		return false;
	}
}

bool coordExiste(int x, int y, plateau *p)
{
	return (x>=0)&&(y>=0)&&(x<p->dim)&&(y<p->dim) ;
}


/*!
 *	\author	Julien Laurent
 *	\param	nbtours Nombre de tours qui ont eu lieu avant l'appel. Utile pour économiser des traitements en début de partie
 *	\param	x Abscisse matricielle du dernier coup joué
 *	\param	y Ordonnée matricielle du dernier coup joué
 *	\param	p Pointeur sur le plateau à analyser
 *	\return	Vrai si le dernier pion posé est un pion gagnant, faux sinon
 *
 *	Cette fonction est chargée de vérifier les conditions nécessaires et suffisantes au gain du joueur
 *	qui vient de placer un pion sur la case correspondant aux coordonnées \a x et \a y passées en paramètre.
 *	Si ce joueur n'a pas au moins un pion présent sur chacune des deux frontières de sa couleur, le traitement
 *	est arrêté, et la fonction renvoie faux. Dans le cas contraire, une analyse récursive à l'aide de
 *	#jumperBlanc() et #jumperNoir() est effectuée.
 */
bool check_gain(int nbtours, int x, int y, plateau *p)
{
	if(nbtours < (2*p->dim)-1) // La partie ne peut être gagnée que si un nombre suffisant de pions ont été placés
	{
		return false;
	}

	int i,j; // On met à "false" les booléens anti-stack-overflow
	for ( i = 0 ; i < p->dim ; i++ )
	{
		for ( j = 0 ; j < p->dim ; j++ )
		{
			p->checked[i][j] = false;
		}
	}

	// La conjonction de ces deux booléens indique si la partie a été gagnée ou non
	bool winning[2] = {false, false};

	// Selon la couleur du dernier pion joué (celui depuis lequel on part)
	switch(p->tab[y][x])
	{
		case 'N': // On vérifie la présence d'un pion de la couleur en question sur chacune des deux frontières
				// qui correspondent:
			if(presenceLignes('N',p))
			{
				jumperNoir(x, y, p, winning); // Puis on lance le jumper qui va bien sur le pion de départ
			}
		break;

		case 'B': // On vérifie la présence d'un pion de la couleur en question sur chacune des deux frontières
				// qui correspondent:
			if(presenceLignes('B',p))
			{
				jumperBlanc(x, y, p, winning); // Puis on lance le jumper qui va bien sur le pion de départ
			}
		break;

		default:
			return false;
		break;
	}

	// On renvoie finalement la conjonction des deux conditions du gain
	return winning[0] && winning[1];
}

/*!
 *	\author	Lucas Dessaignes, Alexis Brisset
 *	\param	pion Couleur considérée (influe sur les frontières vérifiées, et les pions recherchés)
 *	\param	p Pointeur sur le plateau à analyser
 *
 *	Cette fonction parcourt les deux frontières de la couleur passée en paramètre, et renvoie vrai
 *	si au moins un pion de cette couleur est présent sur chacune des deux frontières. Sinon, elle
 *	renvoie faux. Elle est utilisée par #check_gain() à des fins d'optimisation.
 */
bool presenceLignes(char pion, plateau *p)
{
	int i;
	bool frontiere_1 = false, frontiere_2 = false;

	switch(pion)
	{
		case 'N': // Parcours des frontières noires à la recherche d'un pion noir:
			for(i = 0 ; i < p->dim ; i++)
			{
				if(p->tab[0][i] == 'N') frontiere_1 = true;
			}
			for(i = 0 ; i < p->dim ; i++)
			{
				if(p->tab[p->dim-1][i] == 'N') frontiere_2 = true;
			}
		break;

		case 'B': // Parcours des frontières blanches à la recherche d'un pion blanc:
			for(i = 0 ; i < p->dim ; i++)
			{
				if(p->tab[i][0] == 'B') frontiere_1 = true;
			}
			for(i = 0 ; i < p->dim ; i++)
			{
				if(p->tab[i][p->dim-1] == 'B') frontiere_2 = true;
			}
		break;

		default:
			return false;
		break;
	}

	return frontiere_1 && frontiere_2;
}

/*!
 *	\author	Julien Laurent, Lucas Dessaignes
 *	\param	x Abscisse matricielle de la case de départ
 *	\param	y Ordonnée matricielle de la case de départ
 *	\param	p Pointeur sur le plateau à parcourir
 *	\param	winning Tableau de deux booléens, utilisé par #check_gain() pour déterminer si la partie est finie ou non
 *
 *	Fonction récursive sautant de case blanche en case blanche à partir d'une case de départ,
 *	et plaçant la valeur \a vrai dans l'un des deux booléens du tableau \a winning dès qu'elle atteint
 *	l'une des deux frontières du joueur blanc.
 */
void jumperBlanc(int x, int y, plateau *p, bool *winning)
{
	p->checked[y][x] = true; // On marque la case pour ne pas revenir dessus par la suite

	if(x == 0) // Si on a atteint la frontière blanche de gauche, on a l'une des deux conditions de gain
	{
		winning[0] = true;
	}
	if (x == p->dim-1) // Si on a atteint la frontière blanche de droite, on a l'une des deux conditions de gain
	{
		winning[1] = true;
	}

	// On continue à sauter sur les pions blancs adjacents non-visités:

	if( coordValide(x, y-1, p, 'B') )
	{
		jumperBlanc(x, y-1, p, winning);
	}

	if (coordValide(x+1, y-1, p, 'B'))
	{
		jumperBlanc(x+1, y-1, p, winning);
	}

	if (coordValide(x+1, y, p, 'B'))
	{
		jumperBlanc(x+1, y, p, winning);
	}

	if (coordValide(x, y+1, p, 'B'))
	{
		jumperBlanc(x, y+1, p, winning);
	}

	if (coordValide(x-1, y+1, p, 'B'))
	{
		jumperBlanc(x-1, y+1, p, winning);
	}

	if (coordValide(x-1, y, p, 'B'))
	{
		jumperBlanc(x-1, y, p, winning);
	}

	return;
}

/*!
 *	\author	Julien Laurent, Lucas Dessaignes
 *	\param	x Abscisse matricielle de la case de départ
 *	\param	y Ordonnée matricielle de la case de départ
 *	\param	p Pointeur sur le plateau à parcourir
 *	\param	winning Tableau de deux booléens, utilisé par #check_gain() pour déterminer si la partie est finie ou non
 *
 *	Fonction récursive sautant de case noire en case noire à partir d'une case de départ,
 *	et plaçant la valeur \a vrai dans l'un des deux booléens du tableau \a winning dès qu'elle atteint
 *	l'une des deux frontières du joueur noir.
 */
void jumperNoir(int x, int y, plateau *p, bool *winning)
{
	p->checked[y][x] = true; // On marque la case pour ne pas revenir dessus par la suite

	if(y == 0) // Si on a atteint la frontière noire du haut, on a l'une des deux conditions de gain
	{
		winning[0] = true;
	}
	if (y == p->dim-1) // Si on a atteint la frontière noire du bas, on a l'une des deux conditions de gain
	{
		winning[1] = true;
	}

	// On continue à sauter sur les pions noirs adjacents non-visités:

	if( coordValide(x, y-1, p, 'N') )
	{
		jumperNoir(x, y-1, p, winning);
	}

	if (coordValide(x+1, y-1, p, 'N'))
	{
		jumperNoir(x+1, y-1, p, winning);
	}

	if (coordValide(x+1, y, p, 'N'))
	{
		jumperNoir(x+1, y, p, winning);
	}

	if (coordValide(x, y+1, p, 'N'))
	{
		jumperNoir(x, y+1, p, winning);
	}

	if (coordValide(x-1, y+1, p, 'N'))
	{
		jumperNoir(x-1, y+1, p, winning);
	}

	if (coordValide(x-1, y, p, 'N'))
	{
		jumperNoir(x-1, y, p, winning);
	}

	return;
}

/*!
 *	\author	Julien Laurent
 *	\param	v Nombre entier dont on souhaite obtenir la valeur absolue
 *	\return	Valeur absolue du paramètre
 *
 *	Implémentation express de la valeur absolue (utilisée par diverses fonctions du logiciel)
 */
int vabs(int v)
{
	return (v>0?v:-v);
}



/*!
 *	\author	Julien Laurent
 *	\param	minimum Valeur minimale (incluse) du nombre à renvoyer
 *	\param	maximum Valeur maximale (incluse) du nombre à renvoyer
 *	\return	Un nombre pseudo-aléatoire compris entre minimum et maximum
 *
 *	Cette fonction génère et renvoie un nombre pseudo-aléatoire avec le minimum et le maximum
 *	passés en paramètre. Elle est utilisée par la fonction #ia_hasard().
 */
int hasard(int minimum, int maximum)
{
	return (rand() % (maximum - minimum + 1)) + minimum;
}

/*!
 *	\author	Julien Laurent
 *	\param	secondes Nombre de secondes d'attente
 *
 *	Cette fonction utilise les fonctions fournies par la librairie time.h
 *	pour effectuer une pause (précisée en nombre de secondes dans l'unique
 *	paramètre) dans l'exécution, puis se terminer.
 */
void latence(int secondes)
{
	if(secondes <= 0) return;

    clock_t start,end;
    start=clock();
    while(((end=clock())-start)<=secondes*CLK_TCK);

    return;
}


