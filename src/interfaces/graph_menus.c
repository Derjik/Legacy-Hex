/*!
 *	\file	graph_menus.c
 *	\brief	Fonctions des menus de configuration
 *	\author	Lucas Dessaignes, Alexis Brisset
 *
 *	Définitions des fonctions de menus pour la configuration des parties
 */

#include "graph_menus.h"


/*!
 *	\author	Alexis Brisset
 *	\param	fond Fond à afficher derrière le menu
 *	\param 	ecran Pointeur sur la surface d'affichage
 *	\return	Choix principal de l'utilisateur
 *
 *	Prompte l'utilisateur pour obtenir un choix,
 *	et le renvoie.
 */
int choix_menu(SDL_Surface *fond, SDL_Surface *ecran)
{
	int choix=-1;

	SDL_Surface *image = NULL;
	SDL_Rect positionImage;

	positionImage.x = 0;
	positionImage.y = 0;

	image = SDL_LoadBMP("images/menus/menu_principal.bmp");
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 0, 0));

	SDL_BlitSurface(fond, NULL, ecran, &positionImage);
	SDL_BlitSurface(image, NULL, ecran, &positionImage);

	SDL_Flip(ecran);

	coord positionClick;
	positionClick.x = 0;
	positionClick.y = 0;

	do
	{
			positionClick = get_click();

			if ((positionClick.x >= ABSCISSE_COIN_PRINCIPAL) && (positionClick.x <= ABSCISSE_COIN_PRINCIPAL+LARGEUR_MENU_PRINCIPAL) && (positionClick.y >= ORDONNEE_COIN_PRINCIPAL) && (positionClick.y <= ORDONNEE_COIN_PRINCIPAL + HAUTEUR_MENU_PRINCIPAL ))
			choix = 1; // renvoie le choix du click sur "Nouvelle partie"

			if ((positionClick.x >= ABSCISSE_COIN_PRINCIPAL) && (positionClick.x <= ABSCISSE_COIN_PRINCIPAL+LARGEUR_MENU_PRINCIPAL) && (positionClick.y >= ORDONNEE_COIN_PRINCIPAL2) && (positionClick.y <= ORDONNEE_COIN_PRINCIPAL2 + HAUTEUR_MENU_PRINCIPAL ))
			choix = 2; // renvoie le choix du click sur "Charger partie"

			if ((positionClick.x >= ABSCISSE_COIN_PRINCIPAL) && (positionClick.x <= ABSCISSE_COIN_PRINCIPAL+LARGEUR_MENU_PRINCIPAL) && (positionClick.y >= ORDONNEE_COIN_PRINCIPAL3) && (positionClick.y <= ORDONNEE_COIN_PRINCIPAL3 + HAUTEUR_MENU_PRINCIPAL ))
			choix = 0; // renvoie le choix du click sur "GoFuckYourself (ou quitter)"

	}while(choix == -1);

	SDL_FreeSurface(image);
	return choix;
}

/*!
 *	\author	Alexis Brisset
 *	\param	fond Fond à afficher derrière le menu
 *	\param 	ecran Pointeur sur la surface d'affichage
 *	\return	Type de partie choisi par l'utilisateur
 *
 *	Affiche les dimensions disponibles, et interroge l'utilisateur sur son choix.
 */
