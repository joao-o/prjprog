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
  pts->px[0]=0;
  pts->px[1]=*(lens1->pos);

  pts->pye[1]=*(pts->ldn)/2;
  pts->pye[0]=(pts->pye[1])-(tan(pts->ang)*pts->px[1]);

  pts->pyp[1]=*(pts->ldn)/2+(pts->ylen/2);
  pts->pyp[0]=(pts->pyp[1])-(tan(pts->ang)*pts->px[1]);

  pts->px[2]=*(lens2->pos);
  pts->pye[2]=lin(0,pts->pye[0],pts->px[1],pts->pye[1],pts->px[2]);//pode ser muito optimizado
  pts->pye[3]=lin(0,pts->pye[0],pts->px[1],pts->pye[1],
                  *(lens1->pos)+*(lens1->focus));//pode ser muito optimizado
  pts->pyp[2]=lin(pts->px[1],pts->pyp[1],
                  *(lens1->pos)+*(lens1->focus),pts->pye[3],pts->px[2]);

  return;
}
