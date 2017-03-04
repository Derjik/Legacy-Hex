/*!
 *	\file	data_storage.c
 *	\brief	Fonctions du module d'accès à la mémoire morte
 *	\author	Julien Laurent
 *
 *	Fonctions de sauvegarde et chargement de partie, utilisant l'accès aux fichiers pour stocker l'état du plateau,
 *	la nature des joueurs, le joueur courant et la difficulté de l'intelligence artificielle.
 *	Le fichier généré (save.hex) est une sauvegarde unique.
 */

#include "data_storage.h"

/*!
 *	\author	Julien Laurent
 *	\param	p Pointeur sur le plateau à sauver
 *	\param	J1 Pointeur sur le premier joueur à sauver
 *	\param	J2 Pointeur sur le deuxième joueur à sauver
 *	\param	difficulte Difficulté de l'intelligence artificielle
 *	\param	numero_tour Numéro du tour courant
 *	\param	current Pointeur sur le joueur qui devra jouer lors du chargement
 *	\return	0 en cas de succès à la sauvegarde, -1 sinon
 *
 *	Fonction de sauvegarde de partie qui écrit sur un fichier le contenu pointé par les pointeurs
 *	passés en paramètre.
 *	Cette fonction écrase ou crée le fichier save.hex, et y écrit séquentiellement toutes les données de jeu
 *	dans un ordre précis, de manière à ce que #charge_partie() puisse les retrouver correctement.
 *	En cas d'échec à l'ouverture du fichier en écriture, un code d'erreur est renvoyé, et la fonction appelante
 *	peut l'interpréter et afficher un message d'erreur.
 */
int sauve_partie(plateau *p, joueur *J1, joueur *J2, int difficulte, int numero_tour, joueur *current)
{
	// Tentative d'ouverture du fichier destiné à la sauvegarde
	FILE *container = fopen("save.hex","w+");

	int i,j, type_j1=0, type_j2=0;

	if(container == NULL) // Si l'ouverture a échoué, on renvoie un code d'erreur
	{
		return -1;
	}
	else // Sinon, on procède à l'écriture du fichier
	{
		/*
		 *	Séquence d'écriture:
		 *	-Dimension du plateau
		 *	-Contenu du plateau
		 *	-Type du joueur 1
		 *	-Type du joueur 2
		 *	-Couleur du joueur 1
		 *	-Couleur du joueur 2
		 *	-Couleur du joueur dont c'était le tour au moment de la sauvegarde
		 *	-Numéro du tour au moment de la sauvegarde
		 *	-Difficulté de l'IA
		 */

		fwrite (&(p->dim), sizeof(int), 1, container); // Écriture de de la dimension du plateau

		for ( i = 0 ; i < p->dim ; i++ ) // Écriture du contenu du plateau
		{
			for ( j = 0 ; j < p->dim ; j++ )
			{
				fwrite(&(p->tab[i][j]), sizeof(char), 1, container);
			}
		}

		// Détection du type du joueur 1
		type_j1 = type_joueur(J1);

		// Détection du type du joueur 2
		type_j2 = type_joueur(J2);


		fwrite(&(type_j1), sizeof(int), 1, container); // Écriture du type du joueur 1
		fwrite(&(type_j2), sizeof(int), 1, container); // Écriture du type du joueur 2

		fwrite(&(J1->pion), sizeof(char), 1, container); // Écriture de la couleur du joueur 1
		fwrite(&(J2->pion), sizeof(char), 1, container); // Écriture de la couleur du joueur 1

		fwrite(&(current->pion), sizeof(char), 1, container); // Écriture de la couleur du joueur courant

		fwrite(&numero_tour, sizeof(int), 1, container); // Écriture du numéro du tour courant
		fwrite(&difficulte, sizeof(int), 1, container); // Écriture de la difficulté


		fclose(container); // Fermeture du fichier de sauvegarde

		return 0;
	}
}


/*!
 *	\author	Julien Laurent
 *
 *	\param	p Adresse du pointeur à utiliser pour stocker le plateau chargé
 *	\param	J1 Adresse du pointeur sur le premier joueur à charger
 *	\param	J2 Adresse du pointeur sur le deuxième joueur à charger
 *	\param	difficulte Pointeur sur la difficulté à charger
 *	\param	numero_tour Pointeur sur le numéro de tour à charger
 *	\param	current Adresse du pointeur sur le joueur courant à charger
 *	\return	0 en cas de succès au chargement, -1 sinon
 *
 *	Ouvre le fichier save.hex en lecture (s'il existe), et charge les données de jeu dans la fonction appelante
 *	par le biais des pointeurs dont les adresses sont passées en paramètre.
 *	En cas d'échec à l'ouverture du fichier, un code d'erreur est renvoyé, et la fonction appelante peut l'interpréter pour afficher
 *	le message d'erreur adapté.
 */
int charge_partie(plateau **p, joueur **J1, joueur **J2, int *difficulte, int *numero_tour, joueur **current)
{
	// Tentative d'ouverture du fichier censé contenir la sauvegarde
	FILE *container = fopen("save.hex","r");

	int i,j, type_j1, type_j2, taille;
	char pion_j1, pion_j2, pion_courant;

	if(container == NULL) // Si l'ouverture a échoué, on renvoie un code d'erreur
	{
		return -1;
	}
	else
	{
		// Pour éviter les fuites de mémoire, on détruit les structures existantes,
		// si elles ont été créées précédemment (dans les menus par exemple)
		detruis_plateau(p);
		detruis_joueur(J1);
		detruis_joueur(J2);

		/*
		 *	Séquence de lecture:
		 *	-Dimension du plateau
		 *	-Contenu du plateau
		 *	-Type du joueur 1
		 *	-Type du joueur 2
		 *	-Couleur du joueur 1
		 *	-Couleur du joueur 2
		 *	-Couleur du joueur dont c'était le tour au moment de la sauvegarde
		 *	-Numéro du tour au moment de la sauvegarde
		 *	-Difficulté de l'IA
		 */

		fread(&taille, sizeof(int), 1, container); // Lecture de la dimension du plateau

		*p = nouveau_plateau(taille); // Création du plateau avec la dimension lue

		for ( i = 0 ; i < (*p)->dim ; i++ ) // Lecture du contenu du plateau
		{
			for ( j = 0 ; j < (*p)->dim ; j++ )
			{
				fread(&((*p)->tab[i][j]), sizeof(char), 1, container);
			}
		}

		fread(&type_j1, sizeof(int), 1, container); // Lecture du type du joueur 1
		fread(&type_j2, sizeof(int), 1, container); // Lecture du type du joueur 2

		fread(&pion_j1, sizeof(char), 1, container); // Lecture de la couleur du joueur 1
		fread(&pion_j2, sizeof(char), 1, container); // Lecture de la couleur du joueur 2


		*J1 = nouveau_joueur(pion_j1, type_j1); // Création du joueur 1 avec les paramètres précédemment lus
		*J2 = nouveau_joueur(pion_j2, type_j2); // Création du joueur 2 avec les paramètres précédemment lus

		fread( &pion_courant, sizeof(char), 1, container); // Lecture de la couleur du pion courant

		if(pion_courant == (*J1)->pion) // Mise à jour du pointeur sur le joueur courant
		{
			*current = *J1;
		}
		else
		{
			*current = *J2;
		}

		fread(numero_tour, sizeof(int), 1, container); // Lecture du numéro de tour
		fread(difficulte, sizeof(int), 1, container); // Lecture de la difficulté d'IA


		fclose(container); // Fermeture du fichier de sauvegarde
		return 0;
	}

}
