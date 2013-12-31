#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

#include <structs.h>
#include <cairofunc.h>
#include <phys.h>

#define TOL 20			//Distância ente eixo optico e borda da drawbox

#define NPTS 7

//função sinal
double
dsign (double x)
{
  return (x > 0) - (x < 0);
}

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

  cairo_set_line_width (cr, 2);
  cairo_move_to (cr, x, y);
  cairo_line_to (cr, x, y - hgt);
  for (i = -1; i <= 1; i += 2)
    {
      cairo_move_to (cr, x, y - hgt);
      cairo_line_to (cr,
		     x + i * (-0.02 * focus + 10),
		     y - hgt + 
                       (0.02 * focus + 10) * dsign (focus) * dsign (hgt));
      // numeros mágicos acima controlam o ajuste de curvatura
      // é do tipo y=mx+b
    }
}

gboolean
expose_evv (GtkWidget * widget, GdkEventExpose * event, gpointer dat)
{
  progdata *pdat;
  cairo_t *cr;
  lens *lens1, *lens2;
  double midref;
  int i;

  pdat = (progdata *) dat;
  midref = *(pdat->pts.ldn) / 2;
  (pdat->pts.ang) = (M_PI/180)* (GTK_ADJUSTMENT (pdat->barang.adj))->value;
  *(pdat->lnsd.focus)= -*(pdat->lnsd.focus); 

  if ((*(pdat->pts.lrt) - TOL) < *(pdat->lnsc.pos))
    *(pdat->lnsc.pos) = *(pdat->pts.lrt) - TOL;
  (GTK_ADJUSTMENT (pdat->barl.adj))->upper = *(pdat->pts.lrt) - TOL;

  if ((*(pdat->pts.lrt) - TOL) < *(pdat->lnsd.pos))
    *(pdat->lnsd.pos) = *(pdat->pts.lrt) - TOL;
  (GTK_ADJUSTMENT (pdat->barr.adj))->upper = *(pdat->pts.lrt) - TOL;
  cr = gdk_cairo_create (pdat->drawbox->window);

  cairo_set_source_rgb (cr, 1., 1., 1.);
  draw_line (cr, 0, midref, pdat->drawbox->allocation.width, midref);
  cairo_stroke(cr);
  //desenha lente convergente
  cairo_set_source_rgb (cr, 1., 0.55, 0.);

  draw_varrow (*(pdat->lnsc.pos), midref, pdat->lensdata.ylen,
	       *(pdat->lnsc.focus), cr);
  draw_varrow (*(pdat->lnsc.pos),
	       midref, -pdat->lensdata.ylen, *(pdat->lnsc.focus), cr);

  // desenha lente divergente
  cairo_set_source_rgb (cr, 1., 0.55, 0.);

  draw_varrow (*(pdat->lnsd.pos),
	       midref, pdat->lensdata.ylen, *(pdat->lnsd.focus), cr);
  draw_varrow (*(pdat->lnsd.pos),
	       midref, -pdat->lensdata.ylen, *(pdat->lnsd.focus), cr);
  cairo_stroke(cr);

  //verifica primeira lente
  if (*(pdat->lnsc.pos) < *(pdat->lnsd.pos))
    {
      lens1 = &(pdat->lnsc);
      lens2 = &(pdat->lnsd);
    }
  else
    {
      lens1 = &(pdat->lnsd);
      lens2 = &(pdat->lnsc);
    }

  calcs (&(pdat->pts), lens1, lens2);

  cairo_set_line_width (cr, 1);
  cairo_set_source_rgb (cr, 1., 1., 0);

  for (i=0;i<2;i++){
    draw_line(cr,pdat->pts.px[i],pdat->pts.pye[i],
             pdat->pts.px[i+1],pdat->pts.pye[i+1]);

    draw_line(cr,pdat->pts.px[i],pdat->pts.pyp[i],
             pdat->pts.px[i+1],pdat->pts.pyp[i+1]);
  }
  cairo_stroke(cr);

  for (;i<6;i++){
    draw_line(cr,pdat->pts.px[i],pdat->pts.pye[i],
             pdat->pts.px[i+1],pdat->pts.pye[i+1]);

    draw_line(cr,pdat->pts.px[i],pdat->pts.pyp[i],
             pdat->pts.px[i+1],pdat->pts.pyp[i+1]);
  }
  cairo_stroke(cr);

  //desenha imagens
  cairo_set_source_rgb (cr, 0., 0., 1.);

  draw_varrow (pdat->pts.px[3],
	       midref, midref-pdat->pts.pyp[3],50, cr);
  draw_varrow (pdat->pts.px[5],
	       midref, midref-pdat->pts.pyp[5],50, cr);

  cairo_stroke(cr);
  cairo_destroy (cr);
  *(pdat->lnsd.focus)= -*(pdat->lnsd.focus); 

  return FALSE;

}
