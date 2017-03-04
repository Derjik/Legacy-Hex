/*!
 *	\file	ai.c
 *	\brief	Fonctions du module d'intelligence artificielle
 *	\author	Julien Laurent
 *
 *	Ce fichier contient les différentes fonctions de calcul de l'IA (notamment
 *	les implémentations du MinMax et de l'AlphaBeta).
 *	Les fonctions de construction d'arbre utilisent les fonctions d'évaluation
 *	déclarées dans engine_functions.h pour donner des valeurs aux feuilles,
 *	ainsi que la fonction check_gain() de data_models.c pour vérifier si un
 *	état donné correspond à une fin de jeu ou non.
 */
#include "ai.h"


/*!
 *	\author	Julien Laurent
 *	\param	p Copie du plateau sur lequel l'IA doit jouer
 *	\param	pion Couleur du pion joué par l'IA
 *	\param	level (Inutilisé dans le cas d'ia_hasard())
 *	\return	Coordonnées à jouer (tirées au hasard)
 *
 *	Cette "Intelligence" (sic) Artificielle joue des coups au hasard.
 */
coord ia_hasard(plateau p, char pion, int level)
{
	(void)pion;
	(void)level;
	coord a_renvoyer;

	do
	{	// On récole des couples de coordonnées au hasard
		a_renvoyer.x = hasard(0, p.dim-1);
		a_renvoyer.y = hasard(0, p.dim-1);
	}while(p.tab[a_renvoyer.y][a_renvoyer.x] != 'V');
	// Jusqu'à obtenir un couple valide

	//Puis on renvoie le tout
	return a_renvoyer;
}


/*!
 *	\author	Julien Laurent
 *	\param	p Copie du plateau sur lequel l'IA doit jouer
 *	\param	pion Couleur du pion joué par l'IA
 *	\param	level Horizon à utiliser pour l'évaluation d'un coup
 *	\return Coordonnées à jouer (choisies grâce à l'évaluation basée sur les losanges
 *
 *	Cette Intelligence Artificielle utilise l'algorithme de construction d'arbre
 *	de jeu AlphaBeta, couplé à la fonction d'évaluation eval_losanges() du fichier
 *	eval_functions.c pour renvoyer des coordonnées (valides) à jouer.
 */
coord ia_losanges(plateau p, char pion, int level)
{
	coord a_renvoyer = ia_hasard(p,pion,level);
	int i,j, val=-100, ancienne_val=-100;


	// On parcourt l'intégralité des coups actuellement jouables
	for ( i = 0 ; i < p.dim ; i++ )
	{
		for ( j = 0 ; j < p.dim ; j++ )
		{
			if(p.tab[i][j] == 'V')
			{
				p.tab[i][j] = pion; // On simule le coup

				// On récole sa valeur à l'aide de l'arbre (construction par AlphaBeta)
				val = alphaBetaMin(&p, j, i, -1000, 1000, level, pion, eval_losanges);

				p.tab[i][j] = 'V';// On dé-joue le coup

				// (Debug) On affiche la valeur relevée
				printf("Valeur du coup [%d, %d] : %d\n", j+1, i+1, val);

				if(val > ancienne_val) // Si la valeur est meilleure que les valeurs
				{					// précédemment trouvées, on la promeut.
					ancienne_val = val;
					a_renvoyer.y = i;
					a_renvoyer.x = j;
					printf("Promotion de [%d,%d] via MinMax\n", j+1, i+1);
				}
				else if(val == ancienne_val)
				{
					if(!hasard(0,2))
					{
						a_renvoyer.y = i;
						a_renvoyer.x = j;
					}
				}
			}
		}
	}

	printf("##################\nCoup choisi: [%d,%d]\n##################\n", a_renvoyer.x+1, a_renvoyer.y+1);
	return a_renvoyer;
}

/*!
 *	\author	Julien Laurent
 *	\param	p Pointeur sur le plateau à utiliser
 *	\param	x Abscisse du dernier coup joué
 *	\param	y Ordonnée du dernier coup joué
 *	\param	iterations Nombre d'itérations restantes (horizon)
 *	\param	pion Couleur du pion au sommet de l'arbre
 *	\param	eval Pointeur sur la fonction d'évaluation à utiliser
 *	\deprecated	Cette fonction est très coûteuse en ressources car elle n'effectue aucun élagage,
 *				son utilisation est donc à proscrire. (au profit de son homologue alphaBetaMin())
 *
 *	Cette fonction implémente la minimisation de l'algorithme MinMax, et utilise la fonction
 *	d'évaluation passée en paramètre pour donner une valeur aux feuilles de l'arbre à construire.
 */
