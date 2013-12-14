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

  GtkWidget *window, *button, *bar; 
  
  // boxes
  GtkWidget *vbox1, *topbox, *midbox, *setbox, *datbox, *drawbox, *noteb, *notebp1, *optnbox;
  
  //frames
  GtkWidget *dtbfrm, *drwfrm;

  pdat = calloc (1, sizeof (progdata));
  strcpy (pdat->val, "value= ");

  pdat->btnbarra.state=0;
  sprintf(pdat->btnbarra.label," Unlocked ");

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 720, 576);
  gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);

////////////////////////////////////////////////////////////////////////////////
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

  optnbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(dtbfrm),optnbox);

////////////////////////////////////////////////////////////////////////////////
  pdat->adj = gtk_adjustment_new (0.0, 0.0, 101.0, 0.1, 1.0, 1.0);
  
  bar = gtk_hscale_new (GTK_ADJUSTMENT (pdat->adj));
  gtk_box_pack_start (GTK_BOX (optnbox), bar, FALSE, TRUE, 0);

  pdat->lbl = gtk_label_new (pdat->val);
  gtk_box_pack_start (GTK_BOX (topbox), pdat->lbl, TRUE, TRUE, 0);

  noteb = gtk_notebook_new();
  gtk_notebook_append_page (GTK_NOTEBOOK(noteb),notebp1,NULL);
  gtk_box_pack_start(GTK_BOX(setbox),noteb, TRUE, TRUE, 0);

  button = gtk_button_new_with_label("\treset\t");
  gtk_box_pack_start(GTK_BOX(optnbox), button, FALSE, FALSE ,20);

  //butão toggle com cores

  gdk_color_parse ("red", &pdat->btnbarra.coloron);
  gdk_color_parse ("green", &pdat->btnbarra.coloroff);
  gdk_color_parse ("cyan", &pdat->btnbarra.colorhigh);
  pdat->btnbarra.name = gtk_toggle_button_new_with_label(pdat->btnbarra.label);
  gtk_box_pack_start(GTK_BOX(optnbox), pdat->btnbarra.name, FALSE, FALSE ,2);
  gtk_widget_modify_bg (pdat->btnbarra.name, GTK_STATE_NORMAL, &pdat->btnbarra.coloroff);
  gtk_widget_modify_bg (pdat->btnbarra.name, GTK_STATE_PRELIGHT, &pdat->btnbarra.colorhigh);
  gtk_widget_modify_bg (pdat->btnbarra.name, GTK_STATE_ACTIVE, &pdat->btnbarra.coloron);
  

////////////////////////////////////////////////////////////////////////////////
  g_signal_connect_swapped (G_OBJECT (window), "destroy",
			    G_CALLBACK (gtk_main_quit), NULL);
  
  g_signal_connect (G_OBJECT (button), "clicked",
		    G_CALLBACK (set_val), pdat);

  g_signal_connect (G_OBJECT (pdat->btnbarra.name), "toggled",
		    G_CALLBACK (lchange), pdat);

  g_signal_connect (G_OBJECT (pdat->adj), "value-changed",
		    G_CALLBACK (upd_txt), pdat);
 

  gtk_widget_show_all(window);
  gtk_main ();
  return 0;
}
