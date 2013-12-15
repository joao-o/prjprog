#include <gtk/gtk.h>
#include <cairo.h>


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <callbacks.h>
#include <structs.h>

int
main (int argc, char **argv)
{
  progdata *pdat;

  GtkWidget *window, *button, *barlensl, *barlensr; 
  
  // boxes
  GtkWidget *vbox1, *topbox, *midbox, *setbox, *datbox, *drawbox,
    *noteb, *notebp1, *notebp2, *notebp3, 
    *optnbox, *statusbox;
  
  //frames
  GtkWidget *dtbfrm, *drwfrm;
  //setup inicial e criação da janela principal
  pdat = calloc (1, sizeof (progdata));
  strcpy (pdat->barl.str, "value= 0.000");
  strcpy (pdat->barr.str, "value= 0.000");

  pdat->btnlock.state=0;
  sprintf(pdat->btnlock.label," Unlocked ");

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 720, 576);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);

////////////////////////////////////////////////////////////////////////////////
  // layout geral das boxes
  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox1);

  topbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(vbox1), topbox, FALSE, TRUE, 0);

  midbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(vbox1), midbox, TRUE, TRUE, 0);

  setbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_end (GTK_BOX(vbox1), setbox, FALSE, FALSE, 0);
  
  datbox = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_end (GTK_BOX(midbox), datbox, FALSE, FALSE, 0);

  dtbfrm = gtk_frame_new ("Opções");
  gtk_box_pack_start (GTK_BOX (datbox), dtbfrm, TRUE, TRUE,0);

  drawbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX(midbox), drawbox, TRUE, TRUE, 0); 

  drwfrm = gtk_frame_new ("main");
  gtk_container_add (GTK_CONTAINER (drawbox), drwfrm);

  notebp1 = gtk_vbox_new(FALSE, 0);
  notebp2 = gtk_vbox_new(FALSE, 0);
  notebp3 = gtk_vbox_new(FALSE, 0);

  optnbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(dtbfrm),optnbox);

  statusbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_end (GTK_BOX(optnbox), statusbox, FALSE, FALSE, 100);

////////////////////////////////////////////////////////////////////////////////
  // adjusts e barras
  pdat->barl.adj = gtk_adjustment_new (0.0, 0.0, 101.0, 0.1, 1.0, 1.0);
  pdat->barr.adj = gtk_adjustment_new (0.0, 0.0, 101.0, 0.1, 1.0, 1.0);
  
  barlensl = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barl.adj));
  gtk_box_pack_start (GTK_BOX (notebp1), barlensl, FALSE, TRUE, 0);

  barlensr = gtk_hscale_new (GTK_ADJUSTMENT (pdat->barr.adj));
  gtk_box_pack_start (GTK_BOX (notebp1), barlensr, FALSE, TRUE, 0);

  pdat->barl.lbl = gtk_label_new (pdat->barl.str);
  gtk_box_pack_start (GTK_BOX (statusbox), pdat->barl.lbl, TRUE, TRUE, 0);

  pdat->barr.lbl = gtk_label_new (pdat->barr.str);
  gtk_box_pack_start (GTK_BOX (statusbox), pdat->barr.lbl, TRUE, TRUE, 0);
////////////////////////////////////////////////////////////////////////////////
  //botões
  button = gtk_button_new_with_label("\treset\t");
  gtk_box_pack_start(GTK_BOX(optnbox), button, FALSE, FALSE ,20);

  //butão toggle com cores

  gdk_color_parse ("red", &pdat->btnlock.coloron);
  gdk_color_parse ("green", &pdat->btnlock.coloroff);
  gdk_color_parse ("cyan", &pdat->btnlock.colorhigh);
  pdat->btnlock.name = gtk_toggle_button_new_with_label(pdat->btnlock.label);

  gtk_box_pack_start(GTK_BOX(optnbox), 
		     pdat->btnlock.name, FALSE, FALSE ,2);
  gtk_widget_modify_bg (pdat->btnlock.name, 
			GTK_STATE_NORMAL, &pdat->btnlock.coloroff);
  gtk_widget_modify_bg (pdat->btnlock.name, 
			GTK_STATE_PRELIGHT, &pdat->btnlock.colorhigh);
  gtk_widget_modify_bg (pdat->btnlock.name, 
			GTK_STATE_ACTIVE, &pdat->btnlock.coloron);
  
////////////////////////////////////////////////////////////////////////////////
  //notebook 
  noteb = gtk_notebook_new();
  gtk_notebook_append_page (GTK_NOTEBOOK(noteb),notebp1,NULL);
  gtk_notebook_append_page (GTK_NOTEBOOK(noteb),notebp2,NULL);
  gtk_notebook_append_page (GTK_NOTEBOOK(noteb),notebp3,NULL);
  gtk_box_pack_start(GTK_BOX(setbox),noteb, TRUE, TRUE, 0);

////////////////////////////////////////////////////////////////////////////////
  //sinais e callbacks
  g_signal_connect_swapped (G_OBJECT (window), "destroy",
			    G_CALLBACK (gtk_main_quit), NULL);
  
  g_signal_connect (G_OBJECT (button), "clicked",
		    G_CALLBACK (set_val), pdat);

  g_signal_connect (G_OBJECT (pdat->btnlock.name), "toggled",
		    G_CALLBACK (lchange), pdat);

  g_signal_connect (G_OBJECT (pdat->barl.adj), "value-changed",
		    G_CALLBACK (upd_txt), pdat);

  g_signal_connect (G_OBJECT (pdat->barr.adj), "value-changed",
		    G_CALLBACK (upd_txt), pdat);
 

  gtk_widget_show_all(window);
  gtk_main ();
  return 0;
}
