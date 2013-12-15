#include <gtk/gtk.h>
#include <cairo.h>

#include <stdio.h>
#include <stdlib.h>
#include <structs.h>

// ficheiro com as CSR (callback service routines)

// callback que muda os números nas labels quando os ajust mudam
gboolean
upd_txt (GtkWidget *widget, gpointer dat)
{
  progdata *pdat;
  barradat *barra;
  pdat = (progdata*) dat;

  if (GTK_OBJECT(widget) == pdat->barl.adj)
      barra=&pdat->barl;
  else
      barra=&pdat->barr;
    
    
  if (!pdat->btnlock.state) 
    {
      sprintf (barra->str + 7, "%.3f", (GTK_ADJUSTMENT (barra->adj))->value);
      gtk_label_set_text (GTK_LABEL (barra->lbl), barra->str);
    } 
  else 
      (GTK_ADJUSTMENT (barra->adj))->value = barra->save;
  return TRUE;
}

//CSR do butão reset 

gboolean
set_val (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  if (!pdat->btnlock.state) 
    {
      (GTK_ADJUSTMENT (pdat->barl.adj))->value = 0.;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj), "changed");
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj), "value-changed");

      (GTK_ADJUSTMENT (pdat->barr.adj))->value = 0.;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj), "changed");
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj), "value-changed");
    }
  return TRUE;
}

// CSR do butão "lock"

gboolean
lchange (GtkWidget * widget, gpointer dat)
{
  tbtn *btnlock;
  progdata *pdat;
  pdat = (progdata *) dat;
  btnlock = &pdat->btnlock;
  if (!btnlock->state)
    {
      sprintf (btnlock->label, "  Locked  ");
      gtk_button_set_label (GTK_BUTTON (btnlock->name), btnlock->label);
      btnlock->state = 1;
      pdat->barl.save = (GTK_ADJUSTMENT (pdat->barl.adj))->value;
      pdat->barr.save = (GTK_ADJUSTMENT (pdat->barr.adj))->value;
    }
  else 
    {
      sprintf (btnlock->label, " Unlocked ");
      gtk_button_set_label (GTK_BUTTON (btnlock->name), btnlock->label);
      btnlock->state = 0;
    }
  return TRUE;
}
