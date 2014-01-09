#include <gtk/gtk.h>
#include <cairo.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <structs.h>
#include <callbacks.h>
#include <prjcairo.h>
#include <cairofunc.h>

#ifndef RENDER
#define RENDER expose_ev
#endif

int
main (int argc, char **argv)
{
  progdata *pdat;

  //barras e butões
  GtkWidget *button, *barlensl, *barlensr,
    *barfocc, *barfocd, *barangl, *lunbtn,
    *barsclx,
    *ampxx, *redxx,*lenstype2;

  // boxes
  GtkWidget *vbox1, *topbox, *midbox, *setbox, *datbox,
    *noteb, *notebp1, *notebp2, *notebp3, *notebp4, 
    *optnbox, *statusbox, *noteb4xx, *rlbox;

  //frames
  GtkWidget *dtbfrm, *drwfrm, 
    *blcfrm, *bldfrm, *bfcfrm, *bfdfrm, *bangfrm,
    *xxfrm, *lensfrm;

  //setup inicial e criação da janela principal
  pdat = calloc (1, sizeof (progdata));

  strcpy (pdat->barl.str, "value= 0000.0");
  strcpy (pdat->barr.str, "value= 0000.0");
  strcpy (pdat->barfc.str, "focal length= 000.0");
  strcpy (pdat->barfd.str, "focal length= 000.0");
  strcpy (pdat->barang.str, "angulo= 00.0");
  strcpy (pdat->barxx.str, "1:0000");

  sprintf (pdat->btnlock.label, " Unlocked ");

  gtk_init (&argc, &argv);

  pdat->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (pdat->window), 920, 576);
  gtk_window_set_position (GTK_WINDOW (pdat->window), GTK_WIN_POS_CENTER);

////////////////////////////////////////////////////////////////////////////////
  // layout geral das boxes
  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pdat->window), vbox1);

  topbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox1), topbox, FALSE, TRUE, 0);

  midbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox1), midbox, TRUE, TRUE, 0);

  setbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_end (GTK_BOX (vbox1), setbox, FALSE, FALSE, 0);

  datbox = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_end (GTK_BOX (midbox), datbox, FALSE, FALSE, 0);

  dtbfrm = gtk_frame_new ("Opções");
  gtk_box_pack_start (GTK_BOX (datbox), dtbfrm, TRUE, TRUE, 0);

  pdat->drawbox = gtk_hbox_new (FALSE, 0);
  drwfrm = gtk_frame_new ("");

  gtk_box_pack_start (GTK_BOX (midbox), drwfrm, TRUE, TRUE, 0);
  gtk_container_add (GTK_CONTAINER (drwfrm), pdat->drawbox);

  notebp1 = gtk_vbox_new (FALSE, 0);
  notebp2 = gtk_vbox_new (FALSE, 0);
  notebp3 = gtk_vbox_new (FALSE, 0);
  notebp4 = gtk_vbox_new (FALSE, 0);

  optnbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (dtbfrm), optnbox);

  statusbox = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_end (GTK_BOX (optnbox), statusbox, FALSE, FALSE, 100);

  lensfrm = gtk_frame_new ("Tipo de Lentes");
  gtk_box_pack_end (GTK_BOX (optnbox), lensfrm, TRUE, TRUE, 0);
  
  rlbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (lensfrm), rlbox);

