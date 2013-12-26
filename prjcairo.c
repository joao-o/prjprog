#include <cairo.h>
#include <gtk/gtk.h>
#include <structs.h>
#include <stdio.h>
#include <phys.h>
#include <math.h>

// é na realidade uma CSR para espose events está aqui porque desenha principalmente

gboolean
expose_ev (GtkWidget * widget,GdkEventExpose *event, gpointer dat)
{
  progdata *pdat;
  cairo_t *cr;
  int width, height;
  double pos1, pos2, pos3, pos4, pos5, fc, fd, hmid, hfin;
  double ylen, xwid, hwid1, hwid2;
  
  pdat = (progdata*) dat;

  ylen = pdat->lensdata.ylen;
  xwid = pdat->lensdata.xwid;
  hwid1 = pdat->lensdata.headwid1;
  hwid2 = pdat->lensdata.headwid2;

  gtk_window_get_size (GTK_WINDOW (pdat->window), &width, &height);

  pos1 = gtk_adjustment_get_value (GTK_ADJUSTMENT (pdat->barl.adj));

  if (pos1 > pdat->drawbox->allocation.width)
    {
      pos1 = pdat->drawbox->allocation.width;
      gtk_adjustment_set_value (GTK_ADJUSTMENT (pdat->barl.adj), pos1);
    }
  gtk_adjustment_set_upper (GTK_ADJUSTMENT (pdat->barl.adj),
			    pdat->drawbox->allocation.width);

  pos3 = gtk_adjustment_get_value (GTK_ADJUSTMENT (pdat->barr.adj));

  if (pos3 > pdat->drawbox->allocation.width)
    {
      pos3 = pdat->drawbox->allocation.width;
      gtk_adjustment_set_value (GTK_ADJUSTMENT (pdat->barr.adj), pos3);
    }
  gtk_adjustment_set_upper (GTK_ADJUSTMENT (pdat->barr.adj),
			    pdat->drawbox->allocation.width);

  cr = gdk_cairo_create (pdat->window->window);

  cairo_set_source_rgb (cr, 1., 1., 1.);
  cairo_set_line_width (cr, 2);
  cairo_move_to (cr, 0, 3. * pdat->drawbox->allocation.height / 5.);
  cairo_line_to (cr, pdat->drawbox->allocation.width,
		 3. * pdat->drawbox->allocation.height / 5.);
  cairo_stroke (cr);

  cairo_set_source_rgb (cr, 1, 0.55, 0);

  cairo_set_line_width (cr, xwid);
  pos2 = 3. * pdat->drawbox->allocation.height / 5.;
  cairo_move_to (cr, pos1, pos2 - 3);
  cairo_line_to (cr, pos1, pos2 - ylen);
  cairo_stroke (cr);

  cairo_move_to (cr, pos1 - hwid1, pos2 - ylen);
  cairo_line_to (cr, pos1 + hwid1, pos2 - ylen);
  cairo_line_to (cr, pos1, pos2 - ylen -15);
  cairo_line_to (cr, pos1 - hwid1, pos2 - ylen);
  cairo_line_to (cr, pos1 + hwid1, pos2 - ylen);

  cairo_move_to (cr, pos1 - hwid1, pos2 - 15);
  cairo_line_to (cr, pos1 + hwid1, pos2 - 15);
  cairo_line_to (cr, pos1, pos2);
  cairo_line_to (cr, pos1 - hwid1, pos2 -15);
  cairo_line_to (cr, pos1 + hwid1, pos2 -15);

  cairo_fill (cr);
  cairo_stroke (cr);

  cairo_set_source_rgb (cr, 0.21, 0.21, 1);

  cairo_set_line_width (cr, xwid);
  cairo_move_to (cr, pos3, pos2);
  cairo_line_to (cr, pos3, pos2 - ylen -13);
  cairo_stroke (cr);

  cairo_move_to (cr, pos3 - hwid2, pos2 - ylen - 15);
  cairo_line_to (cr, pos3 + hwid2, pos2 - ylen - 15);
  cairo_line_to (cr, pos3, pos2 - ylen);
  cairo_line_to (cr, pos3 - hwid2, pos2 - ylen -15);
  cairo_line_to (cr, pos3 + hwid2, pos2 - ylen -15);

  cairo_move_to (cr, pos3 - hwid2, pos2);
  cairo_line_to (cr, pos3 + hwid2, pos2);
  cairo_line_to (cr, pos3, pos2 - 15);
  cairo_line_to (cr, pos3 - hwid2, pos2);
  cairo_line_to (cr, pos3 + hwid2, pos2);
  cairo_fill (cr);
  cairo_stroke (cr);


  // Desenho das imagens (alpha) 

  fd = gtk_adjustment_get_value (GTK_ADJUSTMENT (pdat->barfd.adj));
  fc = gtk_adjustment_get_value (GTK_ADJUSTMENT (pdat->barfc.adj));


  // Os sinais precisam de ser revistos
  if(pos3 < pos1)
    {
      pos4 = - fd + pos3;
      pos5 = pos1 + fconj(-fc,- fabs(pos1 - pos4));
    }
  else
    {
      pos4 = fc + pos1;
      pos5 = pos3 - fconj(fd, (pos3 - pos4));
    }

  if (pos4 > pdat->drawbox->allocation.width)
    pos4 = pdat->drawbox->allocation.width;

  if (pos5 > pdat->drawbox->allocation.width)
    pos5 = pdat->drawbox->allocation.width;

  cairo_set_source_rgb (cr, 0.44, 1.00, 0.22);

  cairo_set_line_width (cr, xwid/2);
  cairo_move_to (cr, pos4, pos2);
  cairo_line_to (cr, pos4, pos2 - ylen -13);
  cairo_stroke (cr);

  cairo_set_source_rgb (cr, 0.55, 0.00, 0.55);

  cairo_set_line_width (cr, xwid/2);
  cairo_move_to (cr, pos5, pos2);
  cairo_line_to (cr, pos5, pos2 - ylen -13);
  cairo_stroke (cr);

  // prototipo raios
  cairo_set_source_rgb (cr, 0.90, 0.90, 0.00);
  if (pos1 < pos3)
    {
      cairo_set_line_width (cr, xwid/3);
      cairo_move_to (cr, 0, pos2 - ylen - 15);
      cairo_line_to (cr, pos1, pos2 - ylen - 15);
      cairo_stroke (cr);

      cairo_set_line_width (cr, xwid/3);
      cairo_move_to (cr, 0, pos2);
      cairo_line_to (cr, pos1, pos2);
      cairo_stroke (cr);

      cairo_set_line_width (cr, xwid/3);
      cairo_move_to (cr, pos1, pos2 - ylen - 15);
      cairo_line_to (cr, pos4, pos2 - (ylen +15)/2 );
      cairo_stroke (cr);

      cairo_set_line_width (cr, xwid/3);
      cairo_move_to (cr, pos1, pos2);
      cairo_line_to (cr, pos4, pos2 - (ylen +15)/2 );
      cairo_stroke (cr);

      cairo_set_line_width (cr, xwid/3);
      cairo_move_to (cr, pos4, pos2 - (ylen +15)/2 );
      cairo_line_to (cr, pos3, pos2 - ( ( (ylen +15)/2 ) * ( (pos3-pos1)/(pos4-pos1) ) ) );
      cairo_stroke (cr);
      
      cairo_set_line_width (cr, xwid/3);
      cairo_move_to (cr, pos4, pos2 - (ylen +15)/2 );
      cairo_line_to (cr, pos3, pos2 + ( ( (ylen +15)/2 ) * ( (pos3-pos4)/(pos4-pos1) -1 ) ) );
      cairo_stroke (cr);

    }

  cairo_destroy (cr);
  return FALSE;
}
