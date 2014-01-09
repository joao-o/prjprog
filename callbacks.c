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
upd_phys (progdata *dat)
{
  progdata *pdat = (progdata *) dat; 

  pdat->physdata.poslc = GTK_ADJUSTMENT (pdat->barl.adj)->value
    /GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  pdat->physdata.fc = *pdat->lnsc.focus
    /GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  pdat->physdata.posld = GTK_ADJUSTMENT (pdat->barr.adj)->value
    /GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  pdat->physdata.fd = *pdat->lnsd.focus
    /GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  pdat->physdata.axis = 3. * pdat->drawbox->allocation.height / 5.;

  pdat->physdata.ldist = pdat->physdata.poslc - pdat->physdata.posld;

  return TRUE; 
}

gboolean
cfg_event (GtkWidget * widget, GdkEventExpose * event, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->lenstype), TRUE);
  gtk_widget_queue_draw (pdat->window);
  return FALSE;
}

// callback que muda coisas quando os ajust mudam (lockable)

void
upd_mod (bardat * barra)
{
  if( (GTK_ADJUSTMENT(barra->adj))->value 
      > (GTK_ADJUSTMENT(barra->adj))->upper)
    (GTK_ADJUSTMENT(barra->adj))->value = (GTK_ADJUSTMENT(barra->adj))->upper;
  if( (GTK_ADJUSTMENT(barra->adj))->value 
      < (GTK_ADJUSTMENT(barra->adj))->lower)
    (GTK_ADJUSTMENT(barra->adj))->value = (GTK_ADJUSTMENT(barra->adj))->lower;

  sprintf (barra->str + 7, "%5.1f", (GTK_ADJUSTMENT (barra->adj))->value);
  gtk_label_set_text (GTK_LABEL (barra->lbl), barra->str);
  barra->save = (GTK_ADJUSTMENT (barra->adj))->value;
  return;
}

gboolean
upd_adj (GtkWidget * widget, gpointer dat)
{
  progdata *pdat = (progdata *) dat;
  bardat *barra;
  static double d;

  if (GTK_OBJECT (widget) == pdat->barl.adj)
    barra = &pdat->barl;
  else
    barra = &pdat->barr;
  if (!pdat->flg.lock)
    {
      if(pdat->flg.dist)
	{
	  if (GTK_OBJECT (widget) == pdat->barl.adj)
	      *pdat->lnsd.pos = *pdat->lnsc.pos + d;
          else
	      *pdat->lnsc.pos = *pdat->lnsd.pos - d;
	  upd_mod(barra->alt);
	}

      upd_mod (barra);
      gtk_widget_queue_draw (pdat->window);
    }
  else
    (GTK_ADJUSTMENT (barra->adj))->value = barra->save;

  d = *pdat->lnsd.pos-*pdat->lnsc.pos;
  upd_phys(pdat);
  return TRUE;
}

// callback que muda coisas quando os ajust mudam (independente do lock)

gboolean
upd_adj_free (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  bardat *barra;
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

  if (l == L_VAL && pdat->flg.lock 
      && *pdat->lnsc.focus > *pdat->lnsd.focus+20)
    {
      *pdat->lnsd.pos = *pdat->lnsc.pos +
	*pdat->lnsc.focus - *pdat->lnsd.focus - 5;
      upd_mod(&(pdat->barr));
    }
  else if(pdat->flg.lock)
    (GTK_ADJUSTMENT (barra->adj))->value = barra->save; 

  sprintf (barra->str + l, "%4.1f", (GTK_ADJUSTMENT (barra->adj))->value);
  gtk_label_set_text (GTK_LABEL (barra->lbl), barra->str);

  gtk_widget_queue_draw (pdat->window);
  barra->save = (GTK_ADJUSTMENT (barra->adj))->value;
  
  upd_phys(pdat);
  return TRUE;
}

//CSR do butão reset 

