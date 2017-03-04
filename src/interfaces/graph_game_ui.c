/*!
 *	\file	graph_game_ui.c
 *	\brief	Fonctions de l'interface de jeu
 *	\author	Lucas Dessaignes, Alexis Brisset, Julien Laurent
 *
 *	Définitions des fonctions de l'interface de jeu (gestion du placement des pions,
 *	interception des actions du joueur durant la partie, affichage des informations
 *	de jeu).
 */

#include "graph_game_ui.h"




/*!
 *	\author	Lucas Dessaignes
 *	\param	x_pixel Abscisse (en pixels) à utiliser pour le coin haut-gauche de l'image
 *	\param	y_pixel Ordonnée (en pixels) à utiliser pour le coin haut-gauche de l'image
 *	\param	ecran Pointeur sur la surface SDL à utiliser lors de l'affichage
 *	\param	pion Caractère correspondant au pion demandé (\a N: noir, \a B: blanc, Autre: vide)
 *	\return	Code d'erreur, utilisable à des fins de débogage
 *
 *	Cette fonction ajoute l'image correspondant au pion demandé au plateau déjà affiché
 *	sur la surface \a ecran, en utilisant directement les coordonnées-pixel \a x_pixel et
 *	\a y_pixel. Elle ne devrait pas être appelée seule, mais plutôt enveloppée dans une fonction
 *	comme #affiche_plateau().
 */
int affiche_pion(int x_pixel, int y_pixel, SDL_Surface *ecran, char pion)
{
	SDL_Surface *imagePion = NULL; // Image correspondant au pion à afficher
	if (pion == 'N')
	{	// Pion noir
		imagePion = SDL_LoadBMP("images/pions/Noir.bmp");
	}
	else if (pion == 'B')
	{	// Pion blanc
		imagePion = SDL_LoadBMP("images/pions/Blanc.bmp");
	}
	else
	{	// Inutile d'aller plus loin si le pion demandé n'est pas valide
		return -1;
	}

	// Réglage de la couleur de transparence pour le pion à afficher:
	SDL_SetColorKey(imagePion, SDL_SRCCOLORKEY, SDL_MapRGB(imagePion->format, 255,255,255));

	// Réglage
	SDL_Rect positionPion;

	positionPion.x = x_pixel;
	positionPion.y = y_pixel;

	SDL_BlitSurface(imagePion, NULL, ecran, &positionPion);


	SDL_FreeSurface(imagePion);

	return 0;
}

/*!
 *	\author	Julien Laurent
 *	\param	x_mat Abscisse matricielle du pion à placer
 *	\param	y_mat Ordonnée matricielle du pion à placer
 *	\param	ecran Pointeur sur la surface SDL à utiliser lors de l'affichage
 *	\param	pion Couleur du pion à ajouter (\a N: Noir, \a B: Blanc, Autre: vide)
 *	\param	dim Dimension du plateau (pour calculer les décalages)
 *	\return	Code d'erreur, utilisable à des fins de debugging
 *
 *	Variante de la fonction #ajoute_pion(), elle effectue une conversion des coordonnées
 *	matricielles \a x_mat et \a y_mat en coordonnées pixel, ce à l'aide de la \a dimension du plateau
 *	passée en paramètre.
 */
int ajoute_pion(int x_mat, int y_mat, SDL_Surface *ecran, char pion, int dim)
{
	int origine_x, origine_y;

	calibrageGrille(&origine_x, &origine_y, dim);


	affiche_pion(origine_x + y_mat*L_CASE/2 + x_mat*L_PION, origine_y + y_mat*H_PION, ecran, pion);

	SDL_Flip(ecran);

	return 0;
}

/*!
 *	\author	Lucas Dessaignes
 *	\param	p Pointeur sur le plateau à afficher
 *	\param	imageFond Pointeur sur le fond d'écran à placer derrière le plateau
 *	\param	ecran Pointeur sur la surface SDL à utiliser lors de l'affichage
 *	\return	Code d'erreur, utilisable à des fins de debugging
 *
 *	A chaque appel, la fonction reconstruit le plateau tout entier en affichant l'image de fond
 *	appropriée, pour lui superposer les images des pions correspondant au contenu de la matrice
 *	\a p->tab. Cette fonction a le principal défaut de reconstruire intégralement le plateau,
 *	contrairement à la fonction #ajoute_pion(), qui ne fait qu'ajouter le pion passé en paramètre
 *	à la surface déjà affichée. affiche_plateau ne doit donc être appelée qu'en début de partie,
 *	ou lors du chargement d'une partie sauvée sur disque.
 */
