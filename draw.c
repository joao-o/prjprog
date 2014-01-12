#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

#include <structs.h>
#include <cairofunc.h>
#include <phys.h>
#include <time.h>
#include <callbacks.h>

static const double dash[] = { 8.};
static const double imgdsh[] = { 4., 1. };
static const double nodash[] = { };

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

  cairo_set_line_width (cr, RAY);
  cairo_move_to (cr, x, y);
  cairo_rel_line_to (cr, 0, -hgt);
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

double
draw_vspear (double x, double y, double hgt, double focus, cairo_t * cr)
{
  double i = 240 / (fabs (focus) + 48.33) + 4.9;

  cairo_set_line_width (cr, 2);
  cairo_move_to (cr, x, y);
  cairo_rel_line_to (cr, 0, -hgt);
  cairo_stroke (cr);

  cairo_move_to (cr, x - i, y - hgt);
  cairo_rel_line_to (cr, i, -15 * dsign (focus) * dsign (hgt));
  cairo_rel_line_to (cr, i, 15 * dsign (focus) * dsign (hgt));
  cairo_close_path (cr);
  cairo_fill (cr);
  //fórmula mágica concedida pelo Grande Xamã Fit-Eha-Ya
  //               será que quis dizer: ^mago^ ?
  return i;
}


