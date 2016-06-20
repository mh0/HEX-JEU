#include "hexagons.h"
#include "board.h"
#include "PlayDeque.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define printf(format, args...) ;

// Détection du gagnant avec la structure union-find (simple et vue en DM de SD)
// Problème avec l'annulation du coup : pas d'opération de suppression!
// Solution 1 : Chaque noeud mémorise ses fils (donc tout son ensemble)
// Solution 2 : recréation de la structure grace à l'historique (plus couteux en 
// complexité, mais correction du code plus facile et reste rapide pour nos besoins)

/*
 * Algo : 
 * - Le dernier hexagone joué played_hex est représentant de lui-même.
 * - Les voisins de même couleur de played_hex sont représentés par played_hex
 * - Deux hexagones virtuels V1 et V2 sont voisins des hexagones sur les deux bords opposés.
 * - Le jeu se termine quand V1 et V2 ont le même représentant (donc connectés).
 * */
 
struct uf_node
{
	Hexagon key;
	struct uf_node * group;
} ;

typedef struct uf_node* Node;

struct unionfind
{
	Node **board;
	Node V1;
	Node V2; // hexagones virtuels
};

typedef struct unionfind * UnionFind;

static UnionFind red_uf=NULL;
static UnionFind blue_uf=NULL;

static Hexagon V1(UnionFind uf);
static Hexagon V2(UnionFind uf);

/* ********* board functions ************ */
static Hexagon get_hex(int x, int y, UnionFind uf)
{
	assert(uf!=NULL);
	if (x<0 || x>10 || y<0 || y>10 )
		return NULL;
	else
	{
		if (uf->board[x][y]!=NULL)
			return uf->board[x][y]->key ;
	}
	return NULL;
}

static Hexagon* get_neighbours (UnionFind uf,Hexagon hex)
{
	assert(uf!=NULL);
	Hexagon * neighbours = (Hexagon *) malloc (7*sizeof(Hexagon));
	int x = HEXAGON_x(hex);
	int y = HEXAGON_y(hex);
	int color = HEXAGON_col(hex);
	neighbours[0]=get_hex(x-1,y,uf);
	neighbours[1]=get_hex(x,y-1,uf);
	neighbours[2]=get_hex(x+1,y-1,uf);
	neighbours[3]=get_hex(x+1,y,uf);
	neighbours[4]=get_hex(x-1,y+1,uf);
	neighbours[5]=get_hex(x,y+1,uf);
	neighbours[6]=NULL;
	if (color==RED)
	{
		if(y==0)
			neighbours[6]=V1(uf);
		if(y==10)
			neighbours[6]=V2(uf);
	}
	if (color==BLUE)
	{
		if(x==0)
			neighbours[6]=V1(uf);
		if(x==10)
			neighbours[6]=V2(uf);
	}
	return neighbours;
}


/* ******** BEGIN Node ****************** */

static Node NewNode (Hexagon key)
{
	Node n = (Node) malloc (sizeof(struct  uf_node));
	assert(n!=NULL);
	n->key=key;
	n->group=n;
	return n;
}

static void PrintNode(Node n)
{
	return;// fonction désactivée
	printf("\nPrintNode: \nKey: ");
	HEXAGON_print(n->key);
	printf("Direct Group: ");
	HEXAGON_print(n->group->key);
	printf("END PrintNode \n-----------\n");
}

static Node SetGroup (Node n, Node Group)
{
	assert(n!=NULL);
	n->group=Group;
	return n;
}

static Node Group(Node n)
{
	assert(n!=NULL);
	return n->group;
}

static Hexagon Key(Node n)
{
	assert(n!=NULL);
	return n->key;
}

/* ******** END Node ******************** */


/* ******** BEGIN Unionfind ******** */

static Node uf_insert(UnionFind uf, Hexagon key)
{
	assert(uf!=NULL);
	int i = HEXAGON_x(key);
	int j = HEXAGON_y(key);
	if (uf->board[i][j]==NULL)
		//free(uf->board[i][j]);
	uf->board[i][j]= NewNode(key);
	return uf->board[i][j];
}

// Retourne les noeuds des hexagones du bord (non virtuels)
static Node node_search (UnionFind uf, Hexagon key )
{
	assert(uf!=NULL);
	if (V1(uf)==key)
		return uf->V1;
	if (V2(uf)==key)
		return uf->V2;

	int i = HEXAGON_x(key);
	int j = HEXAGON_y(key);
	//assert (uf->board[i][j]!=NULL);
	return uf->board[i][j];
}


