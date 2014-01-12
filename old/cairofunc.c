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
expose_evv (GtkWidget * widget, GdkEventExpose * event, gpointer dat)
{
  cairo_t *cr;
  lens *lens1, *lens2;
  progdata *pdat = (progdata*) dat;
  int *wwidth = &pdat->drawbox->allocation.width;

  static double buffer[5];

  double midref;

  pdat = (progdata *) dat;
  midref = pdat->drawbox->allocation.height / 2;
  buffer[4] = tan ((M_PI / 180) * (GTK_ADJUSTMENT (pdat->barang.adj))->value);
  *(pdat->lnsd.focus) = -*(pdat->lnsd.focus);

  if ((*wwidth - TOL) < *(pdat->lnsc.pos))
    *(pdat->lnsc.pos) = *wwidth - TOL;
  (GTK_ADJUSTMENT (pdat->barl.adj))->upper = *wwidth - TOL;

  if ((*wwidth - TOL) < *(pdat->lnsd.pos))
    *(pdat->lnsd.pos) = *wwidth - TOL;
  (GTK_ADJUSTMENT (pdat->barr.adj))->upper = *wwidth - TOL;

  cr = gdk_cairo_create (pdat->drawbox->window);

  // é aqui que a magia acontece 
  cairo_rectangle(cr, 0, 10,
          (double)*wwidth,(double) pdat->drawbox->allocation.height);
  cairo_clip(cr);

  cairo_set_source_rgba (cr, 0., 0., 0., 1.);
  cairo_paint(cr);

  cairo_set_source_rgba (cr, 1., 1., 1., 1.);
  draw_line (cr, 0, midref, *wwidth, midref);
  cairo_stroke (cr);

  //desenha lente convergente
  cairo_set_source_rgba (cr, 1., 0.55, 0., 1.);

  draw_varrow (*(pdat->lnsc.pos), midref, pdat->ldat.ylen,
	       *(pdat->lnsc.focus), cr);
  draw_varrow (*(pdat->lnsc.pos),
	       midref, -pdat->ldat.ylen, *(pdat->lnsc.focus), cr);

  // desenha lente divergente
  cairo_set_source_rgba (cr, 1., 0.55, 0., 1.);

  draw_varrow (*(pdat->lnsd.pos),
	       midref, pdat->ldat.ylen, *(pdat->lnsd.focus), cr);
  draw_varrow (*(pdat->lnsd.pos),
	       midref, -pdat->ldat.ylen, *(pdat->lnsd.focus), cr);
  cairo_stroke (cr);

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

  cairo_set_line_width (cr, 1);
  cairo_set_source_rgba (cr, 1., 1., 0., 1.);

  //reminder : buffer[4] = declive recta que passa no eixo

  buffer[0] = *lens1->pos + *lens1->focus;	//x foco lente 1
  buffer[3] = midref - buffer[4] * *(lens1->pos);
  buffer[1] = buffer[4] * buffer[0] + buffer[3];	//y foco lente 1
  buffer[2] = buffer[4] * *(lens2->pos) + buffer[3];	//y raio eixo lente 2

  // desenha reais

  draw_line (cr, 0, buffer[3], *(lens1->pos), midref); //e
  draw_line (cr, 0, buffer[1] - midref + buffer[3], *lens1->pos, buffer[1]);//p
  draw_line (cr, *(lens1->pos), midref, *lens2->pos, buffer[2]);//e
  draw_line (cr, *(lens1->pos), buffer[1], *lens2->pos, buffer[1]);//p

  cairo_stroke (cr);

  if (pdat->flg.virt)		//desenha virtuais
    {
      cairo_set_dash (cr, dash, 1, 0);
      cairo_set_source_rgba (cr, 0., 1., 0., 1.);
      if (*lens1->focus < 0)
	{
	  draw_line (cr, buffer[0], buffer[1], *lens1->pos, buffer[1]);//e
	  draw_line (cr, buffer[0], buffer[1], *lens1->pos, midref);//p
	  //draw_line (cr,*lens1->pos,,buffer[0],buffer[1]);
	}
      else
	{
	  if (buffer[0] > *lens2->pos)
	    {
	      draw_line (cr, *lens2->pos, buffer[2], buffer[0], buffer[1]);
	      draw_line (cr, *lens2->pos, buffer[1], buffer[0], buffer[1]);
	    }
	  draw_line (cr, *lens2->pos, midref, buffer[0], buffer[1]);
	}
      cairo_stroke (cr);
    }

  buffer[3] = (buffer[0] - *lens2->pos);	//difrença entre fc(l1) e pos(l2)
  buffer[0] = (buffer[1] - midref) / buffer[3];	//declive do raio eixo lente2
  buffer[2] = *lens2->pos + (buffer[3] * *lens2->focus) / (*lens2->focus + buffer[3]);
  //x img2

  buffer[3] = buffer[0] * (buffer[2] - *lens2->pos) + midref;	//y img2
  buffer[4] = *wwidth - *lens2->pos;

  if (pdat->flg.virt)
    {
      cairo_set_dash (cr, dash, 1, 0);
      cairo_set_source_rgba (cr, 0., 1., 0, 1.);

      draw_line (cr, *lens2->pos, midref, buffer[2], buffer[3]);
      draw_line (cr, *lens2->pos, buffer[1], buffer[2], buffer[3]);
      cairo_stroke (cr);
    }

  cairo_set_dash (cr, nodash, 0, 0);
  cairo_set_source_rgba (cr, 1., 1., 0., 1.);

  draw_line (cr, *lens2->pos, midref, *wwidth,
	     buffer[0] * buffer[4] + midref);
  draw_line (cr, *lens2->pos, buffer[1], *wwidth,
	     (midref - buffer[1]) / *lens2->focus * buffer[4] + buffer[1]);

  cairo_stroke (cr);
  cairo_destroy (cr);
  *(pdat->lnsd.focus) = -*(pdat->lnsd.focus);

  return FALSE;
}
