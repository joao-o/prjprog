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
    *barsclx, *lblpos, *lblfoc,
    *ampxx, *redxx, *lenstype2, *stuffings;

  // boxes
  GtkWidget *hbox1, *vbox1, *topbox, *midbox, *setbox, *datbox,
    *noteb, *notebp[4], 
    *optnbox, *statusbox, *noteb4xx, *rlbox, *lblbox[9],
    *btntbl, *notelbl[4];

  //frames
  GtkWidget *dtbfrm, *drwfrm, 
    *blcfrm, *bldfrm, *bfcfrm, *bfdfrm, *bangfrm,
    *xxfrm, *lensfrm, *statusfrm;

  int i;

  //setup inicial e criação da janela principal
  pdat = calloc (1, sizeof (progdata));

  strcpy (pdat->barl.str, "\tLente Convergente = 0000.0");
  strcpy (pdat->barr.str, "\tLente Divergente  = 0000.0");
  strcpy (pdat->barfc.str, "\tLente Convergente = 000.0");
  strcpy (pdat->barfd.str, "\tLente Divergente  = 000.0");
  strcpy (pdat->barang.str, "\n Angulo (em graus) = 00.0");
  strcpy (pdat->barxx.str, "\n Escala 1:0000");

  sprintf (pdat->btnlock.label, " Unlocked ");

  gtk_init (&argc, &argv);

  pdat->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (pdat->window), 920, 650);
  gtk_window_set_position (GTK_WINDOW (pdat->window), GTK_WIN_POS_CENTER);

////////////////////////////////////////////////////////////////////////////////
  // layout geral das boxes

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (pdat->window), hbox1);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox1, TRUE, TRUE, 0);

  topbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox1), topbox, FALSE, TRUE, 0);

  midbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox1), midbox, TRUE, TRUE, 0);

  setbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_end (GTK_BOX (vbox1), setbox, FALSE, FALSE, 0);

  datbox = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_end (GTK_BOX (hbox1), datbox, FALSE, FALSE, 0);

  dtbfrm = gtk_frame_new ("Opções");
  gtk_box_pack_start (GTK_BOX (datbox), dtbfrm, TRUE, TRUE, 0);

  pdat->drawbox = gtk_hbox_new (FALSE, 0);
  drwfrm = gtk_frame_new ("");

  gtk_box_pack_start (GTK_BOX (midbox), drwfrm, TRUE, TRUE, 0);
  gtk_container_add (GTK_CONTAINER (drwfrm), pdat->drawbox);

  notebp[0] = gtk_vbox_new (FALSE, 0);
  notebp[1] = gtk_vbox_new (FALSE, 0);
  notebp[2] = gtk_vbox_new (FALSE, 0);

  optnbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (dtbfrm), optnbox);

  statusfrm = gtk_frame_new ("Dados");
  statusbox = gtk_vbox_new (FALSE, 3);
  gtk_box_pack_end (GTK_BOX (datbox), statusfrm, FALSE, FALSE, 0);
  gtk_container_add (GTK_CONTAINER (statusfrm), statusbox);

  for(i=0; i<9; i++)
    {
      lblbox[i] = gtk_hbox_new(FALSE, 0);
      gtk_box_pack_start (GTK_BOX (statusbox), lblbox[i], FALSE, FALSE, 2);
    }

  lensfrm = gtk_frame_new ("Tipo de Lentes");
  gtk_box_pack_end (GTK_BOX (optnbox), lensfrm, TRUE, FALSE, 0);
  
  rlbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (lensfrm), rlbox);

  btntbl = gtk_table_new (3 , 5, TRUE);
  gtk_table_set_row_spacings(GTK_TABLE(btntbl), 5);
  gtk_table_set_col_spacings(GTK_TABLE(btntbl), 5);
  gtk_box_pack_end( GTK_BOX (optnbox), btntbl, FALSE, FALSE, 5);

