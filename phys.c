#include <gtk/gtk.h>
#include <cairo.h>

#include <structs.h>
#include <prjcairo.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Funcoes internas e funcoes fisicas */

//Funcao dos focos conjugados

double
fconj (double a, double b)
{
  /* (1/c) = (1/a) + (1/b) retorna f */
  return 1 / (1 / a + 1 / b);
}

//Para uma recta que passa em (x1,y1) e (x2,y2) dá y3 de um x3

double
lin (double x1, double y1, double x2, double y2, double x3)
{
  return ((y2 - y1) / (x2 - x1)) * (x3 - x2) + y2;
}

//Para uma recta que passa em (x1,y1) e (x2,y2) dá x3 de um y3

double
alin (double x1, double y1, double x2, double y2, double y3)
{
  return ((x2 - x1) / (y2 - y1)) * (y3 - y2) + x2;
}

void
calcs (draw * pts, lens * lens1, lens * lens2)
{
  // bits da vmask em 1 implica que o raio é virtual
  double temp;
  pts->px1[0] = 0;
  pts->px1[1] = *(lens1->pos);

  pts->pe1[1] = *(pts->ldn) / 2;
  pts->pe1[0] = (pts->pe1[1]) - (tan (pts->ang) * pts->px1[1]);

  pts->vmask = (*(lens1->focus) < 0) ? 2 : 0;
  temp = pts->px1[1] + *(lens1->focus);

  if(*(lens2->pos) < temp)
    pts->vmask |= 4;
  pts->px1[2] = (*(lens2->pos) < temp)? *(lens2->pos) : temp;
  pts->px1[3] = (pts->px1[2] == temp) ? *(lens2->pos): temp;

  pts->pe1[2] = (tan (pts->ang)) * pts->px1[2] + pts->pe1[0];
  pts->pe1[3] = (tan (pts->ang)) * pts->px1[3] + pts->pe1[0];
  pts->pp1[3] = pts->pp1[2] = pts->pp1[1] =  
    pts->pe1[2+((pts->vmask & 4)==4)];
  pts->pp1[0] = (pts->pp1[1]) - (tan (pts->ang) * pts->px1[1]);
/*   
  pts->px2[4] = *(lens2->pos) + (pts->px1[3] - pts->px1[2]) * (*(lens2->focus)) / 
    (*(lens2->focus) + pts->px1[3] - pts->px1[2]);

  pts->pye2[4] = lin (pts->px1[3], pts->pe1[3],
     pts->px1[2], pts->pe1[1], pts->px1[5]);
  pts->pyp2[4] = lin (pts->px1[2], pts->pp1[2], *(lens2->pos) + *(lens2->focus),
	 pts->pe1[1], pts->px1[5]);
*/
/*
  pts->px1[6] =(double) *(pts->lrt);
  pts->pe1[6] = lin (pts->px1[3], pts->pe1[3],
     pts->px1[2], pts->pe1[1], pts->px1[6]);
  pts->pp1[6] = lin (pts->px1[4], pts->pp1[4],
     pts->px1[5], pts->pp1[5], pts->px1[6]);
*/
  return;
}