////////////////////////////////////////////////////////////////////////////////
  // adjusts e barras
  //posição lentes
  pdat->barl.adj = gtk_adjustment_new (200.0, 20.0, 101.0, 0.1, 1.0, 1.0);
  pdat->barr.adj = gtk_adjustment_new (250.0, 20.0, 101.0, 0.1, 1.0, 1.0);

  blcfrm = gtk_frame_new ("Posição da Lente Convergente");
  gtk_container_add (GTK_CONTAINER (notebp1), blcfrm);
  barlensl = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barl.adj));
  gtk_container_add (GTK_CONTAINER (blcfrm), barlensl);

  bldfrm = gtk_frame_new ("Posição da Lente Divergente");
  gtk_container_add (GTK_CONTAINER (notebp1), bldfrm);
  barlensr = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barr.adj));
  gtk_container_add (GTK_CONTAINER (bldfrm), barlensr);

  pdat->barl.lbl = gtk_label_new (pdat->barl.str);
  gtk_box_pack_start (GTK_BOX (statusbox), pdat->barl.lbl, TRUE, TRUE, 0);

  pdat->barr.lbl = gtk_label_new (pdat->barr.str);
  gtk_box_pack_start (GTK_BOX (statusbox), pdat->barr.lbl, TRUE, TRUE, 0);

  //distancia focal
  pdat->barfc.adj = gtk_adjustment_new (300.0, 1.0, 301.0, 0.1, 1.0, 1.0);
  pdat->barfd.adj = gtk_adjustment_new (150.0, 1.0, 301.0, 0.1, 1.0, 1.0);

  bfcfrm = gtk_frame_new ("Distância Focal da Lente Convergente");
  gtk_container_add (GTK_CONTAINER (notebp2), bfcfrm);
  barfocc = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barfc.adj));
  gtk_container_add (GTK_CONTAINER (bfcfrm), barfocc);

  bfdfrm = gtk_frame_new ("Distância Focal da Lente Divergente");
  gtk_container_add (GTK_CONTAINER (notebp2), bfdfrm);
  barfocd = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barfd.adj));
  gtk_container_add (GTK_CONTAINER (bfdfrm), barfocd);

  pdat->barfc.lbl = gtk_label_new (pdat->barfc.str);
  gtk_box_pack_start (GTK_BOX (statusbox), pdat->barfc.lbl, TRUE, TRUE, 0);

  pdat->barfd.lbl = gtk_label_new (pdat->barfd.str);
  gtk_box_pack_start (GTK_BOX (statusbox), pdat->barfd.lbl, TRUE, TRUE, 0);

  //angulo
  pdat->barang.adj = gtk_adjustment_new (20, -89.0, 90.0, 0.1, 1.0, 1.0);

  bangfrm = gtk_frame_new ("Ângulo de Incidência");
  gtk_container_add (GTK_CONTAINER (notebp3), bangfrm);
  barangl = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barang.adj));
  gtk_container_add (GTK_CONTAINER (bangfrm), barangl);

  pdat->barang.lbl = gtk_label_new (pdat->barang.str);
  gtk_box_pack_start (GTK_BOX (statusbox), pdat->barang.lbl, TRUE, TRUE, 0);

  
  //escala
  pdat->barxx.adj = gtk_adjustment_new (2, 1, 100.0, 0.1, 1.0, 1.0);

  xxfrm = gtk_frame_new ("Escala");
  gtk_container_add (GTK_CONTAINER (notebp4), xxfrm);
  barsclx = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barxx.adj));
  gtk_container_add (GTK_CONTAINER (xxfrm), barsclx);

  pdat->barxx.lbl = gtk_label_new (pdat->barxx.str);
 
  noteb4xx = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (notebp4), noteb4xx);

  pdat->barxx.lbl = gtk_label_new (pdat->barxx.str);
  gtk_container_add (GTK_CONTAINER (noteb4xx), pdat->barxx.lbl);

  pdat->lnsc.pos = &(GTK_ADJUSTMENT (pdat->barl.adj)->value);
  pdat->lnsc.focus = &(GTK_ADJUSTMENT (pdat->barfc.adj)->value);
  pdat->barl.alt = &pdat->barr;
  pdat->barr.alt = &pdat->barl;
  pdat->lnsd.pos = &(GTK_ADJUSTMENT (pdat->barr.adj)->value);
  pdat->lnsd.focus = &(GTK_ADJUSTMENT (pdat->barfd.adj)->value);

////////////////////////////////////////////////////////////////////////////////
  //botões
  button = gtk_button_new_with_label ("Recomeçar");
  gtk_box_pack_end (GTK_BOX (optnbox), button, FALSE, FALSE, 5);

  lunbtn = gtk_button_new_with_label("Criar Luneta");
  gtk_box_pack_end(GTK_BOX(optnbox), lunbtn, FALSE, FALSE, 5);

  pdat->virtbtn = gtk_check_button_new_with_label("Raios Virtuais");
  gtk_box_pack_start(GTK_BOX(optnbox), pdat->virtbtn, FALSE, FALSE, 5);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->virtbtn), TRUE);

  pdat->distbtn = gtk_check_button_new_with_label("Fixar Distância\nentre Lentes");
  gtk_box_pack_start(GTK_BOX(optnbox), pdat->distbtn, FALSE, FALSE, 5);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->distbtn), FALSE);

  //radio x

  ampxx = gtk_radio_button_new_with_label(NULL,"Ampliar");
  gtk_container_add (GTK_CONTAINER (noteb4xx), ampxx);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (ampxx), TRUE);

  redxx = gtk_radio_button_new_with_label(gtk_radio_button_group 
					  (GTK_RADIO_BUTTON (ampxx)),
					  "Reduzir");
  gtk_container_add (GTK_CONTAINER (noteb4xx), redxx);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (redxx), FALSE);

  //radio type

  pdat->lenstype = gtk_radio_button_new_with_label(NULL,
						   "Esquemáticas");
  gtk_container_add (GTK_CONTAINER (rlbox), pdat->lenstype);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->lenstype), TRUE);

  lenstype2 = gtk_radio_button_new_with_label(gtk_radio_button_group 
					  (GTK_RADIO_BUTTON (pdat->lenstype)),
					  "Desenhadas");
  gtk_container_add (GTK_CONTAINER (rlbox), lenstype2);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (lenstype2), FALSE);

  //butão toggle com cores

  gdk_color_parse ("red", &pdat->btnlock.coloron);
  gdk_color_parse ("green", &pdat->btnlock.coloroff);
  gdk_color_parse ("orange", &pdat->btnlock.colorhigh);
  pdat->btnlock.name = gtk_toggle_button_new_with_label (pdat->btnlock.label);

  gtk_box_pack_start (GTK_BOX (optnbox), pdat->btnlock.name, FALSE, FALSE, 2);
  gtk_widget_modify_bg (pdat->btnlock.name,
			GTK_STATE_NORMAL, &pdat->btnlock.coloroff);
  gtk_widget_modify_bg (pdat->btnlock.name,
			GTK_STATE_PRELIGHT, &pdat->btnlock.colorhigh);
  gtk_widget_modify_bg (pdat->btnlock.name,
			GTK_STATE_ACTIVE, &pdat->btnlock.coloron);

