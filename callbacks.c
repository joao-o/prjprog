#include <gtk/gtk.h>
#include <cairo.h>

#include <structs.h>
#include <prjcairo.h>
#include <phys.h>
#include <miscui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define L_VAL 14

// ficheiro com as CSR (callback service routines)

// CSR de "configure events" (as dimensões da janela mudaram)

gboolean
cfg_event (GtkWidget * widget, GdkEventExpose * event, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;

  gtk_widget_queue_draw (pdat->window);
  return FALSE;
}

// callback que muda coisas quando os ajust mudam (lockable)

void
upd_mod (barradat * barra)
{
  sprintf (barra->str + 7, "%5.1f", (GTK_ADJUSTMENT (barra->adj))->value);
  gtk_label_set_text (GTK_LABEL (barra->lbl), barra->str);
  barra->save = (GTK_ADJUSTMENT (barra->adj))->value;
  return;
}

gboolean
upd_adj (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  barradat *barra, *barra2;
  double d;
  pdat = (progdata *) dat;

  if (GTK_OBJECT (widget) == pdat->barl.adj)
    barra = &pdat->barl;
  else
    barra = &pdat->barr;

  if (!pdat->btnlock.name->state)
    {
      if(pdat->flg.dist)
	{
	  if (GTK_OBJECT (widget) == pdat->barl.adj)
	    barra2 = &pdat->barr;
	  else
	    barra2 = &pdat->barl;
	  
	  d= GTK_ADJUSTMENT(barra2->adj)->value - barra->save;
	  GTK_ADJUSTMENT(barra2->adj)->value = 
	    GTK_ADJUSTMENT(barra->adj)->value + d;

	  upd_mod(barra2);
	}

      upd_mod (barra);
      gtk_widget_queue_draw (pdat->window);
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
  int l=L_VAL;


  if (GTK_OBJECT (widget) == pdat->barfc.adj)
      barra = &pdat->barfc;
  else if (GTK_OBJECT (widget) == pdat->barfd.adj)
      barra = &pdat->barfd;
  else if (GTK_OBJECT (widget) == pdat->barang.adj)
    {
      barra = &pdat->barang;
      l = 8;
    }
  else if (GTK_OBJECT (widget) == pdat->barxx.adj)
    {
      barra = &pdat->barxx;
      l = 2;
    }

  if (l == L_VAL && pdat->btnlock.name->state 
      && *pdat->lnsc.focus > *pdat->lnsd.focus+20)
    {
      *pdat->lnsd.pos = *pdat->lnsc.pos +
	*pdat->lnsc.focus - *pdat->lnsd.focus - 5;
      upd_mod(&(pdat->barr));
    }
  else if(pdat->btnlock.name->state)
    (GTK_ADJUSTMENT (barra->adj))->value = barra->save; 

  sprintf (barra->str + l, "%4.1f", (GTK_ADJUSTMENT (barra->adj))->value);
  gtk_label_set_text (GTK_LABEL (barra->lbl), barra->str);

  gtk_widget_queue_draw (pdat->window);
  barra->save = (GTK_ADJUSTMENT (barra->adj))->value;

  return TRUE;
}

//CSR do butão reset 

gboolean
set_val (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  if (!pdat->btnlock.name->state)
    {
      (GTK_ADJUSTMENT (pdat->barl.adj))->value = 100.;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
			     "value-changed");
      *(pdat->lnsc.focus) = 300;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfc.adj),
			     "value-changed");
      *(pdat->lnsd.focus) = 149.97;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj),
			     "value-changed");
      *(pdat->lnsd.pos) = 250.;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
			     "value-changed");
    }
  return TRUE;
}

gboolean
luneta (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  if (*pdat->lnsc.focus < *pdat->lnsd.focus)
    erroluneta (pdat);
  else
    {
      *pdat->lnsd.pos = *pdat->lnsc.pos +
	(*pdat->lnsc.focus - *pdat->lnsd.focus - 5);
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

  if (!btnlock->name->state)
    {
      sprintf (btnlock->label, "  Locked  ");
      gtk_button_set_label (GTK_BUTTON (btnlock->name), btnlock->label);
      pdat->barl.save = (GTK_ADJUSTMENT (pdat->barl.adj))->value;
      pdat->barr.save = (GTK_ADJUSTMENT (pdat->barr.adj))->value;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj), "value-changed"); 
    }
  else
    {
      sprintf (btnlock->label, " Unlocked ");
      gtk_button_set_label (GTK_BUTTON (btnlock->name), btnlock->label);
    }

  return TRUE;
}

gboolean
virtchange (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  pdat->flg.virt = !pdat->flg.virt;
  gtk_widget_queue_draw (pdat->window);
  return TRUE;
}

//ainda só funciona no rato
gboolean
distchange (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  pdat->flg.dist = !pdat->flg.dist;
  return TRUE;
}


gboolean
scalechange (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
 
  if( gtk_adjustment_get_upper(GTK_ADJUSTMENT (pdat->barxx.adj)) < 2)
    {
      gtk_adjustment_configure(GTK_ADJUSTMENT (pdat->barxx.adj),
			       1.1, 1.0, 100, 0.1, 0, 0);

      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barxx.adj),
			     "value-changed");
    }
  else  if( gtk_adjustment_get_upper(GTK_ADJUSTMENT (pdat->barxx.adj)) > 2)
    {
      gtk_adjustment_configure(GTK_ADJUSTMENT (pdat->barxx.adj),
			       0.9, 0.0, 1.00, 0.001, 0, 0);
  
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barxx.adj),
			     "value-changed"); 
    }

  return TRUE;
}

//callback quando rato é usado para mexer coisas

