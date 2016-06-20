#define VERBOSE 1
#include "debugging.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "PlayDeque.h"
#include "hexagons.h"

typedef struct t_node
{
	Hexagon hex;
	struct t_node * next ;
} node;


struct t_deque
{
	node * tail; // queue
	node * head; // tete
	unsigned int size ;
} ;

node* iterator_node = NULL;


PlayDeque PLAYDEQUE_init()
{
	PlayDeque d;
	d = (PlayDeque) malloc(sizeof( struct t_deque));
	d->tail = NULL;
	d->head = NULL;
	d->size = 0;
	return d;
}

unsigned int PLAYDEQUE_size(PlayDeque d){
	return d->size;
}


//retourne 1 si la pile est vide et 0 sinon
bool PLAYDEQUE_empty (PlayDeque d)
{
	assert(d!=NULL);
	return (d->head == NULL && d->tail == NULL);

}


/*fonction qui permet d'empiler un hexagon passer en paramétre a la téte de la pile passé en paramétre */
void PLAYDEQUE_push_front (PlayDeque d, Hexagon h)
{
	//on crée une structure node qui prend les valeurs passée en paramétres
	node *hexa = (node*) malloc(sizeof(node));
	assert (hexa != NULL);
	hexa->hex = h;
	hexa->next=NULL;

	//cas ou la pile est vide
	if (PLAYDEQUE_empty(d))
	{
		d->head = hexa;
		d->tail = hexa;
		d->size = 1;
	}

	//cas pile non vide
	else
	{
		d->head->next=hexa;
		d->head=hexa;
		d->size++;
	}
}

/*fonction qui peret d'empiler un hexagon passer en paramétre a la queue de la pile */
void PLAYDEQUE_push_back (PlayDeque d, Hexagon h)
{
	//on crée une structure node qui prend les valeurs passée en paramétres
	node *hexa = (node*) malloc(sizeof(node));
	assert (hexa != NULL);
	hexa->hex = h;


	//cas ou la pile est vide
	if (PLAYDEQUE_empty(d))
	{
		hexa->next = NULL;;
		d->head = hexa;
		d->tail = hexa;
		d->size = 1;
	}

	//cas pile non vide
	else
	{
		hexa->next=d->tail;
		d->tail=hexa;
		d->size++;
	}
}

/*recuperer la téte de la pile sans la supprimer  */
Hexagon PLAYDEQUE_get_front (PlayDeque d)
{
	assert(!PLAYDEQUE_empty(d));
	return d->head->hex;
}

/*recupéré la queue de la pile sans la supprimer */
Hexagon PLAYDEQUE_get_back (PlayDeque d)
{
	assert(d!=NULL);
	return d->tail->hex;
}


/*recupéré la téte de la pile avec suppression */
Hexagon PLAYDEQUE_pop_front (PlayDeque d)
{
	Hexagon r=NULL;
	if((PLAYDEQUE_empty(d)))
		return r;
	if(d->size==1)
	{
		r=d->head->hex;
		d->head=NULL;
		d->tail=NULL;
		d->size=0;
		return r;
	}

	// size>1

	node *it = d->tail->next;
	node *prev=d->tail;
	while ( it->next != NULL )
	{
		prev=it;
		it=it->next;
	}
	assert(it->next==NULL);
	r=it->hex;
	free(it);
	prev->next=NULL;
	d->head=prev;
	d->size--;
	return (r);
}


/*recupéré la queue de la pile en la supprimant */
Hexagon PLAYDEQUE_pop_back (PlayDeque d)
{

	Hexagon r=NULL;
	if((PLAYDEQUE_empty(d)))
		return r;

	node *back = d->tail;
	r=back->hex;
	d->tail = back->next;
	if (d->tail==NULL)
		d->head=NULL;
	free(back);
	d->size--;
	return (r);
}


void PLAYDEQUE_print (PlayDeque d)
{
	printf("PLAYDEQUE_print:\n");
	if (!PLAYDEQUE_empty(d)){
	printf("HEAD: "); 
	HEXAGON_print(d->head->hex);
	printf("HEAD->NEXT: "); 
	printf("%p\n",d->head->next);
	
	printf("TAIL: "); 
	HEXAGON_print(d->head->hex);
	printf("\nTAIL->NEXT: "); 
	printf("%p\n",d->tail->next);
	}
	else 
		printf("Empty DECK");
	
	node *courant = d->tail;
	while(courant != NULL)
	{
		HEXAGON_print (courant->hex);
		courant=courant->next;
	}
}

void PLAYDEQUE_destroy (PlayDeque d){
	assert(d!=NULL);
	node *courant = d->tail;
	node *prev;
	while(courant != NULL)
	{
		prev=courant;
		courant=courant->next;
		free(prev);
	}
	free(d);
	d=NULL;
}

void PLAYDEQUE_iter_init(PlayDeque d){
	assert(d!=NULL);
	iterator_node=d->tail;
}

Hexagon PLAYDEQUE_iterate(){
	if (iterator_node==NULL)
		return NULL;
	Hexagon hex= iterator_node->hex;
	iterator_node=iterator_node->next;
	return hex;
}