////////////////////////////////////////////////////////////////////////////////
  //notebook 
  noteb = gtk_notebook_new ();
  gtk_notebook_append_page (GTK_NOTEBOOK (noteb), notebp1, NULL);
  gtk_notebook_append_page (GTK_NOTEBOOK (noteb), notebp2, NULL);
  gtk_notebook_append_page (GTK_NOTEBOOK (noteb), notebp3, NULL);
  gtk_notebook_append_page (GTK_NOTEBOOK (noteb), notebp4, NULL);
  gtk_box_pack_start (GTK_BOX (setbox), noteb, TRUE, TRUE, 0);

/////////////////////////////////////////////////////////////////////////
  // temporário até ser ajustável
  pdat->lensdata.ylen = 175;
  pdat->lensdata.xwid = 3;
  pdat->flg.virt = 1;
  pdat->mouse.trap = 0;
////////////////////////////////////////////////////////////////////////////////
  //sinais e callbacks

  gtk_widget_add_events (pdat->window, GDK_BUTTON_PRESS_MASK);

  gtk_widget_set_events (pdat->window, 
			 GDK_POINTER_MOTION_MASK | 
			 GDK_BUTTON_PRESS_MASK | 
			 GDK_BUTTON_RELEASE_MASK);

  g_signal_connect_swapped (G_OBJECT (pdat->window), "destroy",
			    G_CALLBACK (gtk_main_quit), NULL);

  //callbacks butões
  g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (set_val), pdat);

  g_signal_connect (G_OBJECT (lunbtn), "clicked", G_CALLBACK (luneta), pdat);

  g_signal_connect (G_OBJECT (pdat->btnlock.name), "toggled",
		    G_CALLBACK (lchange), pdat);

  g_signal_connect (G_OBJECT (pdat->virtbtn), "toggled",
		    G_CALLBACK (virtchange), pdat);

  g_signal_connect (G_OBJECT (pdat->distbtn), "toggled",
		    G_CALLBACK (distchange), pdat);

  g_signal_connect (G_OBJECT (ampxx), "toggled",
		    G_CALLBACK (scalechange), pdat);

  g_signal_connect (G_OBJECT (lenstype2), "toggled",
		    G_CALLBACK (typechange), pdat);

  //callbacks barras
  g_signal_connect (G_OBJECT (pdat->barl.adj), "value-changed",
		    G_CALLBACK (upd_adj), pdat);

  g_signal_connect (G_OBJECT (pdat->barr.adj), "value-changed",
		    G_CALLBACK (upd_adj), pdat);

  g_signal_connect (G_OBJECT (pdat->barfc.adj), "value-changed",
		    G_CALLBACK (upd_adj_free), pdat);

  g_signal_connect (G_OBJECT (pdat->barfd.adj), "value-changed",
		    G_CALLBACK (upd_adj_free), pdat);

  g_signal_connect (G_OBJECT (pdat->barang.adj), "value-changed",
		    G_CALLBACK (upd_adj_free), pdat);

  g_signal_connect (G_OBJECT (pdat->barxx.adj), "value-changed",
		    G_CALLBACK (upd_adj_free), pdat);

  //callbacks janela / Xwindows
  g_signal_connect (pdat->window, "expose-event", 
		    G_CALLBACK (RENDER), pdat);

  g_signal_connect (pdat->window, "motion-notify-event", 
		    G_CALLBACK (titanmouse), pdat);

  g_signal_connect (pdat->window, "button_press_event", 
		    G_CALLBACK (titanmouse), pdat);

  g_signal_connect (pdat->window, "button_release_event", 
		    G_CALLBACK (titanmouse), pdat);

  // neste caso o configure-event é accionado por mudança no tamanho da janela
  g_signal_connect (pdat->window, "configure-event",
		    G_CALLBACK (cfg_event), pdat);

  gtk_widget_set_app_paintable (pdat->window, TRUE);

////////////////////////////////////////////////////////////////////////////////  

  gtk_widget_show_all (pdat->window);
  gtk_main ();

  free(pdat);
  return 0;
}
