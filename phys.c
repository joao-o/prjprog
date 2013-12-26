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
  /* (1/f) = (1/p) + (1/q) retorna f*/
  return 1/(1/p+1/q);
}