int minimisation(plateau *p, int x, int y, int iterations, char pion, int (*eval) (plateau *, char))
{
	int i,j;
	int val = 100; // On veut minimiser, la valeur de départ doit donc
				// être "maximale".

	if(check_gain(p->dim*2, x, y, p))	// Si la situation reçue en paramètre correspond
	{									// à une fin de partie, on renvoie une valeur très grande
		return 100;						// (car c'est le joueur au sommet de l'arbre qui vient de jouer)
	}
	else if(iterations <= 0)	// Si le compteur d'itérations a atteint sa limite, on procède à
	{							// l'évaluation de la situation, et on renvoie la valeur obtenue
		return eval(p, pion);	// à la fonction appelante.
	}
	else // Sinon, on procède à des appels récursifs sur tous les "sous-plateaux"
	{	// possibles, et on relève la plus petite valeur possible parmi les nœuds fils
		for( i = 0 ; i < p->dim ; i++ ) // obtenus:
		{
			for( j = 0 ; j < p->dim ; j++ )
			{
				if(p->tab[i][j] == 'V')
				{
					p->tab[i][j] = couleur_opposee(pion); // On joue virtuellement le coup
					val = low(val, maximisation(p, j, i, iterations-1, pion, eval)); // On construit le sous-arbre puis on met à jour la valeur "minimale"
					p->tab[i][j] = 'V'; // On dé-joue le coup précédemment joué
				}
			}
		} // Une fois l'intégralité des sous-arbres construits, on renvoie la valeur retenue,
		return val; // qui est la plus faible parmi les valeurs des sous-nœuds.
	}
}

/*!
 *	\author	Julien Laurent
 *	\param	p Pointeur sur le plateau à utiliser
 *	\param	x Abscisse du dernier coup joué
 *	\param	y Ordonnée du dernier coup joué
 *	\param	iterations Nombre d'itérations restantes (horizon)
 *	\param	pion Couleur du pion au sommet de l'arbre
 *	\param	eval Pointeur sur la fonction d'évaluation à utiliser
 *	\deprecated	Cette fonction est très coûteuse en ressources car elle n'effectue aucun élagage,
 *				son utilisation est donc à proscrire. (au profit de son homologue alphaBetaMax())
 *
 *	Cette fonction implémente la maximisation de l'algorithme MinMax, et utilise la fonction
 *	d'évaluation passée en paramètre pour donner une valeur aux feuilles de l'arbre à construire.
 */
int maximisation(plateau *p, int x, int y, int iterations, char pion, int (*eval) (plateau *, char))
{
	int i,j;
	int val=-100; // On veut maximiser, la valeur de départ doit donc
				// être "minimale".

	if(check_gain(p->dim*2, x, y, p))	// Si la situation reçue en paramètre correspond
	{									// à une fin de partie, on renvoie une valeur très basse
		return -100;					// (car c'est le joueur "adverse" qui vient de jouer)
	}
	else if(iterations <= 0) 	// Si le compteur d'itérations a atteint sa limite, on procède à
	{							// l'évaluation de la situation, et on renvoie la valeur obtenue
		//return eval(p, pion); 	// à la fonction appelante.
		return eval(p, pion); 	// à la fonction appelante.
	}
	else // Sinon, on procède à des appels récursifs sur tous les "sous-plateaux"
	{	// possibles, et on relève la plus petite valeur possible parmi les nœuds fils
		for( i = 0 ; i < p->dim ; i++ ) // obtenus:
		{
			for( j = 0 ; j < p->dim ; j++ )
			{
				if(p->tab[i][j] == 'V')
				{
					p->tab[i][j] = pion; // On joue virtuellement le coup
					val = high(val, minimisation(p, j, i, iterations-1, pion, eval)); // On construit le sous-arbre puis on met à jour la valeur "maximale"
					p->tab[i][j] = 'V'; // On dé-joue le coup précédemment joué
				}
			}
		} // Une fois l'intégralité des sous-arbres construits, on renvoie la valeur retenue,
		return val; // qui est la plus faible parmi les valeurs des sous-nœuds.
	}
}

/*!
 *	\author Julien Laurent
 *	\param p Pointeur sur le plateau à utiliser
 *	\param x Abscisse du dernier coup joué
 *	\param y Ordonnée du dernier coup joué
 *	\param alpha Meilleure valeur maximale trouvée avant l'appel
 *	\param beta Meilleure valeur minimale trouvée avant l'appel
 *	\param iterations Nombre d'itérations restantes (horizon)
 *	\param pion Couleur du pion au sommet de l'arbre
 *	\param eval Pointeur sur la fonction d'évaluation à utiliser
 *
 *	Cette fonction implémente la minimisation de l'algorithme AlphaBeta, et utilise la fonction
 *	d'évaluation passée en paramètre pour donner une valeur aux feuilles de l'arbre à construire.
 *	A la différence de son homologue minimisation(), cette fonction ne parcourt pas naïvement l'intégralité
 *	de l'arbre de jeu, mais procède dès que possible à une coupure bêta, réduisant ainsi
 *	drastiquement le nombre de nœuds à évaluer.
 */
