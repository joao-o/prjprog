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
  static double buffer[5];

  double midref;

  pdat = (progdata *) dat;
  midref = pdat->drawbox->allocation.height / 2;
  buffer[4] = tan((M_PI / 180) * (GTK_ADJUSTMENT (pdat->barang.adj))->value);
  *(pdat->lnsd.focus) = -*(pdat->lnsd.focus);

  if ((pdat->drawbox->allocation.width - TOL) < *(pdat->lnsc.pos))
    *(pdat->lnsc.pos) = pdat->drawbox->allocation.width - TOL;
  (GTK_ADJUSTMENT (pdat->barl.adj))->upper =
    pdat->drawbox->allocation.width - TOL;

  if ((pdat->drawbox->allocation.width - TOL) < *(pdat->lnsd.pos))
    *(pdat->lnsd.pos) = pdat->drawbox->allocation.width - TOL;
  (GTK_ADJUSTMENT (pdat->barr.adj))->upper =
    pdat->drawbox->allocation.width - TOL;

  cr = gdk_cairo_create (pdat->drawbox->window);

  cairo_set_source_rgb (cr, 1., 1., 1.);
  draw_line (cr, 0, midref, pdat->drawbox->allocation.width, midref);
  cairo_stroke (cr);

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

  // calcs (&(pdat->pts), lens1, lens2);

  cairo_set_line_width (cr, 2);
  cairo_set_source_rgb (cr, 1., 1., 0);

  //stacks are fun
  //reminder : i = 4

  buffer[0] = *lens1->pos + *lens1->focus;	//x foco lente 1
  buffer[3] = midref - buffer[4] * *(lens1->pos);
  buffer[1] = buffer[4] * buffer[0] + buffer[3];	//y foco lente 1
  buffer[2] = buffer[4] * *(lens2->pos) + buffer[3];	//y raio paralelo lente 2

  // desenha reais

  draw_line (cr, 0, buffer[3], *(lens1->pos), midref);
  draw_line (cr, 0, buffer[1] - midref + buffer[3], *lens1->pos, buffer[1]);
  draw_line (cr, *(lens1->pos), midref, *lens2->pos, buffer[2]);
  draw_line (cr, *(lens1->pos), buffer[1], *lens2->pos, buffer[1]);

  // posição imagem lente 2
  buffer[3] = (buffer[0] - *lens2->pos) * *lens2->focus /
    (*lens2->focus + buffer[0] - *lens2->pos);

  // if (buffer[3]>*lens2->pos)
//      draw_line(cr,*lens2->pos,,buffer[3],);

  cairo_stroke (cr);

  if (pdat->virt)		//desenha virtuais
    {
      cairo_set_dash (cr, dash, 1, 0);
      cairo_set_source_rgb (cr, 0., 1., 0);
      if (*lens1->focus < 0)
	{
	  draw_line (cr, buffer[0], buffer[1], *lens1->pos, buffer[1]);
	  draw_line (cr, buffer[0], buffer[1], *lens1->pos, midref);
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
    }
  cairo_stroke (cr);














/*
  for (i = 0, j=1; i < imax; j*=2, i++)
    {
      if ((pdat->pts.vmask & j ) == j)
        continue;
      draw_line (cr, pdat->pts.px1[i], pdat->pts.pe1[i],
		 pdat->pts.px1[i + 1], pdat->pts.pe1[i + 1]);

      draw_line (cr, pdat->pts.px1[i], pdat->pts.pp1[i],
		 pdat->pts.px1[i + 1], pdat->pts.pp1[i + 1]);
    }
  cairo_stroke (cr);

  cairo_set_dash (cr, dash, 1, 0);
  cairo_set_source_rgb (cr, 0., 1., 0);
   
  for (i = 0, j=1; i < imax; j*=2, i++)
    {
      if ((pdat->pts.vmask & j ) != j)
        continue;
      draw_line (cr, pdat->pts.px1[i], pdat->pts.pe1[i],
		 pdat->pts.px1[i + 1], pdat->pts.pe1[i + 1]);

      draw_line (cr, pdat->pts.px1[i], pdat->pts.pp1[i],
		 pdat->pts.px1[i + 1], pdat->pts.pp1[i + 1]);
    }
  cairo_stroke (cr);


  for (;i<6;i++)
  {
    draw_line(cr,pdat->pts.px1[i],pdat->pts.pe1[i],
             pdat->pts.px1[i+1],pdat->pts.pe1[i+1]);

    draw_line(cr,pdat->pts.px1[i],pdat->pts.pp1[i],
             pdat->pts.px1[i+1],pdat->pts.pp1[i+1]);
  }
  cairo_stroke(cr);
*/
/*
  //desenha imagens
  cairo_set_source_rgb (cr, 0., 0., 1.);

  draw_varrow (pdat->pts.px1[2+((pdat->pts.vmask&4)==4)], 
    midref, midref - pdat->pts.pp1[2+((pdat->pts.vmask&4)==4)], 50, cr);
//draw_varrow (pdat->pts.px1[5],
//  midref, midref - pdat->pts.pp1[5], 50, cr);
*/
  cairo_stroke (cr);
  cairo_destroy (cr);
  *(pdat->lnsd.focus) = -*(pdat->lnsd.focus);

  return FALSE;

}
