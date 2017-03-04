/*!
 *	\file	version.h
 *	\brief	Fichier d'information sur la version du logiciel
 *	\author	C::B
 *
 *	Fichier géré automatiquement par l'IDE, contenant les différents descripteurs de version du logiciel.
 */

/*!
 *	\dir	src
 *	\brief	Racine des sources
 *
 *	L'ensemble des sources du logiciel sont contenues dans ce répertoire, ainsi que ses
 *	répertories fils (classés par "modules" du logiciel)
 */

/*!
 *	\dir	model
 *	\brief	Noyau du logiciel (modèle de données)
 *
 *	Ce répertoire contient tous les fichiers qui décrivent et définissent le modèle de données du
 *	logiciel, la logique du jeu, etc.
 */

/*!
 *	\dir	engine
 *	\brief	Modules constituant le moteur de jeu
 *
 *	Ce répertoire contient les fichiers du moteur (pour l'instanciation et la destruction d'entités
 *	du modèle de données, la gestion des parties, la sauvegarde et le chargement, etc.)
 */

/*!
 *	\dir	ai
 *	\brief	Modules constituant l'intelligence artificielle du jeu
 *
 *	Ce répertoire contient les fichiers de l'intelligence artificielle du jeu, comprenant les fonctions
 *	de construction d'arbres, ou encore les fonctions d'évaluation de plateaux.
 */

/*!
 *	\dir	interfaces
 *	\brief	Modules constituant l'interface homme-machine du jeu
 *
 *	Ce répertoire contient les fichiers de l'interface utilisateur du jeu.
 */

/*!
 *	\file	main.c
 *	\brief	Fichier décrivant le déroulement complet du programme.
 *	\author	Julien Laurent, Alexis Brisset, Lucas Dessaignes
 *
 *	Ce fichier contient l'ensemble des appels aux fonctions essentielles
 *	du programme: il régit la configuration d'une nouvelle partie, la
 *	sauvegarde ou encore le chargement.
 *	Les initialisations primordiales s'y déroulent. (SDL, tables de condensats,
 *	réglages de partie...)
 */

#include "version.h"
#include "engine/engine_functions.h"
#include "engine/data_storage.h"

/// Fonction de benchmarking pour le système de mise en cache des plateaux (compte et affiche le nombre de collisions de la fonction de hachage)
void collisionTestBed(int dimension, int iterations);


/*!
 *	\brief	Fonction principale du logiciel
 *	\author	Julien Laurent, Alexis Brisset, Lucas Dessaignes
 *	\param	argc (Inutilisé)
 *	\param	argv (Inutilisé)
 *	\return	Code d'erreur, utilisable à des fins de débogage
 *
 *	Cette fonction gère la configuration des parties, la sauvegarde et le chargement, ainsi que le
 *	lancement. Elle appelle les différents menus pour aiguiller l'utilisateur, puis lance les parties
 *	à l'aide de lance_partie()
 */

