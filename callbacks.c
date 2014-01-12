#include <gtk/gtk.h>
#include <cairo.h>

#include <structs.h>
#include <miscui.h>
#include <draw.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// ficheiro com as CSR (callback service routines)

// CSR de "configure events" (as dimensões da janela mudaram)



gboolean
upd_phys (progdata * pdat)
{
  pdat->phys.c.pos = *pdat->lnsc.pos / *pdat->phys.scl;
  pdat->phys.c.focus = *pdat->lnsc.focus / *pdat->phys.scl;
  pdat->phys.d.pos = *pdat->lnsd.pos / *pdat->phys.scl;
  pdat->phys.d.focus = *pdat->lnsd.focus / *pdat->phys.scl;

  return TRUE;
}

gboolean
cfg_event (GtkWidget * widget, GdkEventExpose * event, progdata *pdat)
{
  gtk_widget_queue_draw (pdat->window);
  return FALSE;
}

// callback que muda coisas quando os ajust mudam (lockable)

void
upd_mod (bardat * barra, int offset)
{
  if ((GTK_ADJUSTMENT (barra->adj))->value
      > (GTK_ADJUSTMENT (barra->adj))->upper)

    (GTK_ADJUSTMENT (barra->adj))->value =
      (GTK_ADJUSTMENT (barra->adj))->upper;

  if ((GTK_ADJUSTMENT (barra->adj))->value
      < (GTK_ADJUSTMENT (barra->adj))->lower)

    (GTK_ADJUSTMENT (barra->adj))->value =
      (GTK_ADJUSTMENT (barra->adj))->lower;

  sprintf (barra->str + offset, "%5.1f", (GTK_ADJUSTMENT (barra->adj))->value);
  gtk_label_set_text (GTK_LABEL (barra->lbl), barra->str);
  barra->save = (GTK_ADJUSTMENT (barra->adj))->value;
  return;
}

gboolean
upd_adj (GtkWidget * widget, progdata *pdat)
{
  bardat *barra;
  double temp;

  if (GTK_OBJECT (widget) == pdat->barl.adj)
    barra = &pdat->barl;
  else
    barra = &pdat->barr;
  if (!pdat->flg.lock)
    {
      if (pdat->flg.dist)
	{
	  temp = (GTK_ADJUSTMENT (barra->adj))->value + pdat->phys.ldist *
	    dsign (-barra->save + barra->alt->save);
	  /*
	     calcula onde por a outra lente quando "dist" está set
	     o dsign serve para ver a posições relativas
	     ie se soma ou subtrai uso temp pois só quero fazer aquilo^ uma vez
	   */

	  if (temp > TOL &&
	      temp < pdat->drawbox->allocation.width * *pdat->phys.scl - TOL)
	    {
	      GTK_ADJUSTMENT (barra->alt->adj)->value = temp;
	      upd_mod (barra->alt,OFFPOS);
	    }
	  else
	    GTK_ADJUSTMENT (barra->adj)->value = barra->save;
	}

      upd_mod (barra,OFFPOS);
      gtk_widget_queue_draw (pdat->window);
    }
  else
    (GTK_ADJUSTMENT (barra->adj))->value = barra->save;


  upd_phys (pdat);

  return TRUE;
}

// callback que muda coisas quando os ajust mudam (independente do lock)

gboolean
upd_adj_free (GtkWidget * widget, progdata *pdat)
{
  bardat *barra;
  int l = OFFFOC;

  if (GTK_OBJECT (widget) == pdat->barfc.adj)
    barra = &pdat->barfc;
  else if (GTK_OBJECT (widget) == pdat->barfd.adj)
    barra = &pdat->barfd;
  else if (GTK_OBJECT (widget) == pdat->barang.adj)
    {
      barra = &pdat->barang;
      l = OFFANG;
    }
  else if (GTK_OBJECT (widget) == pdat->barxx.adj)
    {
      barra = &pdat->barxx;
      l = OFFSCL;
    }

  if (l == OFFFOC && pdat->flg.lock
      && *pdat->lnsc.focus > *pdat->lnsd.focus + 20)
    {
      *pdat->lnsd.pos = *pdat->lnsc.pos +
	*pdat->lnsc.focus - *pdat->lnsd.focus - 5;
      upd_mod (&(pdat->barr),l);
    }
  else if (pdat->flg.lock && !pdat->flg.dist)
    (GTK_ADJUSTMENT (barra->adj))->value = barra->save;

  upd_mod(barra,l);
  gtk_widget_queue_draw (pdat->window);

  upd_phys (pdat);
  return TRUE;
}

