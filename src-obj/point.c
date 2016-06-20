#include "point.h"
#include <stdlib.h>
#include <stdio.h>

struct t_Point {
	int x;
	int y;
} ;

Point POINT_new(int x, int y)
{
	Point p =(Point) malloc(sizeof(struct t_Point)) ;
	p->x=x;
	p->y=y;
	return p;
}

Point POINT_translate(Point p, Point vect)
{
	return POINT_new(p->x+vect->x,p->y+vect->y);
}

int POINT_x(Point p)
{
	return p->x;
}

int POINT_y(Point p)
{
	return p->y;
}

void POINT_print(Point p)
{
	printf ("Point : (x,y)=(%d,%d)\n",POINT_x(p), POINT_y(p));
}

void POINT_destroy (Point p) {
	if (p!=NULL) {
		free(p);
		p=NULL;
	}
	else 
		printf("!Avertissement! POINT_destroy: p==NULL\n");
}