int alphaBetaMin(plateau *p, int x, int y, int alpha, int beta, int iterations, char pion, int (*eval) (plateau *, char))
{
	int i,j;
	int val = 100; // On veut minimiser, la valeur de départ doit donc
				// être "maximale".

	if(check_gain(p->dim*2, x, y, p))	// Si la situation reçue en paramètre correspond
	{									// à une fin de partie, on renvoie une valeur très grande
		return 100;						// (car c'est le joueur au sommet de l'arbre qui vient de jouer)
	}
	else if(iterations <= 0)	// Si le compteur d'itérations a atteint sa limite, on procède à
	{							// l'évaluation de la situation, et on renvoie la valeur obtenue
		return hashLosanges(p, pion);	// à la fonction appelante.
	}
	else // Sinon, on procède à des appels récursifs sur tous les "sous-plateaux"
	{	// possibles, et on relève la plus petite valeur possible parmi les nœuds fils
		for( i = 0 ; i < p->dim ; i++ ) // obtenus:
		{
			for( j = 0 ; j < p->dim ; j++ )
			{
				if(p->tab[i][j] == 'V')
				{
					p->tab[i][j] = couleur_opposee(pion); // On joue virtuellement le coup
					val = low(val, alphaBetaMax(p, j, i, alpha, beta, iterations-1, pion, eval)); // On construit le sous-arbre puis on met à jour la valeur "minimale"
					p->tab[i][j] = 'V'; // On dé-joue le coup précédemment joué

					if(val <= alpha) // Si la valeur qu'on vient d'obtenir durant la minimisation
					{				// est inférieure à la meilleure valeur maximale d'un ancêtre,
						return val; // on peut procéder à une coupure alpha.
					}

					beta = low(beta, val); // On met à jour la valeur de beta
				}
			}
		} // Une fois l'intégralité des sous-arbres construits, on renvoie la valeur retenue,
		return val; // qui est la plus faible parmi les valeurs des sous-nœuds.
	}
}

/*!
 *	\author Julien Laurent
 *	\param p Pointeur sur le plateau à utiliser
 *	\param x Abscisse du dernier coup joué
 *	\param y Ordonnée du dernier coup joué
 *	\param alpha Meilleure valeur maximale trouvée avant l'appel
 *	\param beta Meilleure valeur minimale trouvée avant l'appel
 *	\param iterations Nombre d'itérations restantes (horizon)
 *	\param pion Couleur du pion au sommet de l'arbre
 *	\param eval Pointeur sur la fonction d'évaluation à utiliser
 *
 *	Cette fonction implémente la maximisation de l'algorithme AlphaBeta, et utilise la fonction
 *	d'évaluation passée en paramètre pour donner une valeur aux feuilles de l'arbre à construire.
 *	A la différence de son homologue maximisation(), cette fonction ne parcourt pas naïvement l'intégralité
 *	de l'arbre de jeu, mais procède dès que possible à une coupure alpha, réduisant ainsi
 *	drastiquement le nombre de nœuds à évaluer.
 */
int alphaBetaMax(plateau *p, int x, int y, int alpha, int beta, int iterations, char pion, int (*eval) (plateau *, char))
{
	int i,j;
	int val=-100; // On veut maximiser, la valeur de départ doit donc
				// être "minimale".

	if(check_gain(p->dim*2, x, y, p))	// Si la situation reçue en paramètre correspond
	{									// à une fin de partie, on renvoie une valeur très basse
		return -100;					// (car c'est le joueur "adverse" qui vient de jouer)
	}
	else if(iterations <= 0)	// Si le compteur d'itérations a atteint sa limite, on procède à
	{							// l'évaluation de la situation, et on renvoie la valeur obtenue
		return hashLosanges(p, pion);	// à la fonction appelante.
	}
	else // Sinon, on procède à des appels récursifs sur tous les "sous-plateaux"
	{	// possibles, et on relève la plus petite valeur possible parmi les nœuds fils
		for( i = 0 ; i < p->dim ; i++ ) // obtenus:
		{
			for( j = 0 ; j < p->dim ; j++ )
			{
				if(p->tab[i][j] == 'V')
				{
					p->tab[i][j] = pion; // On joue virtuellement le coup
					val = high(val, alphaBetaMin(p, j, i, alpha, beta, iterations-1, pion, eval)); // On construit le sous-arbre puis on met à jour la valeur "maximale"
					p->tab[i][j] = 'V'; // On dé-joue le coup précédemment joué

					if(val >= beta) // Si la valeur qu'on vient d'obtenir durant la maximisation
					{				// est supérieure à la meilleure valeur minimale d'un ancêtre,
						return val; // on peut procéder à une coupure bêta.
					}

					alpha = high(alpha, val);
				}
			}
		} // Une fois l'intégralité des sous-arbres construits, on renvoie la valeur retenue,
		return val; // qui est la plus faible parmi les valeurs des sous-nœuds.
	}
}


/*!
 *	\author N/A
 *	\param	p Copie du plateau sur lequel l'IA doit jouer
 *	\param	pion Couleur du pion joué par l'IA
 *	\param	level Horizon à utiliser pour l'évaluation d'un coup
 *	\return Coordonnées à jouer (choisies grâce à l'évaluation basée sur la résistance électrique du plateau)
 *
 *	(En travaux)
 */
coord ia_electrique(plateau p, char pion, int level)
{
	return ia_hasard(p,pion,level);
}
