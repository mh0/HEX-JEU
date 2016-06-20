#define VERBOSE
#include "debugging.h"
#include "hexagons.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct t_hexagon
{
	int x;
	int y;
	int color;
};

Hexagon HEXAGON_new (int x , int y, int color)
{
	DEBUG("HEXAGON_new(%d, %d) : %d\n",x,y,color);
	assert(color>-1 && color <3);
	assert(x<11 && y <11 && x>-1 && y >-1);

	Hexagon h ;
	h= (Hexagon) malloc(sizeof(struct t_hexagon));
	h->x=x;
	h->y=y;
	h->color=color;
	return h;
}

Hexagon HEXAGON_copy(Hexagon h)
{
	return HEXAGON_new(h->x,h->y,h->color);
}

void HEXAGON_print(Hexagon h)
{

	if (h)
	{
		DEBUG("HEXAGON_print: x=%d ; y=%d ; color=%d\n",h->x,h->y,h->color);

	}
	else
	{
		DEBUG("HEXAGON_print: NULL\n");
	}
}

void HEXAGON_setColor(Hexagon h, int new_color)
{
	DEBUG("change_hexagon_color : x=%d ; y=%d; color=%d (new_color=%d)\n",h->x,h->y,h->color,new_color);
	h->color=new_color;
}

int HEXAGON_x(Hexagon h)
{
	return h->x;
}
int HEXAGON_y(Hexagon h)
{
	return h->y;
}
int HEXAGON_col(Hexagon h)
{
	assert(h!=NULL);
	return h->color;
}