int affiche_plateau(plateau *p, SDL_Surface *imageFond, SDL_Surface *ecran)
{
	SDL_Surface *imagePlateau = NULL;
	SDL_Rect positionFond;
	positionFond.x = 0;
	positionFond.y = 0;

	int i,j, origine_x, origine_y;

	imagePlateau = chargePlateau(p->dim);
	calibrageGrille(&origine_x, &origine_y, p->dim);

	// On affiche l'image de fond (écrasant tout ce qui avait été affiché jusqu'à présent, au passage):
	SDL_BlitSurface(imageFond, NULL, ecran, &positionFond);

	// On prépare la transparence:
	SDL_SetColorKey(imagePlateau, SDL_SRCCOLORKEY, SDL_MapRGB(imagePlateau->format, 255, 0, 0));

	// On blitte par-dessus l'écran
	SDL_BlitSurface(imagePlateau, NULL, ecran, &positionFond);

	for( i = 0 ; i < p->dim ; i++ )
	{
		for ( j = 0 ; j < p->dim ; j++ )
		{
			if(p->tab[i][j]!='V')
			{
				affiche_pion(origine_x + i * L_CASE/2 + j*L_PION, origine_y+i*H_PION, ecran, p->tab[i][j]);
			}
		}
	}

	// On intervertit l'image de tampon avec l'image affichée:
	SDL_Flip(ecran);

	// On libère les surfaces:
	SDL_FreeSurface(imagePlateau);

	return 0;
}

/*!
 *	\author	Alexis Brisset
 *	\param	p Copie du plateau de jeu
 *	\param	pion Couleur de pion du joueur appelé
 *	\param	level Paramètre muet, utile uniquement pour les fonctions d'IA
 *	\return	Coordonnées valides jouées par le joueur, ou signal de sauvegarde {-1, -1}
 *
 *	Cette fonction appelle #get_coord() autant de fois que nécessaire, et vérifie si les coordonnées matricielles renvoyées
 *	correspondent à des coordonnées existantes du plateau (et disponibles), ou au signal conventionnel de sauvegarde de partie.
 */
coord jeu_humain(plateau p, char pion, int level)
{
	(void)pion;
	(void)level;
	coord a_renvoyer;
	a_renvoyer.x=-2;
	a_renvoyer.y=-2;

	do
	{
		a_renvoyer = get_coord(p.dim);
	//La condition de sortie s'architecture comme suit:
	//		[Signal de sauvegarde dans le conteneur]  OU  [Coordonnées "valides" dans le conteneur, et correspondant à une case libre du plateau]
	}while( ((a_renvoyer.x!=-1)||(a_renvoyer.y!=-1)) && (a_renvoyer.x<0 || a_renvoyer.x>=p.dim || a_renvoyer.y<0 || a_renvoyer.y>=p.dim || p.tab[a_renvoyer.y][a_renvoyer.x]!='V') );

	return a_renvoyer;
}

/*!
 *	\author	Alexis Brisset
 *	\param	dim Dimension du plateau (pour calculer les décalages propres à l'image)
 *	\return	Coordonnées matricielles du clic, ou signal de sauvegarde en cas de pression sur la touche S du clavier
 *
 *	Cette boucle d'interception d'évènement ne réagit que lors d'un clic sur une case effective du plateau (approximée par un
 *	rectangle), ou un appui sur la touche S du clavier (pour renvoyer le signal de sauvegarde, au lieu de coordonnées à jouer)
 */
