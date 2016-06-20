#include <stdbool.h>
#include "point.h"
#include "polygon.h"
#include "stdlib.h"

struct t_polygon
{
	int *xpoints;
	int *ypoints;
	int npoints;
} ;


Polygon POLYGON_new()
{
	Polygon pol = (Polygon) malloc(sizeof(struct t_polygon));
	pol->npoints=0;
	pol->xpoints=NULL;
	pol->ypoints=NULL;
	return pol;
}

void POLYGON_addPoint(Polygon P, Point p)
{
	P->npoints++;
	P->xpoints = (int *) realloc(P->xpoints,sizeof(int)*P->npoints); // Il n y'a pas de soucis avec realloc ici
	P->ypoints = (int *) realloc(P->ypoints,sizeof(int)*P->npoints); // on n'aura que peu des millions de points !
	P->xpoints[P->npoints-1]=POINT_x(p);
	P->ypoints[P->npoints-1]=POINT_y(p);
}

void POLYGON_print(Polygon P)
{
	return; //fonction désactivée
	int i;
	printf("POLYGON_print:\n");
	for(i=0; i<P->npoints; i++)
		printf("Point %d : (%d,%d)\n",i,P->xpoints[i],P->ypoints[i]); 
}

bool POLYGON_contains(Polygon P, Point p)
{

	if(P->npoints <= 3)
		return false;
		
	int i;
	int hits = 0;
	
	int lastx = P->xpoints[P->npoints - 1];
	int lasty = P->ypoints[P->npoints - 1];
	int curx, cury;
	
	// Walk the edges of the polygon
	for(i = 0; i < P->npoints; lastx = curx, lasty = cury, i++)
		{
			curx = P->xpoints[i];
			cury = P->ypoints[i];
			if(cury == lasty)
				continue;
			int leftx;
			if(curx < lastx)
				{
					if(POINT_x(p) >= lastx)
						continue;
					leftx = curx;
				}
			else
				{
					if(POINT_x(p) >= curx)
						continue;
					leftx = lastx;
				}
			double test1, test2;
			if(cury < lasty)
				{
					if(POINT_y(p) < cury || POINT_y(p) >= lasty)
						continue;
					if(POINT_x(p) < leftx)
						{
							hits++;
							continue;
						}
						
					test1 = POINT_x(p) - curx;
					test2 = POINT_y(p) - cury;
				}
			else
				{
					if(POINT_y(p) < lasty || POINT_y(p) >= cury)
						continue;
					if(POINT_x(p) < leftx)
						{
							hits++;
							continue;
						}
						
					test1 = POINT_x(p) - lastx;
					test2 = POINT_y(p) - lasty;
				}
				
			if(test1 < (test2 / (lasty - cury) * (lastx - curx)))
				hits++;
		}
	return ((hits & 1) != 0);
}

Polygon POLYGON_translate(Polygon P, int x, int y)
{
	Polygon ret = POLYGON_new();
	int i;
	for(i=0; i<P->npoints; i++)
		POLYGON_addPoint(ret, POINT_new(P->xpoints[i]+x,P->ypoints[i]+y));
	return ret;
}

void POLYGON_destroy (Polygon P) {
	if (P!=NULL) {
		free(P->xpoints);
		free(P->ypoints);
		free(P);
		P=NULL;
	}
	else 
		printf("!Avertissement! POLYGON_destroy: P==NULL\n");
}

Point POLYGON_getPoint(Polygon p, int index) {
	return POINT_new (p->xpoints[index],p->ypoints[index]);
}

int POLYGON_nbPoints(Polygon p) {
	return p->npoints;
}
