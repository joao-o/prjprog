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
  if (!pdat->btnbarra.state) 
    {
      sprintf (pdat->val + 7, "%.3f", (GTK_ADJUSTMENT (pdat->adj))->value);
      gtk_label_set_text (GTK_LABEL (pdat->lbl), pdat->val);
    } 
  else 
    (GTK_ADJUSTMENT (pdat->adj))->value = pdat->adjsave;
  return TRUE;
}

gboolean
set_val (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  if (!pdat->btnbarra.state) 
    {
      (GTK_ADJUSTMENT (pdat->adj))->value = 0.;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->adj), "changed");
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->adj), "value-changed");
    }
  return TRUE;
}

//Função associada a butão "lock"
gboolean
lchange (GtkWidget * widget, gpointer dat)
{
  tbtn *btnbarra;
  progdata *pdat;
  pdat = (progdata *) dat;
  btnbarra = &pdat->btnbarra;
  if (btnbarra->state == 0)
    {
      sprintf (btnbarra->label, "  Locked  ");
      gtk_button_set_label (GTK_BUTTON (btnbarra->name), btnbarra->label);
      btnbarra->state = 1;
      pdat->adjsave = (GTK_ADJUSTMENT (pdat->adj))->value;
      printf("%f\n",pdat->adjsave);
    }
  else if (btnbarra->state == 1)
    {
      sprintf (btnbarra->label, " Unlocked ");
      gtk_button_set_label (GTK_BUTTON (btnbarra->name), btnbarra->label);
      btnbarra->state = 0;
    }
  return TRUE;
}