// Fonctions de Union Find :

UnionFind uf_create()
{
	UnionFind tmp = (UnionFind) malloc(sizeof(struct unionfind));
	assert(tmp!=NULL);
	int i,j;
	tmp->board=(Node **) malloc(11*sizeof(Node*));

	for (i=0; i<11 ; i++)
	{
		tmp->board[i]=(Node*) malloc(sizeof(Node)*11);
		for (j=0; j<11 ; j++)
			tmp->board[i][j]=NULL;
	}
	tmp->V1=NewNode(HEXAGON_new(4,2,EMPTY));
	tmp->V2=NewNode(HEXAGON_new(6,9,EMPTY));
	SetGroup(tmp->V1,tmp->V1);
	SetGroup(tmp->V2,tmp->V2);
	return tmp;
}

static Hexagon V1(UnionFind uf)
{
	assert(uf!=NULL);
	return uf->V1->key;
}

static Hexagon V2(UnionFind uf)
{
	assert(uf!=NULL);
	return uf->V2->key;
}

void uf_destroy(UnionFind * uf)
{
	int i,j;
	assert((*uf)!=NULL);
	free((*uf)->V1->key) ;
	free((*uf)->V1) ;
	free((*uf)->V2->key) ;
	free((*uf)->V2) ;
	for (i=0; i<11 ; i++)
	{
		for (j=0; j<11 ; j++)
			if((*uf)->board[i][j]!=NULL){
		//		printf("destroying node: \n");
		//		PrintNode((*uf)->board[i][j]);
				free((*uf)->board[i][j]);
				
			}
		free((*uf)->board[i]);
	}
	free((*uf)->board);
	free(*uf);
	*uf=NULL;
}


Node find_node (UnionFind uf, Hexagon key)
{
	assert(uf!=NULL);
	Node n = node_search(uf,key);
	if (n!=Group(n))
		SetGroup(n, find_node(uf, Key(Group(n))));
	return 	Group(n);
}

Hexagon uf_find (UnionFind uf, Hexagon vertex)
{
	Node n = find_node(uf, vertex);
	if (n!=NULL)
		return Key(Group(n));
	else
		return NULL;
}

//vertex_a: cliqué          //vertex_b:voisin
Node uf_union (UnionFind uf, Hexagon vertex_a, Hexagon vertex_b)
{
	Node a=uf_insert(uf,vertex_a);
	SetGroup(a, a);

	if (vertex_a!=vertex_b)
	{
		Node group_b=find_node(uf,vertex_b);
		SetGroup(group_b,a);
	}
	return a;
}


void WINNER_init()
{
	red_uf=uf_create();
	blue_uf=uf_create();
}

bool WINNER_detect(Hexagon played_hex)
{
	assert(blue_uf!=NULL);
	assert(red_uf!=NULL);

	UnionFind uf ;
	int c = HEXAGON_col(played_hex);
	int i = HEXAGON_x(played_hex);
	int j = HEXAGON_y(played_hex);
	assert(i<11 && j <11 && i>-1 && j >-1);
	assert(c==RED || c==BLUE);
	if (c==RED)
		uf=red_uf;
	else
		uf=blue_uf;
	Hexagon *neighbours=get_neighbours(uf,played_hex);
	Node a= uf_union(uf,played_hex,played_hex);
	printf("Inserted Node:");
	PrintNode(a);
	int k;
	printf("Vertex A (played_hex):");
	HEXAGON_print(played_hex);
	for (k=0; k<7 ; k++)
		if (neighbours[k])
		{
			printf("adding neighbor: ");
			HEXAGON_print(neighbours[k]);
			uf_union ( uf, played_hex, neighbours[k] );
		}
	free(neighbours);
	bool win = ( uf_find(uf,uf->V1->key) == uf_find(uf,uf->V2->key));
	/*
	printf("\n--------------V1-------------------");
	PrintNode(uf->V1);
	printf("--------------V1---super-group-----");
	PrintNode(find_node(uf,V1(uf)));

	printf("\n--------------V2-------------------");
	PrintNode(uf->V2);
	printf("--------------V2---super-group-----");
	PrintNode(find_node(uf,V2(uf)));
	 * */


	return win ;
}

void WINNER_destroy()
{
	uf_destroy(&red_uf);
	uf_destroy(&blue_uf);
	red_uf=NULL;
	blue_uf=NULL;
}

