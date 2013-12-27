#include <gtk/gtk.h>
#include <cairo.h>

#include <structs.h>
#include <prjcairo.h>

#include <stdio.h>
#include <stdlib.h>

/* Funcoes internas e funcoes fisicas */

//Funcao dos focos conjugados

double fconj (double p, double q)
{
  /* (1/f) = (1/p) + (1/q) retorna f */
  return 1/(1/p+1/q);
}

//Para uma recta que passa em (x1,y1) e (x2,y2) dá y3 de um x3

double lin (double x1, double y1, double x2, double y2, double x3)
{
  return ((y2-y1)/(x2-x1))*(x3-x2)+y2;
}

//Para uma recta que passa em (x1,y1) e (x2,y2) dá x3 de um y3

double alin (double x1, double y1, double x2, double y2, double y3)
{
  return ((x2-x1)/(y2-y1))*(y3-y2)+x2;
}
