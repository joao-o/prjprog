#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

#include <structs.h>
#include <cairofunc.h>
#include <phys.h>
#include <time.h>

#define TOL 20			//Distância ente eixo optico e borda da drawbox

#define NPTS 7

const double dash[] = { 8., 8. };

//tr: Recomendo o tracejado com x2 mais off que on
//   È mais agradável à vista e impede que a sobreposição de duas
//   linhas pareça não ter tracejado
const double nodash[] = { 1 };

//função sinal
double
dsign (double x)
{
  return (x > 0) - (x < 0);
}
// ^ ...

void
draw_line (cairo_t * cr, double x0, double y0, double x1, double y1)
{
  cairo_move_to (cr, x0, y0);
  cairo_line_to (cr, x1, y1);
  return;
}

void
draw_varrow (double x, double y, double hgt, double focus, cairo_t * cr)
{
  double i;

  cairo_set_line_width (cr, 3);
  cairo_move_to (cr, x, y);
  cairo_rel_line_to (cr, 0, hgt);
  for (i = -1; i <= 1; i += 2)
    {
      cairo_move_to (cr, x, y - hgt);
      cairo_rel_line_to (cr,
		         i * (-0.02 * focus + 10),
		         (0.02 * focus + 10) * dsign (focus) * dsign (hgt));
      // numeros mágicos acima controlam o ajuste de curvatura
      // é do tipo y=mx+b
    }
}

gboolean
expose_ev (GtkWidget * widget, GdkEventExpose *event, progdata pdat)
{
  cairo_t *cr;
  lens *lens1, *lens2;
  int *wwidth = &pdat->drawbox->allocation.width;
  static int init=1;
  static double buffer[5];

  pdat->phys.axis = pdat->drawbox->allocation.height/2;

  buffer[4] = tan ((M_PI / 180) * (GTK_ADJUSTMENT (pdat->barang.adj))->value);
  *(pdat->lnsd.focus) = -*(pdat->lnsd.focus);

  //verifica se init é 1 e seguidamete incrementa-a
  if (init++) 
      set_val (NULL,dat);

  (GTK_ADJUSTMENT (pdat->barl.adj))->upper = *wwidth - TOL;
  (GTK_ADJUSTMENT (pdat->barr.adj))->upper = *wwidth - TOL;

  cr = gdk_cairo_create (pdat->drawbox->window);

  // é aqui que a magia acontece 
  cairo_rectangle(cr, 0, 10,
          (double)*wwidth,(double) pdat->drawbox->allocation.height);
  cairo_clip(cr);






  return FALSE;
}