coord get_coord(int dim)
{
	SDL_Event event; // Événement SDL qui sera utilisé par la suite
	SDL_Rect coordPix; // Conteneur pour les coordonnées-pixel du clic intercepté
	int i, j, origine_x, origine_y;
	coord resultat; // Conteneur pour les coordonnées matricielles à renvoyer
	resultat.x=-2; // On place des valeurs spéciales dans resultat, pour
	resultat.y=-2; // savoir quand la boucle doit s'arrêter.

	int continuer = 1; // Interrupteur pour la boucle d'interception

	calibrageGrille(&origine_x, &origine_y, dim);

	while (continuer) // Tant qu'un évènement "valide" n'a pas été intercepté,
	{
		SDL_WaitEvent(&event); // on attend un évènement,
		switch(event.type) // puis on l'analyse:
		{
			case SDL_QUIT: // S'il s'agit d'un signal de fermeture, on quitte,
				exit(0);
			break;

			case SDL_MOUSEBUTTONUP: // s'il s'agit d'un clic,
				switch(event.button.button) // on analyse le bouton utilisé:
				{
					case SDL_BUTTON_LEFT: // S'il s'agit du bouton gauche,
						coordPix.x = event.button.x; // on relève les coordonnées
						coordPix.y = event.button.y; // du clic, puis
						for ( i = 0 ; i < dim; i++ ) // on tente de le convertir
						{						// en ordonnée matricielle à l'aide
							if ((coordPix.y > H_CASE*i + origine_y)&&(coordPix.y<origine_y+(i+1)*H_CASE))
							{					// d'un modèle basé sur des "bandes" horizontales valides.
								resultat.y = i;
							}
						}
						for(j=0;j<dim;j++) // Connaissant alors l'ordonnée matricielle, on peut
						{			// tenter d'associer l'abscisse-pixel à une abscisse matricielle valide
							if ((coordPix.x > origine_x+resultat.y* (L_CASE/2) + j*L_CASE)&& coordPix.x < origine_x+ (L_CASE/2)*(resultat.y)+(j+1)*L_CASE)
							{		// en prenant compte des décalages induits par la disposition des cases.
								resultat.x = j;
							}
						}
						continuer = 0; // On vient d'intercepter un clic, on peut donc arrêter
					break;				// de boucler

					default:
						// S'il ne s'agit pas d'un clic droit, on ne fait rien.
					break;
				}
			break;

			case SDL_KEYDOWN: // S'il s'agit d'un appui sur une touche du clavier,
				switch(event.key.keysym.sym) // on analyse cette touche:
				{
					case SDLK_s: // S'il s'agit de la touche S, on stocke le signal
						resultat.x = -1; // de sauvegarde dans le conteneur à renvoyer
						resultat.y = -1;
						continuer = 0; // et on éteint l'interrupteur de la boucle
					break;

					case SDLK_ESCAPE: // S'il s'agit de la touche Echap, on quitte le jeu sans sauvegarder
						exit(0);	// ni passer par le menu principal
					break;

					default:
						// s'il s'agit d'une autre touche, on ne fait rien.
					break;
				}
			break;

			default:
				// S'il s'agit d'un autre type d'évènement, on ne fait rien.
			break;
		}
	}
	return resultat; // On renvoie le conteneur obtenu.
}

/*!
 *	\author	Alexis Brisset, Lucas Dessaignes
 *	\param	origine_x Pointeur sur la variable de décalage d'abscisse à calibrer
 *	\param	origine_y Pointeur sur la variable de décalage d'ordonnée à calibrer
 *	\param	dim Dimension du plateau (pour choisir les bonnes constantes)
 *
 *	Cette fonction charge met à jour les variables de décalage dont les adresses sont
 *	passées en paramètre, de manière à adapter l'entrée/sortie de la fonction appelante
 *	à la dimension du plateau de jeu actuel.
 */