gboolean
titanmouse (GtkWidget * widget, GdkEvent * event, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  double poslc, posld, posfc, posfd, axis, ldist, scl;

  scl = GTK_ADJUSTMENT (pdat->barxx.adj)->value;
  poslc = GTK_ADJUSTMENT (pdat->barl.adj)->value/scl;
  posfc = *pdat->lnsc.focus/scl;
  posld = GTK_ADJUSTMENT (pdat->barr.adj)->value/scl;
  posfd = *pdat->lnsd.focus/scl;
  axis = 3. * pdat->drawbox->allocation.height / 5.;
  ldist = poslc - posld;

  /*
    Prioridade do rato:
    D Focal > Lente (Lente é maior)
    Conv > Div (Arbitrário)
  */

  if (event->type == GDK_MOTION_NOTIFY)
    {
      pdat->mouse.nestx = ((GdkEventMotion *) event)->x;
      pdat->mouse.nesty = ((GdkEventMotion *) event)->y;
      
      if(pdat->mouse.trap == 1 )
	{
	  (GTK_ADJUSTMENT (pdat->barl.adj))->value =
	    (pdat->mouse.nestx + pdat->mouse.path1)*scl;
	 
	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
				 "value-changed");
	  if(pdat->flg.dist)
	    {
	      (GTK_ADJUSTMENT (pdat->barr.adj))->value =
		(GTK_ADJUSTMENT (pdat->barl.adj))->value - ldist*scl;
	 
	      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
				     "value-changed");
	    }
	}
      else if(pdat->mouse.trap == 2 )
	{
	  (GTK_ADJUSTMENT (pdat->barr.adj))->value =
	    (pdat->mouse.nestx + pdat->mouse.path2)*scl;

	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
				 "value-changed");

	  if(pdat->flg.dist)
	    {
	      (GTK_ADJUSTMENT (pdat->barl.adj))->value =
		(GTK_ADJUSTMENT (pdat->barr.adj))->value + ldist*scl;
	 
	      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
				     "value-changed");
	    }
	}
      else if(pdat->mouse.trap == 3 )
	{
	  (GTK_ADJUSTMENT (pdat->barfc.adj))->value =
	    (pdat->mouse.nestx + pdat->mouse.path1)*scl;
	 
	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfc.adj),
				 "value-changed");
	}
      else if(pdat->mouse.trap == 4 )
	{
	  (GTK_ADJUSTMENT (pdat->barfd.adj))->value =
	    (pdat->mouse.nestx + pdat->mouse.path1)*scl;
	 
	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj),
				 "value-changed");
	}

    }
  else if (event->type == GDK_BUTTON_PRESS)
    {
      
      pdat->mouse.nestx = ((GdkEventButton *) event)->x;
      pdat->mouse.nesty = ((GdkEventMotion *) event)->y;
   
      if(((pdat->mouse.nestx - poslc) 
	  < posfc + pdat->lensdata.xwid*1.5)
	 && ((pdat->mouse.nestx - poslc) 
	     > posfc - pdat->lensdata.xwid*1.5)
	 && (fabs(pdat->mouse.nesty - axis) 
	     < pdat->lensdata.xwid*1.5))
	{
	  pdat->mouse.trap = 3; 
	  pdat->mouse.path1 =
	    (GTK_ADJUSTMENT (pdat->barfc.adj))->value/scl - pdat->mouse.nestx;
	  (GTK_ADJUSTMENT (pdat->barfc.adj))->value =
	    (pdat->mouse.nestx + pdat->mouse.path1)*scl;

	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfc.adj),
				 "value-changed");
	}
      else if(((pdat->mouse.nestx - posld) 
	       < posfd + pdat->lensdata.xwid*1.5)
	      && ((pdat->mouse.nestx - posld) 
		  > posfd - pdat->lensdata.xwid*1.5)
	      && (fabs(pdat->mouse.nesty - axis) 
		  < pdat->lensdata.xwid*1.5))
	{
	  pdat->mouse.trap = 4;
	  pdat->mouse.path1 =
	    (GTK_ADJUSTMENT (pdat->barfd.adj))->value/scl - pdat->mouse.nestx;
	  (GTK_ADJUSTMENT (pdat->barfd.adj))->value =
	    (pdat->mouse.nestx + pdat->mouse.path1)*scl;

	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj),
				 "value-changed");
	}
      else if(fabs(pdat->mouse.nestx - poslc) 
	      < pdat->lensdata.headwid1 
	      && fabs(pdat->mouse.nesty - axis) 
	      < pdat->lensdata.ylen)
	{
	  pdat->mouse.trap = 1;  
	  pdat->mouse.path1 =
	    ((GTK_ADJUSTMENT (pdat->barl.adj))->value)/scl - pdat->mouse.nestx;
	  (GTK_ADJUSTMENT (pdat->barl.adj))->value =
	    (pdat->mouse.nestx + pdat->mouse.path1)*scl;

	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
				 "value-changed");
	}
      else if(fabs(pdat->mouse.nestx - posld)
	      < pdat->lensdata.headwid2 
	      && fabs(pdat->mouse.nesty - axis) 
	      < pdat->lensdata.ylen)
	{
	  pdat->mouse.trap = 2;
	  pdat->mouse.path2 =
	    ((GTK_ADJUSTMENT (pdat->barr.adj))->value)/scl - pdat->mouse.nestx;
	  (GTK_ADJUSTMENT (pdat->barr.adj))->value =
	    (pdat->mouse.nestx + pdat->mouse.path2)*scl;

	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
				 "value-changed");
	}
      
    }
  else if (event->type == GDK_BUTTON_RELEASE)
    pdat->mouse.trap = 0;

  gtk_widget_queue_draw (pdat->window);
  return TRUE;
}
