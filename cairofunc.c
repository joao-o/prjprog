#include <cairo.h>
#include <gtk/gtk.h>
#include <structs.h>
#include <stdio.h>
#include <phys.h>
#include <math.h>

#define A_THK 10.

void draw_varrow(double x, double y, double hgt, double focus ,cairo_t *cr)
{
  double i;
  cairo_set_source_rgb (cr, 1., 0.55, 0.);

  cairo_set_line_width (cr, 2);
  cairo_move_to (cr, x, y);
  cairo_line_to (cr, x, y-hgt);
  cairo_stroke (cr);
  for(i = -A_THK; i <= A_THK; i+=A_THK*2) 
    {
      cairo_move_to(cr,x,y-hgt);
      //encontrar um modelo de jeito para isto.
      //tentar aproximar seno / exponencial (série/modelo linear??).
      cairo_line_to(cr,x+i,y-hgt+A_THK/focus);
      cairo_stroke(cr);
    }
}

void draw_axis(cairo_t *cr,progdata *pdat)
{
    cairo_set_source_rgb(cr,0.,0.,0.);
    cairo_set_line_width(cr , 2);
    cairo_move_to(cr,0,pdat->drawbox->allocation.height/2);
    cairo_line_to (cr, pdat->drawbox->allocation.width,
		   pdat->drawbox->allocation.height/2);
    cairo_stroke(cr);
}

gboolean
expose_evv (GtkWidget * widget,GdkEventExpose *event, gpointer dat)
{
    progdata *pdat;
    cairo_t *cr;

    pdat = (progdata*) dat;
    
    if (pdat->drawbox->allocation.width < (GTK_ADJUSTMENT (pdat->barl.adj))->value )
        ( GTK_ADJUSTMENT (pdat->barl.adj)) ->value = pdat->drawbox->allocation.width;
    ( GTK_ADJUSTMENT (pdat->barl.adj)) ->upper = pdat->drawbox->allocation.width;

    cr=gdk_cairo_create(pdat->drawbox->window);

    draw_axis(cr,pdat);

    draw_varrow(( GTK_ADJUSTMENT (pdat->barl.adj))->value,
              pdat->drawbox->allocation.height/2,
              pdat->lensdata.ylen,
              (GTK_ADJUSTMENT (pdat->barfc.adj))->value,
              cr);

    draw_varrow(( GTK_ADJUSTMENT (pdat->barl.adj))->value,
              pdat->drawbox->allocation.height/2,
              -pdat->lensdata.ylen,
              -(GTK_ADJUSTMENT (pdat->barfc.adj))->value,
              cr);

    cairo_destroy(cr);
    return FALSE;
    
}