int main(int argc, char *argv[])
{
	(void)argc; // Statement inutile, pour éclipser les avertissements
	(void)argv; // concernant les paramètres de fonction inusités

	srand(time(NULL)); // Initialisation du générateur de nombres pseudo-aléatoires

	tableNoire = NULL; // Mise à NULL des pointeurs vers les tables de condensats
	tableBlanche = NULL;

	SDL_Surface *ecran = initFenetre(); // Initialisation de la fenêtre et préparation du pointeur associé
	SDL_Surface *fond = NULL; // Pointeur sur la surface servant de "fond d'écran" (décor)


	// Ces variables sont destinées à contenir les choix de l'utilisateur au fil des menus
	int choix_principal = -1;
	int dimension_choisie = -1;
	int difficulte_choisie = -1;
	int type_de_jeu_choisi = -1;
	int ia_choisie = -1;
	char couleur_joueur_humain = 'V';

	/*
	 * Ces pointeurs accueilleront les adresses des éléments "fondamentaux" pour le
	 * déroulement d'une partie: les joueurs, le plateau, et le joueur courant.
	 */
	plateau *jeu = NULL; // Le plateau de jeu
	joueur *J1 = NULL; // Le joueur 1
	joueur *J2 = NULL; // Le joueur 2
	joueur *joueur_courant = NULL; // Le "joueur courant"
	int *numero_tour = malloc(sizeof(int)); // Le nombre de tours déjà passés

	// En mode de debug, des informations sur la version sont données (à l'aide des constantes
	// mises à jour par le plugin d'autoversionning de Code::Blocks)
	printf("Hexecution - Version %ld.%ld.%ldrev%ld\n", MAJOR, MINOR, BUILD, REVISION);
	printf("Concu et developpe par: Alexis Brisset, Lucas Dessaignes & Julien Laurent\n");

	// L'instruction qui suit effectue un test de collisions au démarrage
	// (à commenter pour les versions de production)
	//collisionTestBed(5,1000);

	// On crée un curseur d'étapes, et on l'initialise au premier menu:
	etape_menu etape = MENU_PRINCIPAL;


	do
	{
		switch(etape) // Selon l'étape à atteindre:
		{
			case MENU_PRINCIPAL: // Menu principal

				// On libère le fond s'il existe,
				if(fond!=NULL) SDL_FreeSurface(fond);
				// puis on en charge un nouveau
				fond = initFond();

				// On détruit les éventuels joueurs encore présents en mémoire,
				// et on remet à NULL le pointeur sur le joueur courant
				detruis_joueur(&J1);
				detruis_joueur(&J2);
				detruis_plateau(&jeu);
				joueur_courant = NULL;

				// On réinitialise les tables de condensats
				detruisTables();
				initTables();

				// Interrogation de l'utilisateur sur son choix principal
				switch(choix_principal = choix_menu(fond, ecran))
				{
					case 0: // Quitter
						etape = QUITTER; // On quitte le jeu (sortie de la boucle do{}while();)
					break;

					case 1: // Nouveau jeu
						etape = CHOIX_DIMENSION; // On passe à l'étape de choix de la dimension
					break;

					case 2: // Charger jeu
						etape = CHARGEMENT_JEU; // On passe à l'étape de chargement des paramètres
					break;
				}
			break;

			case CHOIX_DIMENSION: // Choix de la dimension
				if((dimension_choisie = choix_dimension(fond, ecran)) == 0) // Si le signal de retour est renvoyé
				{
					etape = MENU_PRINCIPAL; // On repasse à l'étape précédente
				}
				else if(dimension_choisie <= 7) // Si la dimension est acceptable pour une IA:
				{
					detruis_plateau(&jeu); // On détruit l'éventuel plateau précédemment créé (en cas de retour par exemple)
					jeu = nouveau_plateau(dimension_choisie); // On en crée un nouveau, adapté au dernier choix effectué
					etape = CHOIX_TYPE_DE_JEU; // On passe à l'étape suivante
				}
				else // Si la dimension est trop grande pour laisser une IA jouer:
				{
					detruis_plateau(&jeu); // On détruit les éventuels plateaux et joueurs précédemment construits
					detruis_joueur(&J1);
					detruis_joueur(&J2);
					jeu = nouveau_plateau(dimension_choisie); // On crée le nouveau plateau, adapté au dernier choix effectué
					J1 = nouveau_joueur('N', 0); // Puis on configure une partie de type "humain contre humain",
					J2 = nouveau_joueur('B', 0); // car la dimension est trop élevée pour faire intervenir l'IA
					etape = LANCEMENT_PARTIE; // On lance la partie nouvellement configurée
				}
			break;

			case CHOIX_TYPE_DE_JEU: // Choix du type de partie:
				switch(type_de_jeu_choisi = choix_type(fond, ecran))
				{
					case 0: // Retour
						etape = CHOIX_DIMENSION;
					break;

					case 1: // Humain vs Humain
						detruis_joueur(&J1);
						detruis_joueur(&J2);
						J1 = nouveau_joueur('N', 0);
						J2 = nouveau_joueur('B', 0);
						etape = LANCEMENT_PARTIE;
					break;

					case 2: // Humain vs Machine
						etape = CHOIX_COULEUR;
					break;

					case 3: // Machine vs Machine
						etape = CHOIX_TYPE_IA;
					break;
				}
			break;

			case CHOIX_COULEUR: // Choix de la couleur du joueur humain
				if((couleur_joueur_humain = choix_couleur(fond, ecran)) == 'R')
				{
					etape = CHOIX_TYPE_DE_JEU; // Retour
				}
				else
				{
					detruis_joueur(&J1);
					J1 = nouveau_joueur(couleur_joueur_humain, 0);
					etape = CHOIX_TYPE_IA; // Passage à l'étape suivante
				}
			break;

			case CHOIX_TYPE_IA: // Choix du "type" d'intelligence artificielle à affronter

				// Dans le cas d'un match Humain vs Machine:
				if(type_de_jeu_choisi == 2)
				{
					switch(ia_choisie = choix_ia(fond, ecran))
					{
						case 0: // Retour
							etape = CHOIX_COULEUR;
						break;

						case 1: // Brainless
							detruis_joueur(&J2);
							J2 = nouveau_joueur(couleur_opposee(couleur_joueur_humain), ia_choisie);
							etape = LANCEMENT_PARTIE; // L'IA Brainless n'a aucun horizon, la difficulté ne peut donc pas
						break;						// être réglée.

						case 2: // Losanges
							detruis_joueur(&J2);
							J2 = nouveau_joueur(couleur_opposee(couleur_joueur_humain), ia_choisie);
							etape = CHOIX_NIVEAU_IA;
						break;
					}
				}
				else // Sinon (Machine vs Machine):
				{
					switch(ia_choisie = choix_ia(fond, ecran))
					{
						case 0: // Retour
							etape = CHOIX_COULEUR;
						break;

						case 1: // Brainless
							detruis_joueur(&J1);
							detruis_joueur(&J2);
							J1 = nouveau_joueur('N', ia_choisie);
							J2 = nouveau_joueur('B', ia_choisie);

							etape = LANCEMENT_PARTIE;
						break;

						case 2: // Losange
							detruis_joueur(&J1);
							detruis_joueur(&J2);
							J1 = nouveau_joueur('N', ia_choisie);
							J2 = nouveau_joueur('B', ia_choisie);

							etape = CHOIX_NIVEAU_IA;
						break;
					}
				}
			break;

			case CHOIX_NIVEAU_IA:
				switch(difficulte_choisie = choix_level(fond, ecran))
				{
					case 0: // Retour
						etape = CHOIX_TYPE_IA;
					break;

					case 1: // Facile
						difficulte_choisie = 5;
						etape = LANCEMENT_PARTIE;
					break;

					case 2: // Moyen
						difficulte_choisie = 6;
						etape = LANCEMENT_PARTIE;
					break;

					case 3: // Difficile
						difficulte_choisie = 7;
						etape = LANCEMENT_PARTIE;
					break;

					case 4: // Nash
						difficulte_choisie = 8;
						etape = LANCEMENT_PARTIE;
					break;

					case 5: // Nash 2 en 1
						difficulte_choisie = 9;
						etape = LANCEMENT_PARTIE;
					break;

				}
			break;

			case CHARGEMENT_JEU:
				if(!charge_partie(&jeu, &J1, &J2, &difficulte_choisie, numero_tour, &joueur_courant))
				{	// Si la partie s'est chargée correctement, on la lance:
					etape = LANCEMENT_PARTIE;
				}
				else // Cas d'erreur
				{	// On revient au menu principal
					etape = MENU_PRINCIPAL;
				}
			break;

			case LANCEMENT_PARTIE:
				// S'il s'agit d'une partie fraichement configurée, on
				// initialise bien proprement le pointeur sur le joueur courant
				if(choix_principal == 1)
				{
					*numero_tour = 0;
					if(J1->pion=='N')
					{
						joueur_courant = J1;
					}
					else
					{
						joueur_courant = J2;
					}
				}

				// Dans tous les cas, on lance la partie avec les paramètres actuels
				switch(lance_partie(jeu, J1, J2, difficulte_choisie, numero_tour, &joueur_courant, fond, ecran))
				{
					case 'N': // Noir a gagné
						etape = MENU_PRINCIPAL;
					break;

					case 'B': // Blanc a gagné
						etape = MENU_PRINCIPAL;
					break;

					case 'S': // Signal de sauvegarde
						if(sauve_partie(jeu,J1,J2,difficulte_choisie,*numero_tour,joueur_courant)==0)
						{
							etape = MENU_PRINCIPAL;
						}
						else
						{
							// Si la sauvegarde échoue, on reste dans le jeu (le joueur
							// peut tout de même quitter via la touche Echap)
						}
					break;

					case 'V': // Partie mal construite

					break;
				}
			break;

			case QUITTER:
				// On détruit tout avant de quitter:

				free(numero_tour);

				detruis_joueur(&J1);
				detruis_joueur(&J2);
				detruis_plateau(&jeu);

				SDL_FreeSurface(fond);
				SDL_FreeSurface(ecran);

				detruisTables();

				SDL_Quit();
			break;
		}
	}while(etape != QUITTER);


	return 0;
}


