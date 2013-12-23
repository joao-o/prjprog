#include <cairo.h>
#include <gtk/gtk.h>
#include <structs.h>


void obj_draw()
{
  cairo_surface_t *surface;
  cairo_t *cr;
}

gboolean
on_expose_event (GtkWidget       *widget ,
                 GdkEventExpose  *event  ,
                 gpointer         data   )
{
  progdata  *pdat            ;
  cairo_t   *cr               ;
  gint       width, height    ;
  double     pos1, pos2       ;

  pdat = (progdata *) data;

  gtk_window_get_size (GTK_WINDOW(pdat->window), &width, &height);

  pos1 = gtk_adjustment_get_value (GTK_ADJUSTMENT(pdat->barl.adj));
  if (pos1 > pdat->drawbox->allocation.width)
    {
      pos1 = pdat->drawbox->allocation.width;
      gtk_adjustment_set_value (GTK_ADJUSTMENT(pdat->barl.adj), pos1);
    }
  gtk_adjustment_set_upper (GTK_ADJUSTMENT(pdat->barl.adj), pdat->drawbox->allocation.width);

  cr = gdk_cairo_create (pdat->window->window);

  cairo_set_source_rgb (cr, 1., 1., 1.);
  cairo_set_line_width (cr, 2);
  cairo_move_to (cr, 0, 3. * pdat->drawbox->allocation.height / 4.);
  cairo_line_to (cr, pdat->drawbox->allocation.width, 3. * pdat->drawbox->allocation.height / 4.);
  cairo_stroke (cr);

  cairo_set_source_rgb (cr, 1, 0.55, 0);
  
  cairo_set_line_width (cr, 5);
  pos2 = 3. * pdat->drawbox->allocation.height / 4.;
  cairo_move_to (cr, pos1, pos2);
  cairo_line_to (cr, pos1, pos2 - 40);
  cairo_stroke (cr);

  cairo_move_to(cr, pos1 - 7, pos2 - 40);
  cairo_line_to(cr, pos1 + 7, pos2 - 40);
  cairo_line_to(cr, pos1, pos2 - 55);
  cairo_line_to(cr, pos1 - 7, pos2 - 40);
  cairo_line_to(cr, pos1 + 7, pos2 - 40);
  cairo_fill(cr);
  cairo_stroke(cr);

  cairo_destroy (cr);

  return FALSE;
}

gboolean 
change_scale (GtkWidget  *widget,
              gpointer    data )
{
  progdata  *pdat ;

  pdat = (progdata *) data;
  gtk_widget_queue_draw (pdat->window);

  return TRUE;
}

gboolean
on_configure_event (GtkWidget       *widget ,
                    GdkEventExpose  *event  ,
                    gpointer         data   )
{
  change_scale (widget, data);
  return FALSE;
}