int choix_type(SDL_Surface *fond, SDL_Surface *ecran)
{
	int choix=-1;

	SDL_Surface *image = NULL;
	SDL_Rect positionImage;

	positionImage.x = 0;
	positionImage.y = 0;

	image = SDL_LoadBMP("images/menus/menu_type.bmp");
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 0, 0));

	SDL_BlitSurface(fond, NULL, ecran, &positionImage);
	SDL_BlitSurface(image, NULL, ecran, &positionImage);

	SDL_Flip(ecran);

	coord positionClick;
	positionClick.x = 0;
	positionClick.y = 0;

	do
	{
			positionClick = get_click();

			if ((positionClick.x >= ABSCISSE_COIN_TYPE) && (positionClick.x <= ABSCISSE_COIN_TYPE + LARGEUR_MENU_TYPE) && (positionClick.y >= ORDONNEE_COIN_TYPE) && (positionClick.y <= ORDONNEE_COIN_TYPE + HAUTEUR_MENU_TYPE ))
			choix = 1; // renvoie le choix du click sur "Humain vs Humain"

			if ((positionClick.x >= ABSCISSE_COIN_TYPE) && (positionClick.x <= ABSCISSE_COIN_TYPE + LARGEUR_MENU_TYPE2) && (positionClick.y >= ORDONNEE_COIN_TYPE2) && (positionClick.y <= ORDONNEE_COIN_TYPE2 + HAUTEUR_MENU_TYPE ))
			choix = 2; // renvoie le choix du click sur "Humain vs Machine"

			if ((positionClick.x >= ABSCISSE_COIN_TYPE) && (positionClick.x <= ABSCISSE_COIN_TYPE + LARGEUR_MENU_TYPE3) && (positionClick.y >= ORDONNEE_COIN_TYPE3) && (positionClick.y <= ORDONNEE_COIN_TYPE3 + HAUTEUR_MENU_TYPE ))
			choix = 3; // renvoie le choix du click sur "Machine vs Machine"

			if ((positionClick.x >= ABSCISSE_COIN_TYPE4) && (positionClick.x <= ABSCISSE_COIN_TYPE4 + LARGEUR_MENU_TYPE4) && (positionClick.y >= ORDONNEE_COIN_TYPE4) && (positionClick.y <= ORDONNEE_COIN_TYPE4 + HAUTEUR_MENU_TYPE4 ))
			choix = 0; // renvoie le choix du click sur "Retour"

	}while(choix == -1);

	SDL_FreeSurface(image);
	return choix;
}



/*!
 *	\author	Alexis Brisset
 *	\param	fond Fond à afficher derrière le menu
 *	\param 	ecran Pointeur sur la surface d'affichage
 *	\return	Dimension choisie par l'utilisateur
 *
 *	Affiche les dimensions disponibles, et interroge l'utilisateur sur son choix.
 */
