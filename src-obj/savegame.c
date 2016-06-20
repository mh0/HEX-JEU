#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "PlayDeque.h"
#include "hexagons.h"
#include "savegame.h"
#define TAILLE_MAX 20
#define SAVE_GAME_PATH "save.txt"
#define MAX_TAB 11

// 1 = succès ; 0=échec
static char convert_couleur(int color)
{

	if (color == BLUE)
		return ('B');

	else if (color == RED)
		return ('R');

	else if (color == EMPTY)
		return ('.');
	else
	{
		printf("convert_couleur: invalid color : %d",color);
		return 0;
	}

}


static void convert_tab_to_chaine (char tab[MAX_TAB][MAX_TAB], int i, char chaine[MAX_TAB+1])
{
	//strcpy (chaine, tab[i]);
	int j=0;
	for ( j=0; j<MAX_TAB; j++)
	{
		chaine[j] = tab[i][j];
	}
	chaine[j] = '\0';
}


/*initialisation du tableau avec '.' */
static void init (char tab[MAX_TAB][MAX_TAB])
{
	int i, j;
	for(i=0; i<MAX_TAB; i++)
	{
		for(j=0; j<MAX_TAB; j++)
		{
			tab[i][j] = ('.');
		}
	}
}

bool SAVEGAME_save (PlayDeque Deque)
{

	char tableau[MAX_TAB][MAX_TAB];
	init(tableau);

	//ouverture fichier  et verification de l'ouverture
	FILE *fichier = fopen(SAVE_GAME_PATH,"w");
	if (fichier == NULL)
	{
		fprintf(stderr,"\n impossible d'ouvrire le fichier %s",SAVE_GAME_PATH);
		return 0;
	}

	//on itère sur les valeurs de Deque
	PLAYDEQUE_iter_init(Deque);
	Hexagon hex ;
	int i=0;
	while ((hex=PLAYDEQUE_iterate())!=NULL)
	{
		i++;
		tableau[HEXAGON_x(hex)][HEXAGON_y(hex)] = convert_couleur(HEXAGON_col(hex));    //on converti la couleur en caractére et on la met dans le tableau
	}

// ecriture dans le fichier
	fputs ("/hex\n",fichier);
	fputs ("/board\n",fichier);
	char chaine[MAX_TAB+1];

//ecrire les lignes du tableau (état du plateau) dans le fichier
	int j=0;
	while (j<MAX_TAB)
	{
		convert_tab_to_chaine(tableau,j,chaine);
		fputs(chaine,fichier);
		fputc('\n',fichier);
		j++;
	}

	fputs ("/endboard\n",fichier);

//ecriture de l'historique
	fputs ("/game\n",fichier);
	PLAYDEQUE_iter_init(Deque);
	Hexagon hex1;
	while ((hex1=PLAYDEQUE_iterate())!=NULL)
	{
		fputs("/play",fichier);
		fputc(' ',fichier);
		fputc(convert_couleur(HEXAGON_col(hex1)),fichier);
		fputc(' ',fichier);
		fprintf(fichier, "%d", HEXAGON_x(hex1));
		fputc(' ',fichier);
		fprintf(fichier, "%d", HEXAGON_y(hex1));
		fputc('\n',fichier);

	}

	fputs ("/endgame\n",fichier);

	fputs ("/endhex\n",fichier);

	fclose(fichier);

	return 1;

}

//retourne 1 si les tags(/hex et /board) sont bon 0 sinon
static bool verif_tag (FILE *fichier)
{
	int i = 0;

	while (i < 2)
	{
		char chaine[TAILLE_MAX] = " ";
		//recuperation de la chaine
		fgets(chaine,TAILLE_MAX,fichier);
		//si c la premiére ligne
		if (i == 0)
		{
			if (strcmp(chaine,"/hex\n") != 0)
				return 0;
		}
		//si c la deuxiéme ligne
		else if (i == 1)
		{
			if (strcmp(chaine,"/board\n") != 0)
				return 0;
		}

		i++;
	}

	return 1;

}


//retourne 1 si les tags (/endboard et /game) sont bon et 0 sinon
static bool verif_tag2 (FILE *fichier)
{
	int i = 0;

	while (i < 2)
	{
		char chaine[TAILLE_MAX] = " ";
		//recuperation de la chaine
		fgets(chaine,TAILLE_MAX,fichier);
		//si c la premiére ligne
		if (i == 0)
		{
			if (strcmp(chaine,"/endboard\n") != 0)
				return 0;
		}
		//si c la deuxiéme ligne
		else if (i == 1)
		{
			if (strcmp(chaine,"/game\n") != 0)
				return 0;
		}

		i++;
	}

	return 1;

}


//initialise le tableau avec des 0
static void init_int (int tab[MAX_TAB][MAX_TAB])
{
	int i, j;
	for(i=0; i<MAX_TAB; i++)
	{
		for(j=0; j<MAX_TAB; j++)
		{
			tab[i][j] = EMPTY;
		}
	}
}



static void affiche_tab(int tab[MAX_TAB][MAX_TAB])
{
	int i,j;
	for ( i = 0; i < MAX_TAB; i++)
	{
		for (  j = 0; j < MAX_TAB; j++)
		{
			printf("%d",tab[i][j]);
		}
		printf("\n");
	}
}

//retourne 0,1,2 pour un succé    ou   3 en cas d'echec
static int convert_col_to_int (char c)
{
	if (c =='.')
		return EMPTY;

	else if ( c == 'B')
		return BLUE;

	else if ( c == 'R')
		return RED;

	else
		return -1;

}