/*!
 *	\author	Julien Laurent
 *	\param	dimension Dimension du plateau à créer pour les tests
 *	\param	iterations Nombre de configurations aléatoires à générer et tester
 *
 *	Cette fonction crée des configurations aléatoires (et pas forcément valides)
 *	de plateaux, et teste la fonction de mise en cache de l'IA pour vérifier
 *	si des collisions se présentent (entre des plateaux non-équivalents)
 */
void collisionTestBed(int dimension, int iterations)
{
	if(iterations<=0) return;

	int i,j, valeurN,valeurB, collisions=0;
	plateau *p;

	p = nouveau_plateau(dimension);

	detruisTables();
	initTables();

	do
	{
		for ( i = 0 ; i < p->dim ; i++ )
		{
			for ( j = 0 ; j < p->dim ; j++ )
			{
				switch(hasard(0,2))
				{
					case 0:
						p->tab[i][j]='V';
					break;

					case 1:
						p->tab[i][j]='N';
					break;

					case 2:
						p->tab[i][j]='B';
					break;
				}
			}
		}
		valeurN = hashLosanges(p, 'N');
		valeurB = hashLosanges(p, 'B');
		if(valeurN != eval_losanges(p,'N')) collisions++;
		if(valeurB != eval_losanges(p,'B')) collisions++;

		iterations--;
	}while(iterations>0);

	detruisTables();

	printf("Nombre de collisions: %d\n", collisions);

	return;
}