int choix_dimension(SDL_Surface *fond, SDL_Surface *ecran)
{
	int dim = -1;

	SDL_Surface *image = NULL;
	SDL_Rect positionImage;

	positionImage.x = 0;
	positionImage.y = 0;

	image = SDL_LoadBMP("images/menus/menu_dimension.bmp");

	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 0, 0));

	SDL_BlitSurface(fond, NULL, ecran, &positionImage);
	SDL_BlitSurface(image, NULL, ecran, &positionImage);

	SDL_Flip(ecran);

	coord positionClick;
	positionClick.x = 0;
	positionClick.y = 0;
	do
	{
		positionClick = get_click();

		if ((positionClick.x >= ABSCISSE_COIN_DIM) && (positionClick.x <= ABSCISSE_COIN_DIM + LARGEUR_CASE_DIM_1) && (positionClick.y >= ORDONNEE_COIN_DIM) && (positionClick.y <= ORDONNEE_COIN_DIM + HAUTEUR_CASE_DIM_1 ))
		dim = 5;

		if ((positionClick.x >= ABSCISSE_COIN_DIM2) && (positionClick.x <= ABSCISSE_COIN_DIM2 + LARGEUR_CASE_DIM_1) && (positionClick.y >= ORDONNEE_COIN_DIM2) && (positionClick.y <= ORDONNEE_COIN_DIM2 + HAUTEUR_CASE_DIM_1 ))
		dim = 7;

		if ((positionClick.x >= ABSCISSE_COIN_DIM3) && (positionClick.x <= ABSCISSE_COIN_DIM3 + LARGEUR_CASE_DIM_1) && (positionClick.y >= ORDONNEE_COIN_DIM3) && (positionClick.y <= ORDONNEE_COIN_DIM3 + HAUTEUR_CASE_DIM_1 ))
		dim = 8;

		if ((positionClick.x >= ABSCISSE_COIN_DIM4) && (positionClick.x <= ABSCISSE_COIN_DIM4 + LARGEUR_CASE_DIM_1) && (positionClick.y >= ORDONNEE_COIN_DIM4) && (positionClick.y <= ORDONNEE_COIN_DIM4 + HAUTEUR_CASE_DIM_1 ))
		dim = 9;

		if ((positionClick.x >= ABSCISSE_COIN_DIM5) && (positionClick.x <= ABSCISSE_COIN_DIM5 + LARGEUR_CASE_DIM_1) && (positionClick.y >= ORDONNEE_COIN_DIM5) && (positionClick.y <= ORDONNEE_COIN_DIM5 + HAUTEUR_CASE_DIM_1 ))
		dim = 10;

		if ((positionClick.x >= ABSCISSE_COIN_DIM6) && (positionClick.x <= ABSCISSE_COIN_DIM6 + LARGEUR_CASE_DIM_1) && (positionClick.y >= ORDONNEE_COIN_DIM6) && (positionClick.y <= ORDONNEE_COIN_DIM6 + HAUTEUR_CASE_DIM_1 ))
		dim = 11;

		if ((positionClick.x >= ABSCISSE_COIN_DIM7) && (positionClick.x <= ABSCISSE_COIN_DIM7 + LARGEUR_CASE_DIM_1) && (positionClick.y >= ORDONNEE_COIN_DIM7) && (positionClick.y <= ORDONNEE_COIN_DIM7 + HAUTEUR_CASE_DIM_1 ))
		dim = 13;

		if ((positionClick.x >= ABSCISSE_COIN_DIM8) && (positionClick.x <= ABSCISSE_COIN_DIM8 + LARGEUR_CASE_DIM_2) && (positionClick.y >= ORDONNEE_COIN_DIM8) && (positionClick.y <= ORDONNEE_COIN_DIM8 + HAUTEUR_CASE_DIM_2 ))
		dim = 0;
	}while(dim != 5 && dim != 7 && dim != 8 && dim != 9 && dim != 10 && dim != 11 && dim != 13 && dim != 0);

	SDL_FreeSurface(image);
	return dim;
}


/*!
 *	\author	Alexis Brisset
 *	\param	fond Fond à afficher derrière le menu
 *	\param 	ecran Pointeur sur la surface d'affichage
 *	\return	Couleur choisie par l'utilisateur
 *
 *	Affiche les couleurs de pion disponibles, et interroge l'utilisateur sur son choix.
 */
char choix_couleur(SDL_Surface *fond, SDL_Surface *ecran)
{
	char couleur = 'V';
	SDL_Surface *image = NULL;

	SDL_Rect positionImage;
	positionImage.x=0;
	positionImage.y = 0;

	image = SDL_LoadBMP("images/menus/menu_couleur.bmp");

	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 0, 0));

	SDL_BlitSurface(fond, NULL, ecran, &positionImage);
	SDL_BlitSurface(image, NULL, ecran, &positionImage);
	SDL_Flip(ecran);

	coord positionClick;
	positionClick.x = 0;
	positionClick.y = 0;
	do
	{
		positionClick = get_click();

		if ((positionClick.x >= ABSCISSE_COIN_COULEUR) && (positionClick.x <= ABSCISSE_COIN_COULEUR + LARGEUR_MENU_COULEUR) && (positionClick.y >= ORDONNEE_COIN_COULEUR) && (positionClick.y <= ORDONNEE_COIN_COULEUR + HAUTEUR_MENU_COULEUR ))
		couleur = 'B'; // renvoie le choix du click sur "Noir"

		if ((positionClick.x >= ABSCISSE_COIN_COULEUR2) && (positionClick.x <= ABSCISSE_COIN_COULEUR2 + LARGEUR_MENU_COULEUR2) && (positionClick.y >= ORDONNEE_COIN_COULEUR2) && (positionClick.y <= ORDONNEE_COIN_COULEUR2 + HAUTEUR_MENU_COULEUR2 ))
		couleur = 'N'; // renvoie le choix du click sur "Blanc"

		if ((positionClick.x >= ABSCISSE_COIN_COULEUR3) && (positionClick.x <= ABSCISSE_COIN_COULEUR3 + LARGEUR_MENU_COULEUR3) && (positionClick.y >= ORDONNEE_COIN_COULEUR3) && (positionClick.y <= ORDONNEE_COIN_COULEUR3 + HAUTEUR_MENU_COULEUR3 ))
		couleur = 'R'; // renvoie le choix du click sur "Retour"
	}while(couleur != 'N' && couleur != 'B' && couleur  !='R');

	SDL_FreeSurface(image);
	return couleur;
}

