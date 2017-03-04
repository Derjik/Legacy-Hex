/*!
 *	\file	engine_functions.c
 *	\brief	Fonctions du moteur de jeu
 *	\author	Julien Laurent, Alexis Brisset, Lucas Dessaignes
 *
 *	Ce fichier contient toutes les fonctions propres au moteur:
 *	Création de nouveaux joueurs et plateaux, ou encore vérification
 *	du gain sur un plateau donné.
 */

#include "engine_functions.h"


/*!
 *	\author	Julien Laurent
 *	\param	couleur Couleur de pion du joueur nouvellement créé (N pour Noir, B pour Blanc)
 *	\param	type Type de joueur (\a 0 pour Humain, \a 1 pour IA Random)
 *	\return	Pointeur sur le joueur nouvellement créé
 *
 *	Prend un caractère en paramètre, ainsi qu'un entier correspondant au type
 *	de joueur, humain ou IA, et au type précis d'IA dans le second cas.
 *	Renvoie un pointeur sur un joueur nouvellement instancié.
 */
joueur * nouveau_joueur(char couleur, int type)
{
	joueur *nj = malloc(sizeof(joueur)); // On instancie un nouveau joueur en mémoire

	switch(type) // On lui associe alors la fonction de jeu correspondant au type souhaité
	{
		case 0:
			nj->joue = jeu_humain; // Le joueur est un humain
		break;

		case 1:
			nj->joue = ia_hasard; // Le joueur est une IA de type Brainless (joue au hasard)
		break;

		case 2:
			nj->joue = ia_losanges; // Le joueur est une IA de type Losanges (réfléchit à l'aide des bridges)
		break;

		default:
			nj->joue = ia_hasard;
		break;
	}

	nj->pion = couleur; // On associe au joueur la couleur demandée

	return nj; // Puis on renvoie le joueur nouvellement créé
}


/*!
 *	\author	Julien Laurent
 *	\param	J_det Adresse du pointeur sur le joueur à détruire
 *
 *	Détruit le joueur pointé par \a *J_det, et place la valeur \a NULL dans ce dernier.
 *	Le double référencement est nécessaire pour pouvoir modifier la valeur effective de \a *J_det.
 */
void detruis_joueur(joueur **J_det)
{
	if(*J_det != NULL) // (Sécurité anti-idioties)
	{
		free(*J_det); // Libération de l'instance de joueur
		*J_det = NULL; // Mise à NULL du pointeur
	}
	return;
}


/*!
 *	\author	Julien Laurent
 *	\param	J_type Pointeur sur le joueur dont on souhaite acquérir le type
 */
