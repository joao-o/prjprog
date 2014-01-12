#include <cairo.h>
#include <gtk/gtk.h>
#include <structs.h>
#include <stdio.h>
#include <phys.h>
#include <math.h>

const double dsh[]={5.0, 10.0};
const double nodsh[]={1};

// é na realidade uma CSR para espose events está aqui porque desenha principalmente
gboolean
expose_ev (GtkWidget * widget, GdkEventExpose *event, gpointer dat)
{
  progdata *pdat;
  cairo_t *cr;
  int width, height, i, j, t;
  double pos1, pos2, pos3, scl, rd, cc, opn;
  double uy[7], ly[7], mx[7], rgbr[3], rgbv[3];
  double ang, fc, fd, hwid1, hwid2;
  int dshl=sizeof(dsh)/sizeof(dsh[0]);
  static int faz_init=0;

  pdat = (progdata*) dat;
  scl = gtk_adjustment_get_value (GTK_ADJUSTMENT (pdat->barxx.adj));
  
  //cor dos raios reais
  rgbr[0] = 0.90;
  rgbr[1] = 0.90;
  rgbr[2] = 0.00;

  //cor dos raios virtuais
  rgbv[0] = 0.10;
  rgbv[1] = 0.50;
  rgbv[2] = 0.70;

///////////////////////////////////////////////////////////////////////////////
  //obtem valores

  pdat->phys.axis = pdat->drawbox->allocation.height/2. + 20;

  gtk_window_get_size (GTK_WINDOW (pdat->window), &width, &height);

  gtk_adjustment_set_upper (GTK_ADJUSTMENT (pdat->barl.adj),
			    (pdat->drawbox->allocation.width)*scl);

  gtk_adjustment_set_upper (GTK_ADJUSTMENT (pdat->barr.adj),
			    (pdat->drawbox->allocation.width)*scl);

  if (faz_init == 0)
    {
      faz_init = 1;
      set_val (NULL,dat);
    }

  pos1 = pdat->phys.c.pos;

  if (pos1 > pdat->drawbox->allocation.width)
    {
      pos1 = pdat->drawbox->allocation.width;
      gtk_adjustment_set_value (GTK_ADJUSTMENT (pdat->barl.adj), pos1);
    }

  
  pos3 = pdat->phys.d.pos;

  if (pos3 > pdat->drawbox->allocation.width)
    {
      pos3 = pdat->drawbox->allocation.width;
      gtk_adjustment_set_value (GTK_ADJUSTMENT (pdat->barr.adj), pos3);
    }

  pos2 = pdat->phys.axis;

  fd = pdat->phys.d.focus;
  fc = pdat->phys.c.focus;
  ang = ((GTK_ADJUSTMENT (pdat->barang.adj))->value)*(M_PI/180);

////////////////////////////////////////////////////////////////////////////////

  if(pos3 < pos1)
    {
      mx[2] = - fd + pos3;
      mx[5] = pos1 + fconj(fc, - (pos1 - mx[2]));
      mx[1] = pos3; //primeira lente
      mx[3] = pos1; //segunda lente
    }
  else
    {
      mx[2] = fc + pos1;
      mx[5] = pos3 + fconj(-fd, - (pos3 - mx[2]));
      mx[1] = pos1;
      mx[3] = pos3;
    }

  //raios
  
  // determina os pontos onde tem de passar os raios

  //infinito
  mx[0]=0;
  ly[0]= pos2 - mx[1]*tan(ang);

  //lente convergente
  ly[1]= pos2;

  //imagem 1
  ly[2]= lin(mx[0],ly[0],mx[1],ly[1],mx[2]);
  uy[2]= ly[2];

  uy[1]= uy[2];
  uy[0]= ly[0] + (uy[1] - ly[1]);

  //lente divergente
  uy[3]= pos2;
  ly[3]= ly[2];

  //imagem2
  uy[5]= lin(mx[2],ly[2],mx[3],uy[3],mx[5]);
  ly[5]= uy[5];

  //infinito2
  mx[4]= pdat->drawbox->allocation.width;
  uy[4]= lin(mx[3], uy[3], mx[5], ly[5], mx[4]);
  ly[4]= lin(mx[3], ly[3], mx[5], ly[5], mx[4]);
 

////////////////////////////////////////////////////////////////////////////////
//desenha eixo
  cr = gdk_cairo_create (pdat->window->window);

  cairo_rectangle(cr, 0, 
		  10, 
		  (double) pdat->drawbox->allocation.width,
		  (double) pdat->drawbox->allocation.height);
  cairo_clip(cr);

  cairo_set_source_rgba (cr, 0., 0., 0., 1.);
  cairo_paint(cr);

  cairo_set_source_rgb (cr, 0.5, 0.5, 0.5);
  cairo_set_line_width (cr, 2);
  cairo_move_to (cr, 0, pdat->phys.axis);
  cairo_line_to (cr, pdat->drawbox->allocation.width,
		 pdat->phys.axis);
  cairo_stroke (cr);

  cairo_set_source_rgb (cr, 1, 0.55, 0);

  cairo_set_line_width (cr, XWID);

  //////////////////////////////////////////////////////////////////////////////

  if(fabs(ly[2] - pos2) > 87.5)
    (pdat->ldat.ylen) = 2*fabs(ly[2] - pos2);
  else
    (pdat->ldat.ylen) = 175;


  //////////////////////////////////////////////////////////////////////////////

  //desenha lentes
  if(pdat->lenstype->state)
    {
      //Modo esquemático
      hwid1 = pdat->ldat.headwid1 = 
	240/(fabs(gtk_adjustment_get_value (GTK_ADJUSTMENT (pdat->barfc.adj))) 
	     + 48.33) + 4.9; 
               //fórmula mágica concedida pelo Grande Xamã Fit-Eha-Ya
               //               será que quis dizer: ^mago^ ?

      hwid2 = pdat->ldat.headwid2 =  
	240/(fabs(gtk_adjustment_get_value (GTK_ADJUSTMENT (pdat->barfd.adj)))
	     + 48.33) 
	+ 4.9;

      cairo_move_to (cr, pos1, pos2 + (pdat->ldat.ylen)/2 - 3);
      cairo_line_to (cr, pos1, pos2 - (pdat->ldat.ylen)/2 + 3);
      cairo_stroke (cr);

      cairo_move_to (cr, pos1 - hwid1, pos2 - (pdat->ldat.ylen)/2 + 15);
      cairo_line_to (cr, pos1 + hwid1, pos2 - (pdat->ldat.ylen)/2 + 15);
      cairo_line_to (cr, pos1, pos2 - (pdat->ldat.ylen)/2);
      cairo_line_to (cr, pos1 - hwid1, pos2 - (pdat->ldat.ylen)/2 + 15);
      cairo_line_to (cr, pos1 + hwid1, pos2 - (pdat->ldat.ylen)/2 + 15);

      cairo_move_to (cr, pos1 - hwid1, pos2 + (pdat->ldat.ylen)/2 - 15);
      cairo_line_to (cr, pos1 + hwid1, pos2 + (pdat->ldat.ylen)/2 - 15);
      cairo_line_to (cr, pos1, pos2 + (pdat->ldat.ylen)/2);
      cairo_line_to (cr, pos1 - hwid1, pos2 + (pdat->ldat.ylen)/2 - 15);
      cairo_line_to (cr, pos1 + hwid1, pos2 + (pdat->ldat.ylen)/2 - 15);

      cairo_fill (cr);

      cairo_set_source_rgb (cr, 0.21, 0.21, 1);

      cairo_set_line_width (cr, XWID);

      cairo_move_to (cr, pos3, pos2 + (pdat->ldat.ylen)/2);
      cairo_line_to (cr, pos3, pos2 - (pdat->ldat.ylen)/2);
      cairo_stroke (cr);

      cairo_move_to (cr, pos3 - hwid2, pos2 - (pdat->ldat.ylen)/2 );
      cairo_line_to (cr, pos3 + hwid2, pos2 - (pdat->ldat.ylen)/2 );
      cairo_line_to (cr, pos3, pos2 - (pdat->ldat.ylen)/2 +15 );
      cairo_line_to (cr, pos3 - hwid2, pos2 - (pdat->ldat.ylen)/2 );
      cairo_line_to (cr, pos3 + hwid2, pos2 - (pdat->ldat.ylen)/2 );

      cairo_move_to (cr, pos3 - hwid2, pos2 + (pdat->ldat.ylen)/2 );
      cairo_line_to (cr, pos3 + hwid2, pos2 + (pdat->ldat.ylen)/2 );
      cairo_line_to (cr, pos3, pos2 + (pdat->ldat.ylen)/2 - 15);
      cairo_line_to (cr, pos3 - hwid2, pos2 + (pdat->ldat.ylen)/2 );
      cairo_line_to (cr, pos3 + hwid2, pos2 + (pdat->ldat.ylen)/2 );

      cairo_fill (cr);
    }
  else
    {
      //Modo "desenhadas"
      cairo_set_line_width (cr, 1);
      cairo_set_source_rgba (cr, 0.55, 0.60, 0.65, 0.6);

      rd = 2*fc + pdat->ldat.ylen + 200;
      cc = (rd*rd - (pdat->ldat.ylen)*(pdat->ldat.ylen)*0.25);
      cc = sqrt(cc);
      opn = asin((pdat->ldat.ylen)/(2*rd));
      pdat->ldat.headwid1 = rd - cc;

      cairo_arc (cr,  pos1 + cc, pos2, 
		 rd, M_PI - opn,
		 M_PI + opn);
      cairo_arc (cr,  pos1 - cc, pos2, 
		 rd, - opn,
		 + opn);
      cairo_fill (cr);
     
      rd =  2*fd + pdat->ldat.ylen + 200;
      cc = (rd*rd - (pdat->ldat.ylen)*(pdat->ldat.ylen)*0.25);
      cc = sqrt(cc);
      opn = asin((pdat->ldat.ylen)/(2*rd));
      pdat->ldat.headwid2 = 1.5*(rd - cc);

      cairo_arc (cr,  pos3 + cc + pdat->ldat.headwid2, pos2, 
		 rd, M_PI - opn,
		 M_PI + opn);
      cairo_arc (cr,  pos3 - cc - pdat->ldat.headwid2, pos2, 
		 rd, - opn,
		  + opn);
      cairo_move_to (cr, pos3 + pdat->ldat.headwid2, 
		     pos2 - (pdat->ldat.ylen)/2);
      cairo_line_to (cr, pos3 - pdat->ldat.headwid2, 
		     pos2 - (pdat->ldat.ylen)/2);
      cairo_move_to (cr, pos3 + pdat->ldat.headwid2, 
		     pos2 + (pdat->ldat.ylen)/2);
      cairo_line_to (cr, pos3 - pdat->ldat.headwid2, 
		     pos2 + (pdat->ldat.ylen)/2);
      
      cairo_fill (cr);

		 
    }

  /////////////////////////////////////////////////////////
  //Circulos para alterar distancias focais com rato

  cairo_set_line_width (cr, XWID);

  cairo_set_source_rgba (cr, 1, 0.55, 0, 1);
  cairo_move_to (cr, pos1 + fc, pos2);
  cairo_arc (cr, pos1 + fc, pos2, XWID*1.5, 15, 7. * M_PI);
  cairo_fill(cr);
  
  cairo_set_source_rgb (cr, 0.21, 0.21, 1);
  cairo_move_to (cr, pos3 + fd, pos2);
  cairo_arc (cr, pos3 + fd, pos2, XWID*1.5, 15, 7. * M_PI);
  cairo_fill(cr);
  cairo_stroke(cr);

  ///////////////////////////////////////////////////////////////
    
  cairo_set_source_rgb (cr, 0.44, 1.00, 0.22);

  cairo_set_line_width (cr, XWID/2);
  cairo_move_to (cr, mx[2], pos2);
  cairo_line_to (cr, mx[2], ly[2]);
  cairo_stroke (cr);

  cairo_set_source_rgb (cr, 0.55, 0.00, 0.55);

  cairo_set_line_width (cr, XWID/2);
  cairo_move_to (cr, mx[5], pos2);
  cairo_line_to (cr, mx[5], ly[5]);
  cairo_stroke (cr);

  cairo_set_line_width (cr, XWID/3);

  //desenha
  for(i=0;i<4;i++)
    {
      t = 0;

      //Detecta se deve partir o raio em raio virtual/real)

      if(mx[i]<mx[i+1])                  //quando os raios são à partida reais
	{
	  for(j=1;j<4;j+=2)              //e houver uma lente (1 ou 3)
	    if( (j != i && j != i+1)     //que não é a origem/destino dos raios
	       && (mx[i] < mx[j] && mx[j] < mx[i+1]) ) //mas está entre eles
	      {
		t = 1;

		mx[6]=mx[j];
		ly[6]=lin(mx[i],ly[i],mx[i+1],ly[i+1],mx[6]);
		uy[6]=lin(mx[i],uy[i],mx[i+1],uy[i+1],mx[6]);
		
		if( j > i+1 || pdat->flg.virt == 1)
		  {
		    if( j > i+1 )
		      {
			cairo_set_dash(cr,nodsh,0,0);
			cairo_set_source_rgb (cr, rgbr[0], rgbr[1], rgbr[2]);
		      }
		    else
		      {
			cairo_set_source_rgb (cr, rgbv[0], rgbv[1], rgbv[2]);
			cairo_set_dash(cr,dsh,dshl,0);
		      }

		    cairo_move_to (cr, mx[i], ly[i]);
		    cairo_line_to (cr, mx[6], ly[6]);
		    cairo_move_to (cr, mx[i], uy[i]);
		    cairo_line_to (cr, mx[6], uy[6]);
		    cairo_stroke(cr);
		  }

		if( j < i || pdat->flg.virt == 1 )
		  {
		    if( j < i )
		      {
			cairo_set_dash(cr,nodsh,0,0);
			cairo_set_source_rgb (cr, rgbr[0], rgbr[1], rgbr[2]);
		      }
		    else
		      {
			cairo_set_source_rgb (cr, rgbv[0], rgbv[1], rgbv[2]);
			cairo_set_dash(cr,dsh,dshl,0);
		      }

		    cairo_move_to (cr, mx[6], ly[6]);
		    cairo_line_to (cr, mx[i+1], ly[i+1]);
		    cairo_move_to (cr, mx[6], uy[6]);
		    cairo_line_to (cr, mx[i+1], uy[i+1]);
                    cairo_stroke(cr);
		  }
	      }
	}
	
      if(t == 0)
	if((mx[i] < mx[i+1] || pdat->flg.virt == 1))
	  {
	    if(mx[i] <= mx[i+1])
	      {
		cairo_set_dash(cr,nodsh,0,0);
		cairo_set_source_rgb (cr, rgbr[0], rgbr[1], rgbr[2]);
	      }
	    if(mx[i+1] < mx[i])
	      {
		cairo_set_source_rgb (cr, rgbv[0], rgbv[1], rgbv[2]);
		cairo_set_dash(cr,dsh,dshl,0);
	      }


	    cairo_move_to (cr, mx[i], ly[i]);
	    cairo_line_to (cr, mx[i+1], ly[i+1]);

	    cairo_move_to (cr, mx[i], uy[i]);
	    cairo_line_to (cr, mx[i+1], uy[i+1]);
	    cairo_stroke(cr);
	  }
    }
      
  if(pdat->flg.virt == 1 && mx[5] < pos3)
    {

      cairo_set_source_rgb (cr, rgbv[0], rgbv[1], rgbv[2]);
      cairo_set_dash(cr,dsh,dshl,0);

      cairo_move_to (cr, mx[3], ly[3]);
      cairo_line_to (cr, mx[5], ly[5]);

      cairo_move_to (cr, mx[3], uy[3]);
      cairo_line_to (cr, mx[5], uy[5]);
      cairo_stroke (cr);
    }

  cairo_destroy (cr);
  return FALSE;
}