//ecrie la chaine 'chaine' dans la ligne 'i' du tableau 'tab'
static bool put_int_in_tab (int tab[MAX_TAB][MAX_TAB], char *chaine, int i)
{
	if (strlen(chaine)!=12)
		return false;
	int j;
	for (  j = 0; j < MAX_TAB; j++)
	{
		tab[i][j] = convert_col_to_int (chaine[j]);
		if (tab[i][j]==-1 )
			return false;
	}
	return true;

}

//charger le plateau du jeux (couleur) en forme d'entier
static bool charger_pos(int tab[MAX_TAB][MAX_TAB], FILE *fichier)
{
	char chaine[TAILLE_MAX];
	//pour chaque ligne du tableau
	int i;
	printf("charger_pos:\n");
	for (i = 0; i < MAX_TAB; i++)
	{
		
		//on recupére la ligne
		fgets(chaine,TAILLE_MAX,fichier);
		//on ecrie la ligne dans le tableau
		printf("(%d):chaine=%s",i,chaine);
		if(!put_int_in_tab(tab, chaine, i))
			return false;
	}
	return true;
}

//verifie que la couleur enregistrer dans l'historique correspond a celle qui se trouve dans le tableau  succé = 1, echec = 0
static int verif_non_contradiction (int tab[MAX_TAB][MAX_TAB], char play_str[TAILLE_MAX], char col, int x, int y)
{
	if (strcmp (play_str,"/play") != 0)
		return 0;
	if (! (x>-1 && y>-1 && x<11 && y<11)){
		printf("verif_non_contradiction: (%d,%d) out of range\n",x,y);
		return 0;
	}

	else if (tab[x][y] != convert_col_to_int(col))
		return 0;
	else
		return 1;
}



PlayDeque SAVEGAME_load ()
{
	PlayDeque d;
	d=PLAYDEQUE_init ();

	//ouverture fichier
	FILE *fichier_l = fopen(SAVE_GAME_PATH,"r");
	if (fichier_l == NULL)
	{
		fprintf(stderr,"impossible d'ouvrire le fichier %s\n",SAVE_GAME_PATH);
		return NULL;
	}

	//on verifie la justesse des deux premier tags   et on retourne NULL en cas d'erreurs
	int verif = verif_tag (fichier_l);
	if ( verif == 0 )
	{
		printf("tag1 incorrect !! \n");
		PLAYDEQUE_destroy( d );
		fclose(fichier_l);
		return NULL;
	}

	//on charge les position dans le tableau
	int tableau[MAX_TAB][MAX_TAB];
	init_int(tableau);

	bool pos=charger_pos(tableau,fichier_l);
	affiche_tab(tableau);
	if (!pos)
	{
		printf("charger_pos a échoué\n");
		PLAYDEQUE_destroy( d );
		fclose(fichier_l);
		return NULL;
	}

	//on verifie la 2éme partie des tags
	verif = verif_tag2 (fichier_l);
	if ( verif == 0)
	{
		printf("tag2 incorrect !! \n");
		PLAYDEQUE_destroy (d);
		fclose(fichier_l);
		return NULL;
	}

	//oncharge l'historique dans la pile avec verification
	//verification
	char play_str[TAILLE_MAX];
	char col = '.';
	int x = 0, y = 0;
	int nb = 0;

	//on continue tant que on est pas arriver a /endgame et on recupére tjr 4 élements
	nb = fscanf(fichier_l, "%s %c %d %d\n" , play_str, &col, &x, &y);
	if (nb!=4)
	{
		printf("Historique coorrmpu: fscanf retourne: %d \n",nb);
		PLAYDEQUE_destroy (d);
		fclose(fichier_l);
		return NULL;
	}
	while (nb == 4 && strcmp(play_str,"/endgame") != 0)
	{
		verif = verif_non_contradiction (tableau, play_str, col, x, y);

		//si il ya erreyr
		if (verif == 0)
		{
			printf("Echec chargement: pas de cohérence entre l'historique et la table de positions\n");
			printf("fscanf a lu: '%s''%c' '%d' '%d'\n ", play_str, col, x, y);
			printf("Couleur tab[%d][%d]=%d\n",abs(x%11),abs(y%11),tableau[abs(x%11)][abs(y%11)]);
			PLAYDEQUE_destroy (d);
			fclose(fichier_l);
			return NULL;
		}

		//sinon on empile l'haxagone dans la pile
		else
		{
			Hexagon h = HEXAGON_new (x , y , convert_col_to_int(col));
			PLAYDEQUE_push_front ( d, h) ;

		}

		//on avance vers la ligne suivante
		nb = fscanf(fichier_l, "%s %c %d %d\n" , play_str, &col, &x, &y);
		if (nb!=4 &&  strcmp(play_str,"/endgame") )
		{
			printf("Historique coorrmpu: fscanf retourne: %d \n",nb);
			printf("fscanf a lu: '%s''%c' '%d' '%d'\n ", play_str, col, x, y);
			printf("strcmp(play_str,'/endgame')=%d\n", strcmp(play_str,"/endgame"));
			PLAYDEQUE_destroy (d);
			fclose(fichier_l);
			return NULL;
		}

	}
	char last_check[128];
	fscanf(fichier_l,"%s",last_check);
	printf("Dernière ligne lue:'%s'\n",last_check);
	if (!strcmp(last_check,"/endhex")){
		printf("Avertissement chargement: le tag /endhex est manquant\n");
	}
	fclose(fichier_l);
	return d;
}
