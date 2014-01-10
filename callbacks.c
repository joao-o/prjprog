#include <gtk/gtk.h>
#include <cairo.h>

#include <structs.h>
#include <prjcairo.h>
#include <cairofunc.h>
#include <phys.h>
#include <miscui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define L_VAL 21

// ficheiro com as CSR (callback service routines)

// CSR de "configure events" (as dimensões da janela mudaram)

void
jerrylens (progdata *pdat, bardat *barra)
{
  if(pdat->mouse.nestx + pdat->mouse.path1 
     > pdat->drawbox->allocation.width - TOL)

    (GTK_ADJUSTMENT (barra->adj))->value 
      = (pdat->drawbox->allocation.width - TOL)
      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;	  

  else if(pdat->mouse.nestx + pdat->mouse.path1 < TOL)

    (GTK_ADJUSTMENT (barra->adj))->value 
      = (TOL);

  else
    (GTK_ADJUSTMENT (barra->adj))->value =
      (pdat->mouse.nestx + pdat->mouse.path1)
      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	 
  g_signal_emit_by_name (GTK_ADJUSTMENT (barra->adj),
			 "value-changed");
}

void
mickeyfocus (progdata *pdat, bardat *barra)
{
  if(pdat->mouse.nestx + pdat->mouse.path1 >
     pdat->drawbox->allocation.width - (TOL))

    (GTK_ADJUSTMENT (barra->adj))->value 
      = (pdat->drawbox->allocation.width - (TOL))
      *GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	  
  else if(pdat->mouse.nestx + pdat->mouse.path1
	  < 10)
	  
    (GTK_ADJUSTMENT (barra->adj))->value 
      = 10*GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	  
  else
    (GTK_ADJUSTMENT (barra->adj))->value =
      (pdat->mouse.nestx + pdat->mouse.path1)*
      GTK_ADJUSTMENT (pdat->barxx.adj)->value;
	 
  if((GTK_ADJUSTMENT (barra->adj))->value >
     (GTK_ADJUSTMENT (barra->adj))->upper)
    (GTK_ADJUSTMENT (barra->adj))->value =
      (GTK_ADJUSTMENT (barra->adj))->upper;

  g_signal_emit_by_name (GTK_ADJUSTMENT (barra->adj),
			 "value-changed");
}

gboolean
upd_phys (progdata *dat)
{
  progdata *pdat = (progdata *) dat; 

  pdat->phys.poslc = GTK_ADJUSTMENT (pdat->barl.adj)->value
    /GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  pdat->phys.fc = *pdat->lnsc.focus
    /GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  pdat->phys.posld = GTK_ADJUSTMENT (pdat->barr.adj)->value
    /GTK_ADJUSTMENT (pdat->barxx.adj)->value;

  pdat->phys.fd = *pdat->lnsd.focus
    /GTK_ADJUSTMENT (pdat->barxx.adj)->value;

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

  sprintf (barra->str + 21, "%5.1f", (GTK_ADJUSTMENT (barra->adj))->value);
  gtk_label_set_text (GTK_LABEL (barra->lbl), barra->str);
  barra->save = (GTK_ADJUSTMENT (barra->adj))->value;
  return;
}