/*!
 *	\author	Alexis Brisset
 *	\param	fond Fond à afficher derrière le menu
 *	\param 	ecran Pointeur sur la surface d'affichage
 *	\return	Niveau d'IA choisi par l'utilisateur
 *
 *	Affiche les niveaux d'IA disponibles, et interroge l'utilisateur sur son choix
 */
int choix_level(SDL_Surface *fond, SDL_Surface *ecran)
{
	int choix = -1;
	SDL_Surface *image = NULL;
	SDL_Rect positionImage;

	positionImage.x=0;
	positionImage.y = 0;

	image = SDL_LoadBMP("images/menus/menu_difficulte.bmp");

	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 0, 0));

	SDL_BlitSurface(fond, NULL, ecran, &positionImage);
	SDL_BlitSurface(image, NULL, ecran, &positionImage);

	SDL_Flip(ecran);

	coord positionClick;
	positionClick.x = 0;
	positionClick.y = 0;
	do
	{
			positionClick = get_click();

			if ((positionClick.x >= ABSCISSE_COIN_DIFFICULTE) && (positionClick.x <= ABSCISSE_COIN_DIFFICULTE + LARGEUR_MENU_DIFFICULTE) && (positionClick.y >= ORDONNEE_COIN_DIFFICULTE) && (positionClick.y <= ORDONNEE_COIN_DIFFICULTE + HAUTEUR_MENU_DIFFICULTE ))
			choix = 1; // renvoie le choix du click sur "Facile"

			if ((positionClick.x >= ABSCISSE_COIN_DIFFICULTE2) && (positionClick.x <= ABSCISSE_COIN_DIFFICULTE2 + LARGEUR_MENU_DIFFICULTE2) && (positionClick.y >= ORDONNEE_COIN_DIFFICULTE2) && (positionClick.y <= ORDONNEE_COIN_DIFFICULTE2 + HAUTEUR_MENU_DIFFICULTE2 ))
			choix = 2; // renvoie le choix du click sur "Nash"

			if ((positionClick.x >= ABSCISSE_COIN_DIFFICULTE3) && (positionClick.x <= ABSCISSE_COIN_DIFFICULTE3 + LARGEUR_MENU_DIFFICULTE3) && (positionClick.y >= ORDONNEE_COIN_DIFFICULTE3) && (positionClick.y <= ORDONNEE_COIN_DIFFICULTE3 + HAUTEUR_MENU_DIFFICULTE3 ))
			choix = 3; // renvoie le choix du click sur "moyen"

			if ((positionClick.x >= ABSCISSE_COIN_DIFFICULTE4) && (positionClick.x <= ABSCISSE_COIN_DIFFICULTE4 + LARGEUR_MENU_DIFFICULTE4) && (positionClick.y >= ORDONNEE_COIN_DIFFICULTE4) && (positionClick.y <= ORDONNEE_COIN_DIFFICULTE4 + HAUTEUR_MENU_DIFFICULTE4 ))
			choix = 4; // renvoie le choix du click sur "Fiddicile"

			if ((positionClick.x >= ABSCISSE_COIN_DIFFICULTE5) && (positionClick.x <= ABSCISSE_COIN_DIFFICULTE5 + LARGEUR_MENU_DIFFICULTE5) && (positionClick.y >= ORDONNEE_COIN_DIFFICULTE5) && (positionClick.y <= ORDONNEE_COIN_DIFFICULTE5 + HAUTEUR_MENU_DIFFICULTE5 ))
			choix = 5; // renvoie le choix du click sur "Fiddicile"

			if ((positionClick.x >= ABSCISSE_COIN_DIFFICULTE5) && (positionClick.x <= ABSCISSE_COIN_DIFFICULTE6 + LARGEUR_MENU_DIFFICULTE6) && (positionClick.y >= ORDONNEE_COIN_DIFFICULTE6) && (positionClick.y <= ORDONNEE_COIN_DIFFICULTE6 + HAUTEUR_MENU_DIFFICULTE6 ))
			choix = 0; // renvoie le choix du click sur "Retour"
	}while(choix == -1);

	SDL_FreeSurface(image);
	return choix;
}

