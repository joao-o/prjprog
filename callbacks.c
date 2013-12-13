#include <gtk/gtk.h>
#include <cairo.h>

#include <stdio.h>
#include <stdlib.h>
#include <structs.h>

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