gboolean
upd_adj (GtkWidget * widget, gpointer dat)
{
  progdata *pdat = (progdata *) dat;
  bardat *barra;
  double temp;

  if (GTK_OBJECT (widget) == pdat->barl.adj)
    barra = &pdat->barl;
  else
    barra = &pdat->barr;
 if (!pdat->flg.lock)
    {
      if(pdat->flg.dist)
        {
          temp = (GTK_ADJUSTMENT(barra->adj))->value+pdat->phys.ldist*
              dsign(-barra->save+barra->alt->save);
          /*
          calcula onde por a outra lente quando "dist" está set 
          o dsign serve para ver a posições relativas
          ie se soma ou subtrai uso temp pois só quero fazer aquilo^ uma vez
          */

          if (temp>TOL && 
              temp < pdat->drawbox->allocation.width * *pdat->phys.scl-TOL )
            {
              GTK_ADJUSTMENT(barra->alt->adj)->value = temp;
              upd_mod(barra->alt);
            }
          else
            GTK_ADJUSTMENT(barra->adj)->value = barra->save;
        }

      upd_mod (barra);
      gtk_widget_queue_draw (pdat->window);
    }
  else
    (GTK_ADJUSTMENT (barra->adj))->value = barra->save;

 
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
      l = 21;
    }
  else if (GTK_OBJECT (widget) == pdat->barxx.adj)
    {
      barra = &pdat->barxx;
      l = 10;
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
//Estes valores fazem uma luneta bonitinha
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
      //gtk_adjustment_set_value (GTK_ADJUSTMENT (pdat->barl.adj),320.);
      *(pdat->lnsc.focus) = 180;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfc.adj),
			     "value-changed");
      *(pdat->lnsd.focus) = 20;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj),
			     "value-changed");
      *(pdat->lnsd.pos) = 477.8;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj),
			     "value-changed");
      (GTK_ADJUSTMENT (pdat->barang.adj))->value = 20.;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
			     "value-changed");
      (GTK_ADJUSTMENT (pdat->barxx.adj))->value = 1.;
      g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj),
			     "value-changed");

      gtk_adjustment_set_value (GTK_ADJUSTMENT (pdat->barang.adj), 20.);

      gtk_adjustment_set_value (GTK_ADJUSTMENT (pdat->barxx.adj), 2.);

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
  pdat->phys.ldist = fabs(*pdat->lnsc.pos-*pdat->lnsd.pos);
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
			       1.1, 1.0, 10.0, 0.1, 0, 0);

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
  

  pdat->mouse.nestx = ((GdkEventMotion *) event)->x;
  pdat->mouse.nesty = ((GdkEventMotion *) event)->y;

  if(pdat->mouse.nestx > pdat->drawbox->allocation.width
         || pdat->mouse.nestx < 0)
    {
      pdat->mouse.trap=0;
      return FALSE;
    }
  else if (pdat->mouse.trap != 0 && event->type == GDK_MOTION_NOTIFY)
    {

      switch (pdat->mouse.trap)
        {
	case 1:
	  jerrylens(pdat,&pdat->barl);
	  break;
	case 2:
	  jerrylens(pdat,&pdat->barr);
	  break;
	case 3:
	  mickeyfocus(pdat,&pdat->barfc);
	  break;
	case 4:
	  mickeyfocus(pdat,&pdat->barfd);
	  break;

        }
  
    }
  else if (event->type == GDK_BUTTON_PRESS)
    {
      if (((pdat->mouse.nestx - pdat->phys.poslc) 
          < pdat->phys.fc + pdat->lensdata.xwid*1.5)
         && ((pdat->mouse.nestx - pdat->phys.poslc) 
             > pdat->phys.fc - pdat->lensdata.xwid*1.5)
         && (fabs(pdat->mouse.nesty - pdat->phys.axis) 
             < pdat->lensdata.xwid*1.5))
        {
          pdat->mouse.trap = 3; 
          pdat->mouse.path1 =pdat->phys.fc - pdat->mouse.nestx;
        }
      else if(((pdat->mouse.nestx - pdat->phys.posld) 
               < pdat->phys.fd + pdat->lensdata.xwid*1.5)
              && ((pdat->mouse.nestx - pdat->phys.posld) 
                  > pdat->phys.fd - pdat->lensdata.xwid*1.5)
              && (fabs(pdat->mouse.nesty - pdat->phys.axis) 
                  < pdat->lensdata.xwid*1.5))
        {
          pdat->mouse.trap = 4;
          pdat->mouse.path1 =pdat->phys.fd - pdat->mouse.nestx;
        }
      else if(fabs(pdat->mouse.nestx - pdat->phys.poslc) 
              < pdat->lensdata.headwid1 
              && fabs(pdat->mouse.nesty - pdat->phys.axis) 
              < pdat->lensdata.ylen)
        {
          pdat->mouse.trap = 1;  
          pdat->mouse.path1 = pdat->phys.poslc - pdat->mouse.nestx;
        }
     else if(fabs(pdat->mouse.nestx - pdat->phys.posld)
              < pdat->lensdata.headwid2 
              && fabs(pdat->mouse.nesty - pdat->phys.axis) 
              < pdat->lensdata.ylen)
        {
          pdat->mouse.trap = 2;
          pdat->mouse.path1 = pdat->phys.posld - pdat->mouse.nestx;
        }
    }
  else if (event->type == GDK_BUTTON_RELEASE)
    pdat->mouse.trap = 0;


  return TRUE;
}