int type_joueur(joueur *J_type)
{
	if(J_type->joue == jeu_humain)
	{
		return 0;
	}
	else if(J_type->joue == ia_hasard)
	{
		return 1;
	}
	else if(J_type->joue == ia_losanges)
	{
		return 2;
	}
	else if(J_type->joue == ia_electrique)
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

/*!
 *	\author	Julien Laurent
 *	\param	dim Dimension du plateau à créer
 *	\return	Pointeur sur le plateau nouvellement instancié
 *
 *	Instancie un plateau de dimension \a dim, et place les bonnes valeurs initiales à l'intérieur.
 */
plateau * nouveau_plateau(int dim)
{
	if(dim < 0)	// (Sécurité anti-idioties)
	{
		return NULL;
	}

	int i,j; // Ces entiers sont des curseurs pour parcourir les tableaux instanciés
	plateau *np = malloc(sizeof(plateau)); // Instancie un plateau vierge

	np->dim = dim; // Indique sa dimension dans le champ prévu à cet effet

	// La matrice de caractères contient les pions (ou le vide) des différentes cases du plateau:
	np->tab = malloc(dim * sizeof(char *)); // Initialisation des lignes du plateau
	for (i = 0 ; i < dim ; i++)
	{
		np->tab[i] = malloc(dim * sizeof(char)); // Initialisation des colonnes du plateau
	}

	// La matrice booléenne sert à éviter les boucles infinies lors des parcours récursifs:
	np->checked = malloc(dim * sizeof(bool *)); // Initialisation des lignes de la matrice booléenne
	for (i = 0 ; i < dim ; i++)
	{
		np->checked[i] = malloc(dim * sizeof(bool)); // Initialisation des colonnes de la matrice booléenne
	}

	/*np->valeurN = malloc(dim * sizeof(int *)); // Initialisation des lignes de la matrice booléenne
	for (i = 0 ; i < dim ; i++)
	{
		np->valeurN[i] = malloc(dim * sizeof(int)); // Initialisation des colonnes de la matrice booléenne
	}

	np->valeurB = malloc(dim * sizeof(int *)); // Initialisation des lignes de la matrice booléenne
	for (i = 0 ; i < dim ; i++)
	{
		np->valeurB[i] = malloc(dim * sizeof(int)); // Initialisation des colonnes de la matrice booléenne
	}*/

	// Placement du symbole représentant une case vide dans chaque case du plateau:
	for ( i = 0 ; i < dim ; i++ )
	{
		for ( j = 0 ; j < dim ; j++ )
		{
			np->tab[i][j] = 'V';
		}
	}

	/*for ( i = 0 ; i < dim ; i++ )
	{
		for ( j = 0 ; j<dim+1/2 ; j++)
		{
			np->valeurB[j][i] = dim - j;
			np->valeurB[dim-j-1][i] = dim - j;
			np->valeurN[i][j] = dim - j;
			np->valeurN[i][dim-j-1] = dim - j;
		}
	}*/

	return np; // Et on renvoie le plateau nouvellement créé et configuré
}


/*!
 *	\author	Julien Laurent
 *	\param	p_det Adresse du pointeur sur le plateau à détruire
 *
 *	Cette fonction détruit le plateau pointé par \a *p_det, et place la valeur \a NULL dans ce dernier.
 */
void detruis_plateau(plateau **p_det)
{
	int i;	// Curseur de parcours

	if(*p_det != NULL)	// On ne peut détruire un plateau que s'il existe.
	{
		for ( i = 0 ; i < (*p_det)->dim ; i++ ) // Destruction du tableau de booléens
		{
			free((*p_det)->checked[i]);
		}
		free((*p_det)->checked);

		for ( i = 0 ; i < (*p_det)->dim ; i++ ) // Destruction du tableau de pions
		{
			free((*p_det)->tab[i]);
		}
		free((*p_det)->tab);

		/*for ( i = 0 ; i < (*p_det)->dim ; i++ )
		{
			free((*p_det)->valeurB[i]);
		}
		free((*p_det)->valeurB);

		for ( i = 0 ; i < (*p_det)->dim ; i++ )
		{
			free((*p_det)->valeurN[i]);
		}
		free((*p_det)->valeurN);*/

		free((*p_det)); // Destruction de la structure englobant le plateau

		*p_det = NULL; // Mise à NULL du pointeur
	}
	return;
}

/*!
 *	\author	Julien Laurent, Lucas Dessaignes, Alexis Brisset
 *	\param	jeu Pointeur sur le plateau à utiliser pour la partie
 *	\param	J1 Joueur 1 impliqué dans la partie
 *	\param	J2 Joueur 2 impliqué dans la partie
 *	\param	difficulte Difficulté de l'IA, dans le cas où une IA est présente
 *	\param	numero_tour Numéro du tour en cours au moment du lancement
 *	\param	joueur_courant Pointeur sur le joueur dont le tour est venu
 *	\param	imageFond Pointeur sur la surface contenant le décor à charger en fond (derrière le plateau)
 *	\param	ecran Pointeur sur la surface SDL sur laquelle l'affichage doit être fait
 *	\return	Couleur du gagnant de la partie
 *
 *	Régit une partie (de son début à sa fin) avec les joueurs, la difficulté d'IA et l'affichage passés en paramètre
 */

char lance_partie(plateau *jeu, joueur *J1, joueur *J2, int difficulte, int *numero_tour, joueur **joueur_courant, SDL_Surface *imageFond, SDL_Surface *ecran)
{
	if(jeu==NULL || J1==NULL || J2 == NULL || *joueur_courant==NULL || imageFond==NULL || ecran==NULL)
	{
		return 'V';
	}
	coord a_placer; // Curseur sur les dernières coordonnées jouées
	affiche_plateau(jeu, imageFond, ecran); // Affichage de l'état courant du plateau

	do
	{
		couleurJoue((*joueur_courant)->pion, ecran); // Affichage de l'indication sur le tour en cours


		a_placer = (*joueur_courant)->joue(*jeu, (*joueur_courant)->pion, difficulte); // Réception des coordonnées demandées par le joueur courant

		// Si le signal de sauvegarde est reçu, on le renvoie à la fonction appelante
		if((a_placer.x==-1)&&(a_placer.y==-1)) return 'S';

		jeu->tab[(int)a_placer.y][(int)a_placer.x] = (*joueur_courant)->pion; // Placement du pion correspondant dans la case demandée

		if(difficulte <= 8)	// La difficulté "Nash 2 en 1" implique l'incapacité, pour l'humain,
		{					// de voir les pions actuellement en jeu.
			// On met à jour la vue pour l'humain, sans avoir à replacer tous les pions
			// (pour éviter un scintillement visuel, et un gaspillage de ressources)
			ajoute_pion(a_placer.x, a_placer.y,ecran,(*joueur_courant)->pion, jeu->dim);
		}

		(*numero_tour)++; // Incrémentation du compte-tours

		// Changement de joueur courant
		if((*joueur_courant) == J1)
		{
			(*joueur_courant) = J2;
		}
		else
		{
			(*joueur_courant) = J1;
		}

		if( (J1->joue!=jeu_humain) && (J2->joue!=jeu_humain) ) // Si les deux joueurs sont des IA
		{
			if((J1->joue != ia_hasard)&&(J2->joue!=ia_hasard)) // Si les deux joueurs sont, en prime, des IA
			{												// qui réfléchissent
				pause(); // On attend un évènement au clavier avant de passer au coup suivant (afin d'éviter un blocage graphique jusqu'à la fin de la partie)
			}
			else // Sinon, s'il s'agit d'IA de type Brainless,
			{
				latence(1); // On applique une simple latence entre les coups, pour clarifier le déroulement
			}
		}


	}while(!check_gain(*numero_tour, a_placer.x, a_placer.y, jeu)); // Le jeu continue tant qu'il n'y a pas de gagnant

	// A la fin de la partie, on procède à l'affichage complet du plateau dans son état final:
	affiche_plateau(jeu, imageFond, ecran);
	// Puis on y superpose un panneau indiquant qui a gagné, et demandant d'appuyer sur une touche pour revenir au menu:
	couleurGagnant(couleur_opposee((*joueur_courant)->pion), ecran);
	// L'information est aussi relayée dans la console, en mode de debug:
	printf("%c a gagne !\n\n", couleur_opposee((*joueur_courant)->pion));
	// Un évènement SDL de type clavier/souris est attendu:
	pause();


	return couleur_opposee((*joueur_courant)->pion); // Renvoi de la couleur du gagnant
}