//CSR do butão reset
//Estes valores fazem uma luneta bonitinha
gboolean
set_val (GtkWidget * widget, progdata *pdat)
{
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->btnlock.name),
				FALSE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->virtbtn), TRUE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->distbtn), FALSE);

  *pdat->lnsc.pos = 320.;
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj), "value-changed");
  *(pdat->lnsc.focus) = 157;
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfc.adj), "value-changed");
  *(pdat->lnsd.focus) = 20;
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj), "value-changed");
  *(pdat->lnsd.pos) = 452.;
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj), "value-changed");
  (GTK_ADJUSTMENT (pdat->barang.adj))->value = 20.;
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barang.adj), "value-changed");
  (GTK_ADJUSTMENT (pdat->barxx.adj))->value = 1.;
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barxx.adj), "value-changed");

  gtk_widget_queue_draw (pdat->window);

  return TRUE;
}

gboolean
luneta (GtkWidget * widget, progdata *pdat)
{
  unsigned char t = 0;

  if (pdat->flg.dist)
    {
      pdat->flg.dist = !pdat->flg.dist;
      t = 1;
    }

  if (*pdat->lnsc.focus < *pdat->lnsd.focus)
    erroluneta (pdat);
  else
    {
      if( *pdat->lnsc.pos + (*pdat->lnsc.focus - *pdat->lnsd.focus - 5)
	  > (GTK_ADJUSTMENT (pdat->barr.adj))->upper)
	{
	  *pdat->lnsd.pos = (GTK_ADJUSTMENT (pdat->barr.adj))->upper - 10;
	  *pdat->lnsc.pos = *pdat->lnsd.pos -
	    (*pdat->lnsc.focus - *pdat->lnsd.focus - 5);
	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
				 "value-changed");
	}
      else
	*pdat->lnsd.pos = *pdat->lnsc.pos +
	  (*pdat->lnsc.focus - *pdat->lnsd.focus - 5);
      
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
			     "value-changed");
    }

  if (t == 1)
    {
      pdat->flg.dist = !pdat->flg.dist;
    }
  return TRUE;
}

// CSR do butão "lock"

gboolean
lchange (GtkWidget * widget, progdata *pdat)
{
  tbtn *btnlock;
  btnlock = &pdat->btnlock;

  if (!pdat->flg.lock)
    {
      sprintf (btnlock->label, " Bloqueado  ");
      pdat->flg.lock = !pdat->flg.lock;
      gtk_button_set_label (GTK_BUTTON (btnlock->name), btnlock->label);
      pdat->barl.save = (GTK_ADJUSTMENT (pdat->barl.adj))->value;
      pdat->barr.save = (GTK_ADJUSTMENT (pdat->barr.adj))->value;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj),
			     "value-changed");
    }
  else
    {
      sprintf (btnlock->label, "Desbloqueado");
      pdat->flg.lock = !pdat->flg.lock;
      gtk_button_set_label (GTK_BUTTON (btnlock->name), btnlock->label);
    }

  return TRUE;
}

gboolean
virtchange (GtkWidget * widget, progdata *pdat)
{
  pdat->flg.virt = !pdat->flg.virt;
  gtk_widget_queue_draw (pdat->window);
  return TRUE;
}

//ainda só funciona no rato
gboolean
distchange (GtkWidget * widget, progdata *pdat)
{
  pdat->phys.ldist = fabs (*pdat->lnsc.pos - *pdat->lnsd.pos);
  pdat->flg.dist = !pdat->flg.dist;
  return TRUE;
}

gboolean
typechange (GtkWidget * widget, progdata *pdat)
{
  pdat->flg.ltype = !pdat->flg.ltype;
  gtk_widget_queue_draw (pdat->window);
  return TRUE;
}

gboolean
scalechange (GtkWidget * widget, progdata *pdat)
{
  int *wwidth = &pdat->drawbox->allocation.width;

  if (pdat->flg.dist)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->distbtn), FALSE);

  (GTK_ADJUSTMENT (pdat->barl.adj))->upper =
    (*wwidth - TOL) * *pdat->phys.scl;
  (GTK_ADJUSTMENT (pdat->barr.adj))->upper =
    (*wwidth - TOL) * *pdat->phys.scl;
 
  upd_mod(&pdat->barr,OFFPOS);
  upd_mod(&pdat->barl,OFFPOS);
  upd_phys(pdat);

  upd_mod(&pdat->barxx,OFFSCL);
  gtk_widget_queue_draw (pdat->window);

  return TRUE;
}
    
