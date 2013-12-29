#include <gtk/gtk.h>
#include <cairo.h>

#include <structs.h>
#include <prjcairo.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ficheiro com as CSR (callback service routines)

// CSR de "configure events" (as dimensões da janela mudaram)

gboolean
cfg_event (GtkWidget * widget, GdkEventExpose * event, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;

  gtk_widget_queue_draw(pdat->window);
  return FALSE;
}

// callback que muda coisas quando os ajust mudam (lockable)

gboolean
upd_adj (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  barradat *barra;
  pdat = (progdata *) dat;

  if (GTK_OBJECT (widget) == pdat->barl.adj)
    barra = &pdat->barl;
  else
    barra = &pdat->barr;

  if (!pdat->btnlock.state)
    {
      sprintf (barra->str + 7, "%.3f", (GTK_ADJUSTMENT (barra->adj))->value);
      gtk_label_set_text (GTK_LABEL (barra->lbl), barra->str);
      gtk_widget_queue_draw(pdat->window);
    }
  else
    (GTK_ADJUSTMENT (barra->adj))->value = barra->save;
  return TRUE;
}

// callback que muda coisas quando os ajust mudam (independente do lock)

gboolean
upd_adj_free (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  barradat *barra;
  pdat = (progdata *) dat;
  int l;
  
  if (GTK_OBJECT (widget) == pdat->barfc.adj)
    {
      barra = &pdat->barfc;
      l = 15;
    }
  else if (GTK_OBJECT (widget) == pdat->barfd.adj)
    {
      barra = &pdat->barfd;
      l = 15;
    }
  else if (GTK_OBJECT (widget) == pdat->barang.adj)
    {
      barra = &pdat->barang;
      l = 8;
    }

  sprintf (barra->str + l, "%.3f", (GTK_ADJUSTMENT (barra->adj))->value);
  gtk_label_set_text (GTK_LABEL (barra->lbl), barra->str);
  gtk_widget_queue_draw(pdat->window);

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
      (GTK_ADJUSTMENT (pdat->barl.adj))->value = 25.;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj), "changed");
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
			     "value-changed");

      (GTK_ADJUSTMENT (pdat->barr.adj))->value = 50.;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj), "changed");
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
			     "value-changed");
    }
  return TRUE;
}

gboolean 
erroluneta (progdata * dat)
{
  //mensagem de erro "ocular > objectiva"
  return TRUE;
}

gboolean
luneta (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  if(*pdat->lnsc.focus < fabs(*pdat->lnsd.focus))
    {
      erroluneta(pdat);
    }
  else
    {
     (GTK_ADJUSTMENT (pdat->barr.adj))->value = *pdat->lnsc.pos + 
       (*pdat->lnsc.focus - fabs(*pdat->lnsd.focus) - 5);
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj), 
			     "changed");
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
			     "value-changed");
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
