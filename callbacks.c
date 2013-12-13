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
//Função associada a butão "lock"
gboolean
lchange (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  if (pdat->lockind == 0)
    {
      sprintf(pdat->locklabel,"  Locked  ");
      gtk_button_set_label (GTK_BUTTON(pdat->lock), pdat->locklabel);
      gtk_widget_modify_bg (pdat->lock, GTK_STATE_NORMAL, &pdat->color1);
      gtk_widget_modify_bg (pdat->lock, GTK_STATE_PRELIGHT, &pdat->color1);
      gtk_widget_modify_bg (pdat->lock, GTK_STATE_ACTIVE, &pdat->color1);
      pdat->lockind=1;
    }
   else if (pdat->lockind == 1)
    {
      sprintf(pdat->locklabel," Unlocked ");
      gtk_button_set_label (GTK_BUTTON (pdat->lock), pdat->locklabel);
      gtk_widget_modify_bg (pdat->lock, GTK_STATE_NORMAL, &pdat->color2);
      gtk_widget_modify_bg (pdat->lock, GTK_STATE_PRELIGHT, &pdat->color1);
      gtk_widget_modify_bg (pdat->lock, GTK_STATE_ACTIVE, &pdat->color1);
      pdat->lockind=0;
    }
  return TRUE;
}