/*!
 *	\mainpage	Accueil
 *
 *	\tableofcontents
 *
 *	\section	intro Introduction
 *	Ce logiciel, dont la conception a commencé en Janvier 2013, est une implémentation du
 *	célèbre jeu de plateau nommé Hex, initialement inventé par le danois
 *	<a href="http://fr.wikipedia.org/wiki/Piet_Hein_(physicien)">Piet Hein</a>, puis
 *	réinventé par l'américain <a href="http://fr.wikipedia.org/wiki/John_Forbes_Nash">
 *	John Forbes Nash</a>.\n\n
 *	Faisant intervenir des \ref regles "règles" très simples, ce jeu bénéficie toutefois
 *	d'une variété de stratégies possibles et d'une complexité très intéressantes pour une
 *	implémentation logicielle.\n
 *	Notamment, l'implémentation d'un joueur artificiel fait intervenir des théories
 *	mathématiques poussées, dont principalement la théorie des jeux, ainsi que dans
 *	une autre mesure, la théorie des graphes.
 *
 *	\section	regles Règles du jeu
 *	Les règles du jeu sont relativement simples:\n
 *		- Le jeu se joue à deux joueurs (un joueur noir, et un joueur blanc), sur un plateau
 *		de forme losange, constitué de cellules hexagonales.
 *		- Chaque joueur doit, lorsque vient son tour, poser un pion de sa couleur sur la
 *		cellule libre de son choix.
 *		- Le premier joueur à placer son pion au cours d'une partie est toujours le joueur noir.
 *		.
 *
 *	\section	modules Découpage modulaire du logiciel
 *	\image	html decoupage_modulaire.png
 *	Ce schéma décrit la façon générale dont le logiciel a été découpé durant
 *	la phase d'évaluation du problème. Ce même schéma est resté notre base
 *	organisationnelle durant tout le cycle de développement.\n
 *
 *	\subsection	modele Modèle de données
 *	Ce module représente le "coeur" du logiciel: en effet, il définit précisément
 *	la manière dont les différentes données de jeu (plateau, joueurs, logique de jeu...)
 *	seront représentées en mémoires, et gérées.\n
 *	Tout changement critique dans le modèle impacterait l'ensemble du logiciel,
 *	c'est pourquoi il a été défini après de nombreuses séances de réflexion et de
 *	"mise à l'épreuve" par les membres de l'équipe.
 *
 *	\subsection	moteur Moteur de jeu
 *	Ce module régit le déroulement des parties, l'instanciation des nouveaux joueurs
 *	et plateaux, ou encore la sauvegarde et le chargement depuis une mémoire morte.\n
 *	Il est donc découpé en un sous-module de gestion des parties, joueurs et plateaux,
 *	et un sous-module d'accès à la mémoire morte.
 *
 *	\subsection	interface Interface graphique
 *	Ce module est l'unique Interface Homme-Machine proposée à ce jour dans la version
 *	stable du logiciel, l'interface console ayant finalement été retirée du projet
 *	(mais conservée dans des fichiers distincts).\n
 *	Il contient toutes les fonctions de menus de configuration, ainsi que l'interface
 *	de jeu à proprement parler (affichage de l'état du jeu, acquisition des coups joués
 *	par des joueurs humains).\n
 *	Il est donc découpé en un sous-module "central" de routines SDL, un sous-module
 *	dédié à l'affichage et la gestion des menus, et un sous-module dédié à l'interface
 *	utilisateur durant les parties.
 *
 *	\subsection	ia Intelligence artificielle
 *	Ce module régit les méthodes d'analyse et de jeu des différents "joueurs virtuels"
 *	proposés dans le logiciel, destinés à affronter un joueur humain, ou encore à
 *	s'affronter entre eux.\n
 *	Il est découpé en un sous-module de fonctions d'évaluation de plateaux, un sous-module
 *	de fonctions de mise en cache des plateaux évalués, et un sous-module de construction
 *	d'arbres de jeu à base d'appels récursifs (algorithmes MinMax et AlphaBeta).
 *
 *
 *	\section	credits Crédits de développement
 *
 *	\subsection	remerciements Remerciements
 *	Le projet fut lancé, encadré et noté par <a href="http://fm.colonna.free.fr/">M. François-Marie Colonna</a>,
 *	enseignant-chercheur à l'ISEN Toulon, professeur d'Algorithmique et Bases de Données
 *	en deuxième année de Cycle Informatique et Réseaux.\n
 *	Nos remerciements lui reviennent donc, ainsi qu'à tout le corps enseignant de C.I.R 2
 *	impliqué dans le projet, et nous ayant aidé à concevoir et implémenter le jeu tel qu'il
 *	est aujourd'hui présenté.
 *
 *	\subsection	equipe Équipe de conception/développement
 *	L'équipe de conception et de développement comprend:
 *		- <a href="mailto:julien.laurent@isen.fr">Julien Laurent</a>, chef de projet, concepteur et développeur
 *		- <a href="mailto:alexis.brisset@isen.fr">Alexis Brisset</a>, concepteur et développeur
 *		- <a href="mailto:lucas.dessaignes@isen.fr">Lucas Dessaignes</a>, concepteur et développeur
 *		.
 *
 *	\subsection	repartition Répartition détaillée des tâches
 *
 *	__Modélisation__\n
 *		La modélisation du problème fut réalisée par les trois membres de l'équipe.\n
 *		Le choix des structures de données représentant le plateau et les joueurs
 *		en mémoire fut effectué en commun, après établissement des besoins
 *		de ces structures pour répondre aux différentes problématiques (détection des
 *		fins de partie, exploitation par l'intelligence artificielle...)
 *
 *	__Moteur de jeu__\n
 *		La conception et l'implémentation du moteur du jeu sont l'œuvre des trois membres de l'équipe.\n
 *		La fonction de vérification de gain, qui fait partie des plus importants composants du
 *		moteur, a en effet demandé une réflexion commune, puis un découpage en sous-fonctions et une
 *		répartition des tâches pour implémenter ces différentes sous-fonctions.\n
 *		L'optimisation et le débogage de ces fonctions sont aussi l'œuvre des trois membres de l'équipe.
 *
 *	__Interface graphique__\n
 *		La conception et l'implémentation des fonctions d'affichage de l'interface
 *		graphique sont l'œuvre de Lucas Dessaignes, ainsi que le design des menus et plateaux.\n
 *		La conception et l'implémentation des fonctions de saisie utilisateur sont l'œuvre d'Alexis Brisset,
 *		ainsi que le calibrage des différentes zones de clic.
 *
 *	__Intelligence artificielle__\n
 *		La conception et l'implémentation des fonctions d'évaluation de plateaux sont l'œuvre des
 *		trois membres de l'équipe.\n
 *		La conception et l'implémentation des fonctions des fonctions de construction d'arbres de
 *		l'intelligence artificielle sont l'œuvre de Julien Laurent
 *
 */