gboolean
expose_e (GtkWidget * widget, GdkEventExpose * event, progdata * pdat)
{
  cairo_t *cr;
  lens *lens1, *lens2;
  int *wwidth = &pdat->drawbox->allocation.width;
  static int init = 0;
  static double buffer[5];	//espaço para fazer contas

  pdat->phys.axis = pdat->drawbox->allocation.height / 2;

  buffer[4] = tan ((M_PI / 180) * (GTK_ADJUSTMENT (pdat->barang.adj))->value);
  pdat->phys.d.focus = -pdat->phys.d.focus;

  (GTK_ADJUSTMENT (pdat->barl.adj))->upper =
    (*wwidth - TOL) * *pdat->phys.scl;
  (GTK_ADJUSTMENT (pdat->barr.adj))->upper =
    (*wwidth - TOL) * *pdat->phys.scl;

  //verifica se init é 0 e seguidamete incrementa-a
  if (!init)
    {
      colorreset(NULL , pdat);
      set_val (NULL , (gpointer) pdat);
      ++init;
    }

  cr = gdk_cairo_create (pdat->drawbox->window);

  // é aqui que a magia acontece 
  cairo_rectangle (cr, 0, 10,
		   (double) *wwidth,
		   (double) pdat->drawbox->allocation.height);
  cairo_clip (cr);

  cairo_set_source_rgba (cr, 0., 0., 0., 1.);
  cairo_paint (cr);

  cairo_set_source_rgba (cr, 1., 1., 1., 1.);
  cairo_move_to (cr, 0, pdat->phys.axis);
  cairo_rel_line_to (cr, *wwidth, 0);
  cairo_stroke (cr);

  //verifica primeira lente

  if (*(pdat->lnsc.pos) < *(pdat->lnsd.pos))
    {
      lens1 = &(pdat->phys.c);
      lens2 = &(pdat->phys.d);
    }
  else
    {
      lens1 = &(pdat->phys.d);
      lens2 = &(pdat->phys.c);
    }

  cairo_set_line_width (cr, RAY);
  cairo_set_source_rgba (cr, 1., 1., 0., 1.);

  //reminder : buffer[4] = declive recta que passa no eixo

  buffer[0] = lens1->pos + lens1->focus;	//x foco lente 1
  buffer[3] = pdat->phys.axis - buffer[4] * (lens1->pos);	//ordenada na origem raio 
  buffer[1] = buffer[4] * buffer[0] + buffer[3];	//y foco lente 1
  buffer[2] = buffer[4] * lens2->pos + buffer[3];	//y raio eixo lente 2

  // desenha reais
  draw_line (cr, 0, buffer[3], lens1->pos, pdat->phys.axis);	//e
  draw_line (cr, lens1->pos, pdat->phys.axis, lens2->pos, buffer[2]);	//e
  draw_line (cr, 0, buffer[1] - pdat->phys.axis + buffer[3], lens1->pos, buffer[1]);	//p
  draw_line (cr, (lens1->pos), buffer[1], lens2->pos, buffer[1]);	//p

  cairo_stroke (cr);


  if (pdat->flg.virt)		//desenha virtuais
    {
      cairo_set_dash (cr, dash, 1, 0);
      cairo_set_source_rgba (cr, 0., 0.7, 1, 1);
      if (lens1->focus < 0)
	{
	  draw_line (cr, buffer[0], buffer[1], lens1->pos, buffer[1]);	//e
	  draw_line (cr, buffer[0], buffer[1], lens1->pos, pdat->phys.axis);	//p
          cairo_stroke(cr);
          cairo_set_dash(cr, imgdsh, 2, 0);
          cairo_set_source_rgba (cr, 0, 0.8 , 0.2, 1);
	  draw_varrow (buffer[0], pdat->phys.axis,
		       pdat->phys.axis - buffer[1], 20, cr);
	}
      else
	{
	  draw_line (cr, lens2->pos, pdat->phys.axis, buffer[0], buffer[1]);
	  if (buffer[0] > lens2->pos)
	    {
	      draw_line (cr, lens2->pos, buffer[2], buffer[0], buffer[1]);
	      draw_line (cr, lens2->pos, buffer[1], buffer[0], buffer[1]);
              cairo_stroke(cr);
              cairo_set_dash(cr, imgdsh, 2, 0);
              cairo_set_source_rgba (cr, 0, 0.8 , 0.2, 1);
	      draw_varrow (buffer[0], pdat->phys.axis,
			   pdat->phys.axis - buffer[1], 150, cr);
	    }
	}
      cairo_stroke (cr);
    }

  pdat->ldat.ylen = (fabs (buffer[2] - pdat->phys.axis) > 87.5) ?
    buffer[2] - pdat->phys.axis : 87.5;


  //lentes esquemáticas
  if (!pdat->flg.ltype)
    {
      cairo_set_line_width (cr, XWID);
      cairo_set_dash (cr, nodash, 0, 0);

      cairo_set_source_rgb (cr, 1, 0.55, 0);
      draw_vspear (pdat->phys.c.pos, pdat->phys.axis, pdat->ldat.ylen,
		   pdat->phys.c.focus, cr);
      pdat->ldat.headwid1 = draw_vspear (pdat->phys.c.pos,
					 pdat->phys.axis, -pdat->ldat.ylen,
					 pdat->phys.c.focus, cr);

      cairo_arc (cr, pdat->phys.c.pos + pdat->phys.c.focus, pdat->phys.axis,
		 XWID * 1.5, 0, 2. * M_PI);
      cairo_fill (cr);

      cairo_set_source_rgb (cr, 0.21, 0.21, 1);
      draw_vspear (pdat->phys.d.pos, pdat->phys.axis, pdat->ldat.ylen,
		   pdat->phys.d.focus, cr);
      pdat->ldat.headwid2 = draw_vspear (pdat->phys.d.pos,
					 pdat->phys.axis, -pdat->ldat.ylen,
					 pdat->phys.d.focus, cr);

      cairo_arc (cr, pdat->phys.d.pos - pdat->phys.d.focus, pdat->phys.axis,
		 XWID * 1.5, 0, 2. * M_PI);
      cairo_fill (cr);
    }
  //lentes desenhadas
  else
    {
      //Modo "desenhadas"
      cairo_set_source_rgba (cr, 0.75, 0.70, 0.55, 0.6);

      buffer[3] = 2 * *pdat->lnsc.focus + pdat->ldat.ylen * 2 + 200;
      buffer[2] = sqrt (buffer[3] * buffer[3] -
			(pdat->ldat.ylen) * (pdat->ldat.ylen));

      buffer[4] = asin ((pdat->ldat.ylen) / (buffer[3]));

      cairo_arc (cr, pdat->phys.c.pos + buffer[2], pdat->phys.axis,
		 buffer[3], M_PI - buffer[4], M_PI + buffer[4]);
      cairo_arc (cr, pdat->phys.c.pos - buffer[2], pdat->phys.axis,
		 buffer[3], -buffer[4], +buffer[4]);
      cairo_fill (cr);

      cairo_arc (cr, pdat->phys.c.pos + pdat->phys.c.focus,
		 pdat->phys.axis, XWID * 1.5, 0, 2. * M_PI);
      cairo_fill (cr);

      cairo_set_source_rgba (cr, 0.50, 0.50, 0.65, 0.6);

      buffer[3] = -2 * *pdat->lnsd.focus + pdat->ldat.ylen * 2 + 200;
      buffer[2] = sqrt (buffer[3] * buffer[3] -
			(pdat->ldat.ylen) * (pdat->ldat.ylen));

      buffer[4] = asin ((pdat->ldat.ylen) / (buffer[3]));
      pdat->ldat.headwid2 = 1.5 * (buffer[3] - buffer[2]);

      cairo_arc (cr, pdat->phys.d.pos + buffer[2] + pdat->ldat.headwid2,
		 pdat->phys.axis, buffer[3], M_PI - buffer[4],
		 M_PI + buffer[4]);

      cairo_arc (cr, pdat->phys.d.pos - buffer[2] - pdat->ldat.headwid2,
		 pdat->phys.axis, buffer[3], -buffer[4], +buffer[4]);

      cairo_fill (cr);

      cairo_arc (cr, pdat->phys.d.pos - pdat->phys.d.focus,
		 pdat->phys.axis, XWID * 1.5, 0, 2. * M_PI);
      cairo_fill (cr);
    }
  cairo_set_line_width (cr, RAY);
  buffer[3] = (buffer[0] - lens2->pos);	//difrença entre fc(l1) e pos(l2)
  buffer[0] = (buffer[1] - pdat->phys.axis) / buffer[3];	//declive do raio eixo lente2

  buffer[2] = (lens2->focus + buffer[3]);
  if (buffer[2] == 0 )
      buffer[2] = 0.1;
  buffer[2] = lens2->pos + (buffer[3] * lens2->focus) / buffer[2];
  //x img2

  buffer[3] = buffer[0] * (buffer[2] - lens2->pos) + pdat->phys.axis;	//y img2
  buffer[4] = *wwidth - lens2->pos;

  if (pdat->flg.virt)
    {
      cairo_set_dash (cr, dash, 1, 0);
      cairo_set_source_rgba (cr, 0., 0.7, 1, 1);
      if (buffer[2] < lens2->pos)
	{
	  draw_line (cr, lens2->pos, pdat->phys.axis, buffer[2], buffer[3]);
	  draw_line (cr, lens2->pos, buffer[1], buffer[2], buffer[3]);
          cairo_stroke(cr);
          cairo_set_dash(cr, imgdsh, 2, 0);
          cairo_set_source_rgba (cr, 0.4, 0.8 , 0.2, 1.);
	  draw_varrow (buffer[2], pdat->phys.axis,
		       pdat->phys.axis - buffer[3], 150, cr);
	}
      cairo_stroke (cr);
    }

  cairo_set_dash (cr, nodash, 0, 0);
  cairo_set_source_rgba (cr, 1., 1., 0., 1.);

  draw_line (cr, lens2->pos, pdat->phys.axis, *wwidth,
	     buffer[0] * buffer[4] + pdat->phys.axis);
  draw_line (cr, lens2->pos, buffer[1], *wwidth,
	     (pdat->phys.axis - buffer[1]) / lens2->focus * buffer[4] +
	     buffer[1]);

  cairo_stroke (cr);

  cairo_destroy (cr);
  pdat->phys.d.focus = -pdat->phys.d.focus;

  return FALSE;
}
