#include <gtk/gtk.h>
#include <stdio.h>
#include <cairo.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
  char val[20];
  GtkObject *adj;
  GtkWidget *lbl;
} progdata;

gboolean
upd_txt (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  sprintf (pdat->val + 7, "%.3f", (GTK_ADJUSTMENT (pdat->adj))->value);
  gtk_label_set_text (GTK_LABEL (pdat->lbl), pdat->val);
  return TRUE;
}

gboolean
set_val (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  (GTK_ADJUSTMENT (pdat->adj))->value = 0.;
  g_signal_emit_by_name (GTK_ADJUSTMENT(pdat->adj), "changed");
  g_signal_emit_by_name (GTK_ADJUSTMENT(pdat->adj), "value-changed");
  return TRUE;
}

int
main (int argc, char **argv)
{
  progdata *pdat;

  GtkWidget *window, *button, *bar, *vbox;

  pdat = calloc (1, sizeof (progdata));
  strcpy (pdat->val, "value= ");

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 720, 576);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);

  vbox = gtk_vbox_new (FALSE, 10);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

  pdat->adj = gtk_adjustment_new (0.0, 0.0, 101.0, 0.1, 1.0, 1.0);

  bar = gtk_hscale_new (GTK_ADJUSTMENT (pdat->adj));
  gtk_box_pack_start (GTK_BOX (vbox), bar, TRUE, TRUE, 0);
  gtk_widget_show (bar);

  pdat->lbl = gtk_label_new (pdat->val);
  gtk_box_pack_start (GTK_BOX (vbox), pdat->lbl, TRUE, TRUE, 0);
  gtk_widget_show (pdat->lbl);

  button = gtk_button_new_with_label("reset");
  gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
  gtk_widget_show(button);

  g_signal_connect_swapped (G_OBJECT (window), "destroy",
			    G_CALLBACK (gtk_main_quit), NULL);

  g_signal_connect (G_OBJECT (button), "clicked",
		    G_CALLBACK (set_val), pdat);

  g_signal_connect (G_OBJECT (pdat->adj), "value-changed",
		    G_CALLBACK (upd_txt), pdat);

  gtk_widget_show (window);
  gtk_main ();
  return 0;
}