/*!
 *	\page	userman Manuel d'utilisation
 *
 *	\tableofcontents
 *
 *	Cette page contient le manuel de référence pour l'utilisateur final du logiciel.
 *	Ce manuel est réparti dans les sous-sections suivantes: \ref config et \ref jeu.
 *	\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n
 *
 *	\section	config Menus et configuration d'une partie
 *	Cette section décrit les différentes étapes de configuration d'une partie, au travers
 *	des menus de jeu.
 *	\subsection	mainmenu Écran d'accueil
 *	\image	html menu_principal.png
 *	Au lancement, le menu principal du jeu s'affiche, avec les boutons qui suivent:
 *		- \ref nouvelle_partie "Nouvelle partie"
 *		- \ref charger_partie "Charger partie"
 *		- \ref quitter "Quitter"
 *
 *	\subsection nouvelle_partie Configuration d'une nouvelle partie
 *	Lors du clic sur ce bouton, le \ref choix_dimension "menu de choix de la dimension"
 *	du plateau est appelé, et une nouvelle partie est sur le point d'être configurée.
 *
 *	\subsection	charger_partie Chargement d'une partie existante
 *	Si une partie \ref nouvelle_partie "précédemment configurée" et lancée a été
 *	\ref sauvegarde "sauvegardée", un clic sur le bouton "Charger partie" du
 *	\ref mainmenu "menu principal" provoque son chargement
 *	en mémoire et son lancement, dans l'état exact dans lequel elle était au moment
 *	de la sauvegarde.\n
 *	Si aucune sauvegarde n'est disponible, ou si le chargement échoue (fichier
 *	save.hex inaccessible, par exemple), un retour au \ref mainmenu "menu principal"
 *	est immédiatement opéré.
 *
 *	\subsection quitter Fermeture du jeu
 *	Un clic sur le bouton "Quitter" du menu principal provoque la fermeture du jeu.\n
 *	A tout moment, un clic sur la croix de fermeture de la fenêtre provoque le même effet.\n
 *	Durant une partie, un appui sur la touche \b Echap provoque le même effet.\n
 *	\warning	Aucune sauvegarde (de configuration ou de partie) n'est effectuée lors
 *				de la fermeture du logiciel par le biais des actions mentionnées ci-dessus.\n
 *				Pour savoir comment sauver une partie avant de quitter, reportez-vous à la
 *				section de ce manuel concernant la \ref sauvegarde "sauvegarde de jeu".
 *
 *	\subsection choix_dimension Choix de la dimension du plateau
 *	\image	html menu_dim.png
 *	Ce menu vous propose de choisir parmi les dimensions proposées, celle que vous
 *	souhaitez affecter au plateau de jeu de votre partie.\n
 *	Pour choisir une dimension, cliquez sur la case correspondante.\n
 *		- Si vous choisissez une dimension inférieure ou égale à 7, le jeu passe automatiquement
 *		au menu de \ref choix_type "choix du type de partie".\n
 *		- Sinon, \ref humain_vs_humain "une partie de type Humain contre Humain" est directement lancée.
 *		.
 *	Pour revenir à l'\ref mainmenu "écran principal", cliquez sur le bouton "Retour".\n
 *
 *	\subsection choix_type Choix du type de partie
 *	\image	html menu_type.png
 *	Ce menu vous propose trois modes de jeu:
 *		- Humain vs Humain\n
 *		Un clic sur ce bouton \ref humain_vs_humain "lance immédiatement une partie en humain contre humain",
 *		sur un plateau dont la dimension a été choisie précédemment.
 *		- Humain vs Machine\n
 *		Un clic sur ce bouton appelle le menu de \ref choix_couleur "choix de la couleur".
 *		- Machine vs Machine
 *		Un clic sur ce bouton appelle le menu de \ref choix_ia "choix du type d'intelligence artificielle".
 *		.
 *	Pour revenir au \ref choix_dimension "menu de choix de choix de dimension", cliquez sur le bouton "Retour".
 *
 *	\subsection choix_couleur Choix de la couleur à jouer
 *	\image	html menu_couleur.png
 *	Ce menu vous permet de choisir la couleur que vous souhaitez jouer, lors d'une partie
 *	de type Humain contre Machine.\n
 *	Pour choisir une couleur, cliquez sur le bouton correspondant. Notez que le joueur noir
 *	commence toujours la partie.\n
 *	Une fois la couleur choisie, le jeu passe au \ref choix_ia "menu de choix de du type d'intelligence artificielle".\n
 *	Pour revenir au \ref choix_type "menu de choix du type de partie", cliquez sur le bouton "Retour".
 *
 *	\subsection choix_ia Choix du type d'intelligence artificielle
 *	\image	html menu_ia.png
 *	Ce menu vous permet de choisir le type d'intelligence artificielle que vous souhaitez affronter
 *	dans le cas d'une partie de type Humain contre Machine, ou le type des deux joueurs virtuels
 *	dans le cas d'une partie de type Machine contre Machine.\n
 *		- Brainless\n
 *		Cette intelligence artificielle joue au hasard. Idéale pour les joueurs frustrés de perdre,
 *		ou ayant besoin de gagner en confiance. Un clic sur ce bouton
 *		\ref humain_vs_machine "lance directement la partie".
 *		- Losanges\n
 *		Cette intelligence artificielle analyse les situations de jeu en se basant sur les différentes
 *		connexions possibles et établies dans les deux camps. Elle présente donc un meilleur niveau
 *		de jeu que l'intelligence "Brainless".\n
 *		Un clic sur ce bouton appelle le \ref choix_difficulte "menu de choix de la difficulté".
 *		- Electrique\n
 *		Cette intelligence artificielle est actuellement indisponible, et le bouton, bien que présent,
 *		n'est donc pour le moment pas cliquable.
 *		.
 *	Pour revenir au \ref choix_couleur "menu de choix de la couleur", cliquez sur le bouton "Retour".
 *
 *	\subsection choix_difficulte Choix de la difficulté de l'intelligence artificielle
 *	\image	html menu_difficulte.png
 *	Ce menu vous permet de choisir la difficulté de l'intelligence artificielle dans le cas d'une partie
 *	de type Humain contre Machine, ou encore Machine contre Machine.\n
 *	\note   Plus la difficulté est élevée, plus le temps de calcul nécessaire au jeu de l'intelligence
 *			artificielle est élevé.\n
 *	\note	Le mode de difficulté extrême, "Nash 2 en 1", empêche l'affichage des pions sur le plateau.
 *			Ainsi, vous devrez retenir l'emplacement de vos pions, ainsi que ceux de l'intelligence artificielle.
 *	Pour revenir au \ref choix_ia "menu de choix du type d'intelligence artificielle", cliquez sur le bouton "Retour".
 *
 *	\section	jeu Déroulement d'une partie
 *	Cette section décrit les commandes à votre disposition durant le déroulement d'une partie
 *	\ref nouvelle_partie "précédemment configurée".
 *
 *	\subsection	general	Déroulement général
 *	Une partie continue jusqu'à ce que l'un des joueurs gagne, ou jusqu'à ce que le jeu soit
 *	arrêté (par \ref quitter "fermeture" ou par \ref sauvegarde "sauvegarde").\n
 *	Chaque joueur pose un pion sur une case vide du plateau, lorsque son tour arrive.\n
 *	Un panneau placé en bas à gauche de l'écran de jeu indique la couleur du joueur
 *	qui doit actuellement jouer:
 *	\image	html exemple_jeu.png
 *
 *	\subsection	humain_vs_humain Partie de type "Humain contre Humain"
 *	Durant ce type de partie, seuls des joueurs humains interviennent, et le joueur
 *	de couleur noire doit placer son pion en premier.\n
 *	Pour placer un pion, chaque joueur doit manipuler la souris, et faire un clic gauche
 *	au dessus de la case sur laquelle il désire placer son pion.\n
 *	Une fois le pion placé, le tour passe automatiquement au joueur suivant, et le
 *	panneau d'indication est mis à jour en conséquence. C'est donc au tour de l'autre
 *	joueur de prendre la souris en main, et de placer son pion de la même manière (un clic
 *	gauche).\n\n
 *	\note	A tout moment, la partie peut être quittée sans sauvegarde par un appui sur le bouton
 *			Echap du clavier, ou un clic gauche sur le bouton de fermeture de la fenêtre. (voir aussi:
 *			\ref quitter)
 *	\note	De plus, la partie peut être sauvegardée en mémoire, pour pouvoir être reprise par la
 *			suite avec un clic sur le bouton \ref charger_partie "Charger partie" du
 *			\ref mainmenu "menu principal". (voir aussi: \ref sauvegarde)
 *
 *
 *	\subsection	humain_vs_machine Partie de type "Humain contre Machine"
 *	Ce type de partie ne fait intervenir qu'un seul joueur humain, qui s'oppose
 *	à un joueur "virtuel", dont les coups sont régis par une intelligence artificielle
 *	qui peut analyser le plateau et jouer en conséquence, ou simplement jouer au hasard.
 *	(ce comportement dépend du choix effectué dans le
 *	\ref humain_vs_machine "menu de choix du type d'intelligence artificielle")
 *	\note	Lorsque vient votre tour (c'est-à-dire quand l'intelligence artificielle
 *			a placé son pion, ou au premier tour si vous incarnez le joueur noir),
 *			vous pouvez choisir de \ref quitter "quitter la partie", ou la
 *			\ref sauvegarde "sauvegarder".
 *
 *	\subsection	machine_vs_machine Partie de type "Machine contre Machine"
 *	Dans ce mode de jeu, seuls des joueurs artificiels interviennent. L'humain
 *	devient donc un simple spectateur.\n
 *	Cependant, si les intelligences artificielles sont d'un type autre que "Brainless"
 *	(par exemple: losange), un humain doit appuyer sur une touche quelconque du clavier
 *	à la fin de chaque coup joué par l'IA pour donner la main au joueur suivant.\n
 *	\warning	Durant une partie de type Machine contre Machine, la sauvegarde est
 *				impossible (cette action ne peut être déclenchée que par un humain
 *				impliqué dans la partie, lorsque celui-ci a la main).\n
 *				Vous pouvez cependant forcer la fermeture du programme en cliquant
 *				sur le bouton de fermeture de la fenêtre.
 *
 *	\subsection	sauvegarde Sauvegarder une partie en cours
 *	Quand un joueur humain est présent dans une partie, il peut en provoquer la sauvegarde
 *	lorsque vient son tour, en appuyant sur la touche \b S du clavier au lieu de placer un pion
 *	ou de quitter le jeu.\n
 *	Le jeu tente alors de sauvegarder l'état exact de la partie dans le fichier save.hex,
 *	et si la sauvegarde est réussie, un retour à l'\ref mainmenu "écran d'accueil" est
 *	effectué.\n
 *	En cas d'échec de la sauvegarde, le jeu ne quitte pas la partie en cours, afin que
 *	vous puissiez la terminer, ou quitter sans sauvegarder.\n
 *	Pour charger une partie après l'avoir sauvegardée, cliquez sur le bouton
 *	\ref charger_partie "Charger partie" du \ref mainmenu "menu principal".
 *
 */