void
jerrylens (progdata * pdat, bardat * barra)
{
  if (pdat->mouse.nestx + pdat->mouse.path1
      > pdat->drawbox->allocation.width - TOL)

    (GTK_ADJUSTMENT (barra->adj))->value
      = (pdat->drawbox->allocation.width - TOL)
      * GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  else if (pdat->mouse.nestx + pdat->mouse.path1 < TOL)

    (GTK_ADJUSTMENT (barra->adj))->value = (TOL);

  else
    (GTK_ADJUSTMENT (barra->adj))->value =
      (pdat->mouse.nestx + pdat->mouse.path1)
      * GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  g_signal_emit_by_name (GTK_ADJUSTMENT (barra->adj), "value-changed");
}

void
mickeyfocus (progdata * pdat, bardat * barra)
{
  if (pdat->mouse.nestx + pdat->mouse.path1 >
      pdat->drawbox->allocation.width - (TOL))

    (GTK_ADJUSTMENT (barra->adj))->value
      = (pdat->drawbox->allocation.width - (TOL))
      * GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  else if (pdat->mouse.nestx + pdat->mouse.path1 < 10)

    (GTK_ADJUSTMENT (barra->adj))->value
      = 10 * GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  else
    (GTK_ADJUSTMENT (barra->adj))->value =
      (pdat->mouse.nestx + pdat->mouse.path1) *
      GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  if ((GTK_ADJUSTMENT (barra->adj))->value >
      (GTK_ADJUSTMENT (barra->adj))->upper)
    (GTK_ADJUSTMENT (barra->adj))->value =
      (GTK_ADJUSTMENT (barra->adj))->upper;

  g_signal_emit_by_name (GTK_ADJUSTMENT (barra->adj), "value-changed");
}

//callback quando rato é usado para mexer coisas
gboolean
titanmouse (GtkWidget * widget, GdkEvent * event, progdata *pdat)
{

  /*
     Prioridade do rato:
     D Focal > Lente (Lente é maior)
     Conv > Div (Arbitrário)
   */


  pdat->mouse.nestx = ((GdkEventMotion *) event)->x;
  pdat->mouse.nesty = ((GdkEventMotion *) event)->y;

  if (pdat->mouse.nestx > pdat->drawbox->allocation.width
      || pdat->mouse.nestx < 0)
    {
      pdat->mouse.trap = 0;
      return FALSE;
    }
  else if (pdat->mouse.trap != 0 && event->type == GDK_MOTION_NOTIFY)
    {

      switch (pdat->mouse.trap)
	{
	case 1:
	  jerrylens (pdat, &pdat->barl);
	  break;
	case 2:
	  jerrylens (pdat, &pdat->barr);
	  break;
	case 3:
	  mickeyfocus (pdat, &pdat->barfc);
	  break;
	case 4:
	  mickeyfocus (pdat, &pdat->barfd);
	  break;

	}

    }
  else if (event->type == GDK_BUTTON_PRESS)
    {
      if (((pdat->mouse.nestx - pdat->phys.c.pos)
	   < pdat->phys.c.focus + XWID * 1.5)
	  && ((pdat->mouse.nestx - pdat->phys.c.pos)
	      > pdat->phys.c.focus - XWID * 1.5)
	  && (fabs (pdat->mouse.nesty - pdat->phys.axis) < XWID * 1.5))
	{
	  pdat->mouse.trap = 3;
	  pdat->mouse.path1 = pdat->phys.c.focus - pdat->mouse.nestx;
	}
      else if (((pdat->mouse.nestx - pdat->phys.d.pos)
		< pdat->phys.d.focus + XWID * 1.5)
	       && ((pdat->mouse.nestx - pdat->phys.d.pos)
		   > pdat->phys.d.focus - XWID * 1.5)
	       && (fabs (pdat->mouse.nesty - pdat->phys.axis) < XWID * 1.5))
	{
	  pdat->mouse.trap = 4;
	  pdat->mouse.path1 = pdat->phys.d.focus - pdat->mouse.nestx;
	}
      else if (fabs (pdat->mouse.nestx - pdat->phys.c.pos)
	       < pdat->ldat.headwid1
	       && fabs (pdat->mouse.nesty - pdat->phys.axis)
	       < pdat->ldat.ylen)
	{
	  pdat->mouse.trap = 1;
	  pdat->mouse.path1 = pdat->phys.c.pos - pdat->mouse.nestx;
	}
      else if (fabs (pdat->mouse.nestx - pdat->phys.d.pos)
	       < pdat->ldat.headwid2
	       && fabs (pdat->mouse.nesty - pdat->phys.axis)
	       < pdat->ldat.ylen)
	{
	  pdat->mouse.trap = 2;
	  pdat->mouse.path1 = pdat->phys.d.pos - pdat->mouse.nestx;
	}
    }
  else if (event->type == GDK_BUTTON_RELEASE)
    pdat->mouse.trap = 0;


  return TRUE;
}