void calibrageGrille(int *origine_x, int *origine_y, int dim)
{
	switch (dim)
	{		// Réglages à effectuer en fonction de la dimension du plateau
		case 5:
			(*origine_x) = DISTANCE_X_5;
			(*origine_y) = DISTANCE_Y_5;
		break;
		case 7:
			(*origine_x) = DISTANCE_X_7;
			(*origine_y) = DISTANCE_Y_7;
		break;
		case 8:
			(*origine_x) = DISTANCE_X_8;
			(*origine_y) = DISTANCE_Y_8;
		break;
		case 9:
			(*origine_x) = DISTANCE_X_9;
			(*origine_y) = DISTANCE_Y_9;
		break;
		case 10:
			(*origine_x) = DISTANCE_X_10;
			(*origine_y) = DISTANCE_Y_10;
		break;
		case 11:
			(*origine_x) = DISTANCE_X_11;
			(*origine_y) = DISTANCE_Y_11;
		break;
		case 13:
			(*origine_x) = DISTANCE_X_13;
			(*origine_y) = DISTANCE_Y_13;
		break;
		default:
			(*origine_x) = DISTANCE_X_5;
			(*origine_y) = DISTANCE_Y_5;
		break;
	}

	return ;
}

/*!
 *	\author	Alexis Brisset, Lucas Dessaignes
 *	\param	dim Dimension du plateau (pour choisir le bon fichier)
 *	\return	Pointeur sur la surface correspondant au plateau à afficher
 *
 *	Cette fonction charge l'image correspondant à la dimension passée en paramètre dans une
 *	surface SDL, et renvoie le pointeur obtenu.
 */
SDL_Surface * chargePlateau(int dim)
{
	SDL_Surface *imagePlateau = NULL;
	switch (dim)
	{
		case 5:
			imagePlateau = SDL_LoadBMP("images/plateaux/plateau_1280x768_5x5.bmp");
		break;
		case 7:
			imagePlateau = SDL_LoadBMP("images/plateaux/plateau_1280x768_7x7.bmp");
		break;
		case 8:
			imagePlateau = SDL_LoadBMP("images/plateaux/plateau_1280x768_8x8.bmp");
		break;
		case 9:
			imagePlateau = SDL_LoadBMP("images/plateaux/plateau_1280x768_9x9.bmp");
		break;
		case 10:
			imagePlateau = SDL_LoadBMP("images/plateaux/plateau_1280x768_10x10.bmp");
		break;
		case 11:
			imagePlateau = SDL_LoadBMP("images/plateaux/plateau_1280x768_11x11.bmp");
		break;
		case 13:
			imagePlateau = SDL_LoadBMP("images/plateaux/plateau_1280x768_13x13.bmp");
		break;
		default:
			imagePlateau = SDL_LoadBMP("images/plateaux/plateau_1280x768_5x5.bmp");
		break;
	}

	return imagePlateau;
}


void couleurJoue(char couleur, SDL_Surface *ecran)
{
	SDL_Surface *panneau = NULL;

	SDL_Rect positionPanneau;
	positionPanneau.x = 15;
	positionPanneau.y = 643;

	switch(couleur)
	{
		case 'N':
			panneau = SDL_LoadBMP("images/panneaux/NoirJoue.bmp");
		break;

		case 'B':
			panneau = SDL_LoadBMP("images/panneaux/BlancJoue.bmp");
		break;

		default:
		break;
	}

	if(panneau != NULL)
	{
		SDL_SetColorKey(panneau, SDL_SRCCOLORKEY, SDL_MapRGB(panneau->format, 255,0,0));
		SDL_BlitSurface(panneau, NULL, ecran, &positionPanneau);
		SDL_Flip(ecran);
		SDL_FreeSurface(panneau);
	}

	return ;
}

void couleurGagnant(char couleur, SDL_Surface *ecran)
{
	SDL_Surface *panneau = NULL;

	SDL_Rect positionPanneau;
	positionPanneau.x = 0;
	positionPanneau.y = 0;

	switch(couleur)
	{
		case 'N':
			panneau = SDL_LoadBMP("images/panneaux/NoirGagne.bmp");
		break;

		case 'B':
			panneau = SDL_LoadBMP("images/panneaux/BlancGagne.bmp");
		break;

		default:
		break;
	}

	if(panneau != NULL)
	{
		SDL_SetColorKey(panneau, SDL_SRCCOLORKEY, SDL_MapRGB(panneau->format, 255,0,0));
		SDL_BlitSurface(panneau, NULL, ecran, &positionPanneau);
		SDL_Flip(ecran);
		SDL_FreeSurface(panneau);
	}

	return ;
}




