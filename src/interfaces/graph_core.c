/*!
 *	\file graph_core.c
 *	\author	Alexis Brisset, Lucas Dessaignes, Julien Laurent
 *	\brief Fonctions du cœur de l'interface graphique
 *
 *	Définitions des fonctions qui constituent le "cœur" de l'interface graphique
 *	(initialisation de la fenêtre du programme, chargement du fond parmi une banque
 *	aléatoire, acquisition d'un simple clic, attente d'une saisie clavier...)
 */

 #include "graph_core.h"

/*!
 *	\author Julien Laurent, Alexis Brisset, Lucas Dessaignes
 *	\return Pointeur sur la fenêtre initialisée
 *
 *	Cette fonction crée la fenêtre principale utilisée par le jeu, et la renvoie
 *	à la fonction appelante, sous la forme d'un pointeur sur SDL_Surface.
 */
SDL_Surface *initFenetre()
{
	SDL_Init(SDL_INIT_VIDEO); // Initialisation de la bibliothèque SDL

	#ifdef WIN_DEBUG

	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );

	#endif

	SDL_Surface *ecran = NULL; // Le pointeur qui va stocker la surface de l'écran

	SDL_Surface *icone = SDL_LoadBMP("images/logo_small.bmp");
	SDL_SetColorKey(icone, SDL_SRCCOLORKEY, SDL_MapRGB(icone->format, 255, 255, 255));

	SDL_WM_SetIcon(icone, NULL);

	ecran = SDL_SetVideoMode(1280, 768, 32, SDL_HWSURFACE); // On tente d'ouvrir une fenêtre
	if (ecran == NULL) // Si l'ouverture a échoué, on le note et on arrête
	{
		fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_WM_SetCaption("Hexecution", "Hexecution");

	return ecran;
}

SDL_Surface *initFond()
{
	char fichierFond[25] = "images/fonds/fond_13.bmp";
	sprintf(fichierFond, "images/fonds/fond_%d.bmp", hasard(1, NOMBRE_FONDS));

	return SDL_LoadBMP(fichierFond);
}



/*!
 *	\author N/A
 *
 *	Effectue une pause SDL et attend un évènement clavier pour se terminer.
 */
void pause()
{
	int continuer = 1; // Interrupteur de boucle
	SDL_Event event; // Évènement intercepté
	while (continuer) // Tant que l'interrupteur est activé,
	{
		SDL_WaitEvent(&event); // on attend un évènement,
		switch(event.type) // et on l'analyse:
		{
			case SDL_KEYDOWN: // S'il s'agit d'une pression sur une touche,
				continuer = 0; // on coupe l'interrupteur de boucle.
			break;

			case SDL_QUIT: // S'il s'agit d'un signal de fermeture,
				exit(0); // on quitte.
			break;

			default: // Sinon, on ne fait rien.
			break;
		}
	}
	return;
}

/*!
 *	\author	Julien Laurent
 *	\return	Coordonnées-pixel du clic intercepté
 *
 *	Cette fonction attend un clic dans la fenêtre (et rien d'autre!),
 *	et en renvoie les coordonnées.
 *	Elle sert notamment pour toutes les fonctions de menus.
 */
coord get_click()
{
	SDL_Event evenement;

	coord resultat = {-1,-1}; // Le clic est initialisé à (-1,-1) comme repère pour la suite
	do
	{	// Tant qu'on n'a pas intercepté de clic, on boucle en attente.
		SDL_WaitEvent(&evenement); // Interception d'un évènement
		switch(evenement.type) // Analyse du type d'évènement
		{
			case SDL_QUIT: // En cas de signal de fermeture, on quitte
				exit(0);
			break;

			case SDL_MOUSEBUTTONUP: // En cas de clic,
				switch(evenement.button.button) // on analyse le bouton utilisé,
				{
					case SDL_BUTTON_LEFT: // et s'il s'agit du bouton gauche,
						resultat.x = evenement.button.x; // on enregistre les
						resultat.y = evenement.button.y; // coordonnées du clic.
					break;

					default: // Sinon, on ne fait rien.
					break;
				}
			break;

			default: // En cas d'autre type d'évènement, on ne fait rien
			break;
		}
	}while((resultat.x == -1)||(resultat.y == -1));

	return resultat; // On renvoie le 'clic' en coordonnées-pixel.
}

