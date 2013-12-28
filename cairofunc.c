#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

#include <structs.h>
#include <cairofunc.h>
#include <phys.h>

#define TOL 20 //Distância ente eixo optico e borda da drawbox

//função sinal
double
dsign (double x)
{
  return (x > 0) - (x < 0);
}

void
draw_line (cairo_t * cr, double x0, double y0, double x1, double y1)
{
  cairo_move_to(cr,x0,y0);
  cairo_line_to(cr,x1,y1);
  cairo_stroke(cr);
  return;
}

void
draw_varrow (double x, double y, double hgt, double focus, cairo_t * cr)
{
  double i;

  cairo_set_line_width (cr, 2);
  cairo_move_to (cr, x, y);
  cairo_line_to (cr, x, y - hgt);
  cairo_stroke (cr);
  for (i = -1; i <= 1; i += 2)
    {
      cairo_move_to (cr, x, y - hgt);
      cairo_line_to (cr,
		     x + i * (-0.05 * focus + 10),
		     y - hgt + (0.05 * focus +
				10) * dsign (focus) * dsign (hgt));
      // numeros mágicos acima controlam o ajuste de curvatura
      // é do tipo y=mx+b
      cairo_stroke (cr);
    }
}

gboolean
expose_evv (GtkWidget * widget, GdkEventExpose * event, gpointer dat)
{
  progdata *pdat;
  cairo_t *cr;
  double posc, posd,midref;
  double *lens1, *lens2;

  pdat = (progdata *) dat;

  posc = (GTK_ADJUSTMENT (pdat->barr.adj))->value;
  posd = (GTK_ADJUSTMENT (pdat->barl.adj))->value;
  midref = (pdat->drawbox->allocation.height)/2;

  if (pdat->drawbox->allocation.width < posc)
    posc = pdat->drawbox->allocation.width;
  (GTK_ADJUSTMENT (pdat->barl.adj))->upper = 
    pdat->drawbox->allocation.width - TOL;

  if (pdat->drawbox->allocation.width < posd)
    posd = pdat->drawbox->allocation.width;
  (GTK_ADJUSTMENT (pdat->barr.adj))->upper =
    pdat->drawbox->allocation.width - TOL;

  cr = gdk_cairo_create (pdat->drawbox->window);

  cairo_set_source_rgb (cr, 1.,1.,1.);
  draw_line(cr,TOL,midref,pdat->drawbox->allocation.width-TOL,midref); 

  //desenha lente convergente
  cairo_set_source_rgb (cr, 1., 0.55, 0.);

  draw_varrow (posc,
	       midref,
	       pdat->lensdata.ylen,
	       (GTK_ADJUSTMENT (pdat->barfc.adj))->value, cr);

  draw_varrow (posc,
	       midref,
	       -pdat->lensdata.ylen,
	       (GTK_ADJUSTMENT (pdat->barfc.adj))->value, cr);

  // desenha lente divergente
  cairo_set_source_rgb (cr, 1., 0.55, 0.);
  draw_varrow (posd,
	       midref,
	       pdat->lensdata.ylen,
	       -(GTK_ADJUSTMENT (pdat->barfd.adj))->value, cr);

  draw_varrow (posd,
	       midref,
	       -pdat->lensdata.ylen,
	       -(GTK_ADJUSTMENT (pdat->barfd.adj))->value, cr);

  //verifica primeira lente
  if (posc < posd)
    {
      lens1 = &posc;
      lens2 = &posd;
    }
  else
    {
      lens1 = &posd;
      lens2 = &posc;
    }
  //desenha raios para primeira lente

  cairo_set_source_rgb (cr, 1., 1., 0.);
  draw_line(cr,0,midref-0.5*pdat->lensdata.ylen,
            *lens1,midref-0.5*pdat->lensdata.ylen);
  draw_line(cr,0,midref+0.5*pdat->lensdata.ylen,
            *lens1,midref+0.5*pdat->lensdata.ylen);

  //desenha raios primeira para segunda lente 

  //desenha imagem primeira lente

  //desenha raios segunda infinito

  //desenha imagem segunda lente


  cairo_destroy (cr);
  return FALSE;

}