/*!
 *	\author	Alexis Brisset
 *	\param	fond Fond à afficher derrière le menu
 *	\param 	ecran Pointeur sur la surface d'affichage
 *	\return	Type d'Intelligence Artificielle choisi
 *
 *	Affiche les types (algorithmes) d'IA disponibles, et interroge l'utilisateur sur son choix
 */
int choix_ia(SDL_Surface *fond, SDL_Surface *ecran)
{
	int ia = -1;
	SDL_Surface *image = NULL;

	SDL_Rect positionImage;
	positionImage.x=0;
	positionImage.y = 0;

	image = SDL_LoadBMP("images/menus/menu_ia.bmp");

	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 0, 0));

	SDL_BlitSurface(fond, NULL, ecran, &positionImage);
	SDL_BlitSurface(image, NULL, ecran, &positionImage);

	SDL_Flip(ecran);

	coord positionClick;
	positionClick.x = 0;
	positionClick.y = 0;

	do
	{
			positionClick = get_click();

			if ((positionClick.x >= ABSCISSE_COIN_IA) && (positionClick.x <= ABSCISSE_COIN_IA + LARGEUR_MENU_IA) && (positionClick.y >= ORDONNEE_COIN_IA) && (positionClick.y <= ORDONNEE_COIN_IA + HAUTEUR_MENU_IA ))
			ia = 1; // renvoie le choix du click sur "Brainless"

			if ((positionClick.x >= ABSCISSE_COIN_IA2) && (positionClick.x <= ABSCISSE_COIN_IA2 + LARGEUR_MENU_IA2) && (positionClick.y >= ORDONNEE_COIN_IA2) && (positionClick.y <= ORDONNEE_COIN_IA2 + HAUTEUR_MENU_IA2 ))
			ia = 2; // renvoie le choix du click sur "Losanges"

			// IA indisponible pour le moment:
			//if ((positionClick.x >= ABSCISSE_COIN_IA3) && (positionClick.x <= ABSCISSE_COIN_IA3 + LARGEUR_MENU_IA3) && (positionClick.y >= ORDONNEE_COIN_IA3) && (positionClick.y <= ORDONNEE_COIN_IA3 + HAUTEUR_MENU_IA3 ))
			//ia = 3; // renvoie le choix du click sur "Electrique"

			if ((positionClick.x >= ABSCISSE_COIN_IA4) && (positionClick.x <= ABSCISSE_COIN_IA4 + LARGEUR_MENU_IA4) && (positionClick.y >= ORDONNEE_COIN_IA4) && (positionClick.y <= ORDONNEE_COIN_IA4 + HAUTEUR_MENU_IA4))
			ia = 0; // renvoie le choix du click sur "Retour"

	}while(ia == -1);

	SDL_FreeSurface(image);
	return ia;
}