gboolean
set_val (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  if (!pdat->flg.lock)
    {      
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->virtbtn), TRUE);
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->distbtn), FALSE);

      (GTK_ADJUSTMENT (pdat->barl.adj))->value = 320.;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
			     "value-changed");
      *(pdat->lnsc.focus) = 180;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfc.adj),
			     "value-changed");
      *(pdat->lnsd.focus) = 20;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj),
			     "value-changed");
      *(pdat->lnsd.pos) = 477.8;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
			     "value-changed");
      printf("%d\t%d\n",pdat->flg.virt,pdat->flg.dist);
      gtk_widget_queue_draw (pdat->window);

    }
  return TRUE;
}

gboolean
luneta (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  unsigned char t = 0;

  if(pdat->flg.dist)
    {
      pdat->flg.dist=!pdat->flg.dist;
      t = 1;
    }

  if (*pdat->lnsc.focus < *pdat->lnsd.focus)
    erroluneta (pdat);
  else
    {
      *pdat->lnsd.pos = *pdat->lnsc.pos +
	(*pdat->lnsc.focus - *pdat->lnsd.focus - 5);
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj), "value-changed"); 
    }

  if(t == 1)
    {
      pdat->flg.dist=!pdat->flg.dist;
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

  if (!pdat->flg.lock)
    {
      sprintf (btnlock->label, "  Locked  ");
      pdat->flg.lock=!pdat->flg.lock;
      gtk_button_set_label (GTK_BUTTON (btnlock->name), btnlock->label);
      pdat->barl.save = (GTK_ADJUSTMENT (pdat->barl.adj))->value;
      pdat->barr.save = (GTK_ADJUSTMENT (pdat->barr.adj))->value;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj), "value-changed"); 
    }
  else
    {
      sprintf (btnlock->label, " Unlocked ");
      pdat->flg.lock=!pdat->flg.lock;
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
typechange (GtkWidget * widget, gpointer dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  gtk_widget_queue_draw (pdat->window);
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
			       0.9, 0.1, 1.00, 0.001, 0, 0);
  
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
  
  /*
    Prioridade do rato:
    D Focal > Lente (Lente é maior)
    Conv > Div (Arbitrário)
  */

  if (event->type == GDK_MOTION_NOTIFY)
    {
      pdat->mouse.nestx = ((GdkEventMotion *) event)->x;
      pdat->mouse.nesty = ((GdkEventMotion *) event)->y;
      
      if(pdat->mouse.nestx > pdat->drawbox->allocation.width - TOL
	 || pdat->mouse.nestx < TOL)
	return FALSE;
      
      if(pdat->mouse.trap == 1 )
	{
	  if(pdat->mouse.nestx + pdat->mouse.path1 
	     > pdat->drawbox->allocation.width - TOL)

	    (GTK_ADJUSTMENT (pdat->barl.adj))->value 
	      = (pdat->drawbox->allocation.width - TOL)
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  else if(pdat->mouse.nestx + pdat->mouse.path1 - pdat->physdata.ldist 
		  > pdat->drawbox->allocation.width - TOL 
		  && pdat->flg.dist)

	    (GTK_ADJUSTMENT (pdat->barl.adj))->value 
	      = (pdat->drawbox->allocation.width - TOL 
		 + pdat->physdata.ldist)
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	  
	  else if(pdat->mouse.nestx + pdat->mouse.path1 < TOL)
	    (GTK_ADJUSTMENT (pdat->barl.adj))->value 
	      = (TOL);

	  else if (pdat->mouse.nestx + pdat->mouse.path1 - pdat->physdata.ldist 
		  < (TOL) && pdat->flg.dist)
	     (GTK_ADJUSTMENT (pdat->barl.adj))->value 
	       = ((TOL) + pdat->physdata.ldist)
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  else

	    (GTK_ADJUSTMENT (pdat->barl.adj))->value =
	      (pdat->mouse.nestx + pdat->mouse.path1)
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	 
	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
				 "value-changed");
	  if(pdat->flg.dist)
	    {
	      (GTK_ADJUSTMENT (pdat->barr.adj))->value =
		(GTK_ADJUSTMENT (pdat->barl.adj))->value - pdat->physdata.ldist
		*GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	 
	      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
				     "value-changed");
	    }
	}
      else if(pdat->mouse.trap == 2 )
	{
	  if(pdat->mouse.nestx + pdat->mouse.path1 
	     > pdat->drawbox->allocation.width - (TOL))

	    (GTK_ADJUSTMENT (pdat->barr.adj))->value = 
	      (pdat->drawbox->allocation.width - (TOL))
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  else if(pdat->mouse.nestx + pdat->mouse.path1 < (TOL))
	    (GTK_ADJUSTMENT (pdat->barr.adj))->value 
	      = (TOL);

	  else if (pdat->mouse.nestx + pdat->mouse.path1 + pdat->physdata.ldist 
		   < (TOL) && pdat->flg.dist)
	    (GTK_ADJUSTMENT (pdat->barr.adj))->value 
	      = ((TOL) - pdat->physdata.ldist)
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  else if(pdat->mouse.nestx + pdat->mouse.path1 
		  + pdat->physdata.ldist 
		  > pdat->drawbox->allocation.width - (TOL))

	    (GTK_ADJUSTMENT (pdat->barr.adj))->value 
	      = (pdat->drawbox->allocation.width - (TOL) 
		 - pdat->physdata.ldist)
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  else

	    (GTK_ADJUSTMENT (pdat->barr.adj))->value =
	      (pdat->mouse.nestx + pdat->mouse.path2)
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
				 "value-changed");

	  if(pdat->flg.dist)
	    {
	      (GTK_ADJUSTMENT (pdat->barl.adj))->value =
		(GTK_ADJUSTMENT (pdat->barr.adj))->value 
		+ pdat->physdata.ldist*GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	 
	      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
				     "value-changed");
	    }
	}
      else if(pdat->mouse.trap == 3 )
	{
	  if(pdat->mouse.nestx + pdat->mouse.path1 >
	     pdat->drawbox->allocation.width - (TOL))

	    (GTK_ADJUSTMENT (pdat->barfc.adj))->value 
	      = (pdat->drawbox->allocation.width - (TOL))
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	  
	  else if(pdat->mouse.nestx + pdat->mouse.path1
		  < 10)
	  
	    (GTK_ADJUSTMENT (pdat->barfc.adj))->value 
	      = 10*GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	  
	  else

	    (GTK_ADJUSTMENT (pdat->barfc.adj))->value =
	      (pdat->mouse.nestx + pdat->mouse.path1)*
	      GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	 
	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfc.adj),
				 "value-changed");
	}
      else if(pdat->mouse.trap == 4 )
	{
	  if(pdat->mouse.nestx + pdat->mouse.path1 
	     > pdat->drawbox->allocation.width - (TOL))

	    (GTK_ADJUSTMENT (pdat->barfd.adj))->value 
	      = (pdat->drawbox->allocation.width - (TOL))
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	  
	  else if(pdat->mouse.nestx + pdat->mouse.path1
		  < 10)

	    (GTK_ADJUSTMENT (pdat->barfd.adj))->value 
	    = 10*GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  else

	    (GTK_ADJUSTMENT (pdat->barfd.adj))->value =
	      (pdat->mouse.nestx + pdat->mouse.path1)
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	 
	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj),
				 "value-changed");
	}

    }
  else if (event->type == GDK_BUTTON_PRESS)
    {
      
      pdat->mouse.nestx = ((GdkEventButton *) event)->x;
      pdat->mouse.nesty = ((GdkEventMotion *) event)->y;
   
      if(pdat->mouse.nestx > pdat->drawbox->allocation.width - TOL
	 || pdat->mouse.nestx < TOL)
	return FALSE;

      if(((pdat->mouse.nestx - pdat->physdata.poslc) 
	  < pdat->physdata.fc + pdat->lensdata.xwid*1.5)
	 && ((pdat->mouse.nestx - pdat->physdata.poslc) 
	     > pdat->physdata.fc - pdat->lensdata.xwid*1.5)
	 && (fabs(pdat->mouse.nesty - pdat->physdata.axis) 
	     < pdat->lensdata.xwid*1.5))
	{
	  pdat->mouse.trap = 3; 
	  pdat->mouse.path1 =
	    (GTK_ADJUSTMENT (pdat->barfc.adj))->value
	    /GTK_ADJUSTMENT (pdat->barxx.adj)->value - pdat->mouse.nestx;
	  /*
	  if(pdat->mouse.nestx + pdat->mouse.path1
	     < pdat->physdata.poslc)

	    (GTK_ADJUSTMENT (pdat->barfd.adj))->value 
	      = 1;

	  else
	    (GTK_ADJUSTMENT (pdat->barfc.adj))->value =
	      (pdat->mouse.nestx + pdat->mouse.path1)
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfc.adj),
	  "value-changed"); */
	}
      else if(((pdat->mouse.nestx - pdat->physdata.posld) 
	       < pdat->physdata.fd + pdat->lensdata.xwid*1.5)
	      && ((pdat->mouse.nestx - pdat->physdata.posld) 
		  > pdat->physdata.fd - pdat->lensdata.xwid*1.5)
	      && (fabs(pdat->mouse.nesty - pdat->physdata.axis) 
		  < pdat->lensdata.xwid*1.5))
	{
	  pdat->mouse.trap = 4;
	  pdat->mouse.path1 =
	    (GTK_ADJUSTMENT (pdat->barfd.adj))->value
	    /GTK_ADJUSTMENT (pdat->barxx.adj)->value - pdat->mouse.nestx;
	  /*
	  if(pdat->mouse.nestx + pdat->mouse.path1
	     < pdat->physdata.posld)

	    (GTK_ADJUSTMENT (pdat->barfd.adj))->value 
	      = 1;

	  else
	    (GTK_ADJUSTMENT (pdat->barfd.adj))->value =
	      (pdat->mouse.nestx + pdat->mouse.path1)
	      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj),
	  "value-changed");*/
	}
      else if(fabs(pdat->mouse.nestx - pdat->physdata.poslc) 
	      < pdat->lensdata.headwid1 
	      && fabs(pdat->mouse.nesty - pdat->physdata.axis) 
	      < pdat->lensdata.ylen)
	{
	  pdat->mouse.trap = 1;  
	  pdat->mouse.path1 =
	    ((GTK_ADJUSTMENT (pdat->barl.adj))->value)
	    /GTK_ADJUSTMENT (pdat->barxx.adj)->value - pdat->mouse.nestx;
	  /*
	  (GTK_ADJUSTMENT (pdat->barl.adj))->value =
	    (pdat->mouse.nestx + pdat->mouse.path1)
	    *GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
	  "value-changed");*/
	}
      else if(fabs(pdat->mouse.nestx - pdat->physdata.posld)
	      < pdat->lensdata.headwid2 
	      && fabs(pdat->mouse.nesty - pdat->physdata.axis) 
	      < pdat->lensdata.ylen)
	{
	  pdat->mouse.trap = 2;
	  pdat->mouse.path2 =
	    ((GTK_ADJUSTMENT (pdat->barr.adj))->value)
	    /GTK_ADJUSTMENT (pdat->barxx.adj)->value - pdat->mouse.nestx;
	  /*(GTK_ADJUSTMENT (pdat->barr.adj))->value =
	    (pdat->mouse.nestx + pdat->mouse.path2)
	    *GTK_ADJUSTMENT (pdat->barxx.adj)->value;

	  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
	  "value-changed");*/
	}
      
    }
  else if (event->type == GDK_BUTTON_RELEASE)
    pdat->mouse.trap = 0;

  gtk_widget_queue_draw (pdat->window);
  return TRUE;
}