////////////////////////////////////////////////////////////////////////////////
  // adjusts e barras

  //posição lentes
  pdat->barl.adj = gtk_adjustment_new (200.0, 20.0, 101.0, 0.1, 1.0, 1.0);
  pdat->barr.adj = gtk_adjustment_new (250.0, 20.0, 101.0, 0.1, 1.0, 1.0);

  blcfrm = gtk_frame_new ("Posição da Lente Convergente");
  gtk_container_add (GTK_CONTAINER (notebp[0]), blcfrm);
  barlensl = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barl.adj));
  gtk_container_add (GTK_CONTAINER (blcfrm), barlensl);

  bldfrm = gtk_frame_new ("Posição da Lente Divergente");
  gtk_container_add (GTK_CONTAINER (notebp[0]), bldfrm);
  barlensr = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barr.adj));
  gtk_container_add (GTK_CONTAINER (bldfrm), barlensr);

  lblpos = gtk_label_new ("  Posicao");
  gtk_box_pack_start (GTK_BOX (lblbox[0]), lblpos, FALSE, FALSE, 0);

  pdat->barl.lbl = gtk_label_new (pdat->barl.str);
  gtk_box_pack_start (GTK_BOX (lblbox[1]), pdat->barl.lbl, FALSE, FALSE, 0);

  pdat->barr.lbl = gtk_label_new (pdat->barr.str);
  gtk_box_pack_start (GTK_BOX (lblbox[2]), pdat->barr.lbl, FALSE, FALSE, 0);

  //distancia focal
  pdat->barfc.adj = gtk_adjustment_new (300.0, 1.0, 301.0, 0.1, 1.0, 1.0);
  pdat->barfd.adj = gtk_adjustment_new (150.0, 1.0, 301.0, 0.1, 1.0, 1.0);

  bfcfrm = gtk_frame_new ("Distância Focal da Lente Convergente");
  gtk_container_add (GTK_CONTAINER (notebp[1]), bfcfrm);
  barfocc = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barfc.adj));
  gtk_container_add (GTK_CONTAINER (bfcfrm), barfocc);

  bfdfrm = gtk_frame_new ("Distância Focal da Lente Divergente");
  gtk_container_add (GTK_CONTAINER (notebp[1]), bfdfrm);
  barfocd = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barfd.adj));
  gtk_container_add (GTK_CONTAINER (bfdfrm), barfocd);

  lblfoc = gtk_label_new ("\n  Distancia Focal\t\t\t   ");
  gtk_box_pack_start (GTK_BOX (lblbox[3]), lblfoc, FALSE, FALSE, 0);

  pdat->barfc.lbl = gtk_label_new (pdat->barfc.str);
  gtk_box_pack_start (GTK_BOX (lblbox[4]), pdat->barfc.lbl, FALSE, FALSE, 0);

  pdat->barfd.lbl = gtk_label_new (pdat->barfd.str);
  gtk_box_pack_start (GTK_BOX (lblbox[5]), pdat->barfd.lbl, FALSE, FALSE, 0);

  //angulo
  pdat->barang.adj = gtk_adjustment_new (20, -50.0, 50.0, 0.1, 1.0, 1.0);

  bangfrm = gtk_frame_new ("Ângulo de Incidência");
  gtk_container_add (GTK_CONTAINER (notebp[2]), bangfrm);
  barangl = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barang.adj));
  gtk_container_add (GTK_CONTAINER (bangfrm), barangl);

  pdat->barang.lbl = gtk_label_new (pdat->barang.str);
  gtk_box_pack_start (GTK_BOX (lblbox[6]), pdat->barang.lbl, FALSE, FALSE, 0);
  
  //escala
  pdat->barxx.adj = gtk_adjustment_new (2, 1, 10.0, 0.1, 1.0, 1.0);

  xxfrm = gtk_frame_new ("Escala");
  gtk_container_add (GTK_CONTAINER (notebp[2]), xxfrm);
  barsclx = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barxx.adj));
  gtk_container_add (GTK_CONTAINER (xxfrm), barsclx);

  pdat->barxx.lbl = gtk_label_new (pdat->barxx.str);
 
  noteb4xx = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (notebp[2]), noteb4xx);

  pdat->barxx.lbl = gtk_label_new (pdat->barxx.str);
  gtk_box_pack_start (GTK_BOX (lblbox[7]), pdat->barxx.lbl, FALSE, FALSE, 0);

  stuffings = gtk_label_new ("");
  gtk_box_pack_start (GTK_BOX (lblbox[8]), stuffings, FALSE, FALSE, 0);
  
  pdat->lnsc.pos = &(GTK_ADJUSTMENT (pdat->barl.adj)->value);
  pdat->lnsc.focus = &(GTK_ADJUSTMENT (pdat->barfc.adj)->value);
  pdat->barl.alt = &pdat->barr;
  pdat->barr.alt = &pdat->barl;
  pdat->lnsd.pos = &(GTK_ADJUSTMENT (pdat->barr.adj)->value);
  pdat->lnsd.focus = &(GTK_ADJUSTMENT (pdat->barfd.adj)->value);
  pdat->phys.scl =&(GTK_ADJUSTMENT(pdat->barxx.adj))->value;

////////////////////////////////////////////////////////////////////////////////
  //botões
  button = gtk_button_new_with_label ("Recomeçar");
  gtk_table_attach_defaults (GTK_TABLE (btntbl), button, 
			    1, 4, 0, 1);

  lunbtn = gtk_button_new_with_label("Criar Luneta");
  //  gtk_box_pack_end(GTK_BOX(optnbox), lunbtn, FALSE, FALSE, 5);
  gtk_table_attach_defaults (GTK_TABLE (btntbl), lunbtn, 
			    1, 4, 1, 2);

  pdat->virtbtn = gtk_check_button_new_with_label("Raios Virtuais");
  gtk_box_pack_start(GTK_BOX(optnbox), pdat->virtbtn, FALSE, FALSE, 5);

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->virtbtn), TRUE);

  pdat->distbtn = gtk_check_button_new_with_label
    ("Fixar Distância\nentre Lentes");
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

  //gtk_box_pack_start (GTK_BOX (optnbox), pdat->btnlock.name, FALSE, FALSE, 2);
  gtk_table_attach_defaults (GTK_TABLE (btntbl), pdat->btnlock.name, 
			    1, 4, 2, 3);
  gtk_widget_modify_bg (pdat->btnlock.name,
			GTK_STATE_NORMAL, &pdat->btnlock.coloroff);
  gtk_widget_modify_bg (pdat->btnlock.name,
			GTK_STATE_PRELIGHT, &pdat->btnlock.colorhigh);
  gtk_widget_modify_bg (pdat->btnlock.name,
			GTK_STATE_ACTIVE, &pdat->btnlock.coloron);

////////////////////////////////////////////////////////////////////////////////
  //notebook 

  notelbl[0] = gtk_label_new ("Posicao das Lentes");
  notelbl[1] = gtk_label_new ("Distancias Focais");
  notelbl[2] = gtk_label_new ("Angulo/Escala");

  noteb = gtk_notebook_new ();

  for(i=0; i<3; i++)
    {
      gtk_notebook_append_page (GTK_NOTEBOOK (noteb), notebp[i], NULL);
      gtk_notebook_set_tab_label(GTK_NOTEBOOK (noteb), notebp[i], 
				 notelbl[i]);
    }

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
