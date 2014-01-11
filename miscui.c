#include <gtk/gtk.h>
#include <cairo.h>

#include <structs.h>
#include <prjcairo.h>

#include <stdio.h>
#include <stdlib.h>

// Funções de UI

void 
erroluneta (progdata * dat)
{
  progdata *pdat;
  pdat = (progdata *) dat;
  GtkWidget *dialog, *label;
  char *message = "A criação de uma luneta FALHOU.\n\n"
    "Para formar uma luneta terrestre\n"
    "assegure que a distância focal da lente convergente\n"
    "é maior que a distância focal da lente divergente.";

  dialog = gtk_dialog_new_with_buttons ("ERRO a criar luneta",
					GTK_WINDOW(pdat->window),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_STOCK_OK,
					GTK_RESPONSE_NONE,
					NULL);

  gtk_window_set_default_size (GTK_WINDOW(dialog), 400, 150);
  gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

  label = gtk_label_new (message);

  g_signal_connect_swapped (dialog, "response", 
			    G_CALLBACK (gtk_widget_destroy), dialog);

  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), label);
  gtk_widget_show_all (dialog);
}

gboolean
colorselec (progdata *pdat)
{
  int i;

  return TRUE;
}

gboolean
colormenu (GtkWidget *widget, progdata * pdat)
{
  GtkWidget *dialog, *mainbox, *btnbox,
    *colorbox, *reset;
  int i;
 
  dialog = gtk_dialog_new_with_buttons ("Menu de Cores",
					GTK_WINDOW(pdat->window),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_STOCK_OK,
					GTK_RESPONSE_NONE,
					NULL);
  
  gtk_window_set_default_size (GTK_WINDOW(dialog), 400, 150);
  gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

  mainbox = gtk_hbox_new(FALSE, 0);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), mainbox);
  btnbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start (GTK_BOX (mainbox), btnbox, TRUE, TRUE, 0);
  colorbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_end (GTK_BOX (mainbox), colorbox, TRUE, TRUE, 0);

 
  pdat->btn[0] =  gtk_radio_button_new_with_label (NULL, "Lente Convergente");
  pdat->btn[1] =  gtk_radio_button_new_with_label (gtk_radio_button_group 
						   (GTK_RADIO_BUTTON 
						    (pdat->btn[0])),
						   "Lente Divergente");

  pdat->btn[2] =  gtk_radio_button_new_with_label (gtk_radio_button_group 
						   (GTK_RADIO_BUTTON 
						    (pdat->btn[1])),
						   "Primeira Imagem");

  pdat->btn[3] =  gtk_radio_button_new_with_label (gtk_radio_button_group 
						   (GTK_RADIO_BUTTON 
						    (pdat->btn[2])),
						   "Segunda Imagem");

  pdat->btn[4] =  gtk_radio_button_new_with_label (gtk_radio_button_group 
						   (GTK_RADIO_BUTTON 
						    (pdat->btn[3])),
						   "Raios Reais");

  pdat->btn[5] =  gtk_radio_button_new_with_label (gtk_radio_button_group 
						   (GTK_RADIO_BUTTON 
						    (pdat->btn[4])),
						   "Raios Virtuais");

  reset =  gtk_button_new_with_label ("Cores Predefinidas");
  gtk_box_pack_end (GTK_BOX (btnbox), reset, FALSE, FALSE, 0);

  g_signal_connect_swapped (dialog, "response", 
			    G_CALLBACK (gtk_widget_destroy), dialog);

  for(i=0;i<6;i++)
    {
      gtk_box_pack_start (GTK_BOX (btnbox), pdat->btn[i], FALSE, FALSE, 0);
         
      gtk_widget_modify_bg (pdat->btn[i],
			    GTK_STATE_NORMAL, &pdat->color[i]);
      gtk_widget_modify_bg (pdat->btn[i],
			    GTK_STATE_PRELIGHT, &pdat->color[i]);
      gtk_widget_modify_bg (pdat->btn[i],
			    GTK_STATE_ACTIVE, &pdat->color[i]);
      
      g_signal_connect (G_OBJECT (pdat->btn[i]), "clicked",
		   G_CALLBACK (colorselec), pdat);
    }


 gtk_widget_show_all (dialog);
 return TRUE;
}

/*
gboolean
colorchange (GtkWidget * widget, progdata *pdat)
{

 if (GTK_OBJECT (widget) == pdat->rgbscl[0].adj)
    pdat->ptclr->red = (GTK_ADJUSTMENT(pdat->rgbscl[0].adj))->value*255;
 if (GTK_OBJECT (widget) == pdat->rgbscl[0].adj)
    pdat->ptclr->green = (GTK_ADJUSTMENT(pdat->rgbscl[1].adj))->value*255;
 if (GTK_OBJECT (widget) == pdat->rgbscl[0].adj)
    pdat->ptclr->blue = (GTK_ADJUSTMENT(pdat->rgbscl[2].adj))->value*255;
 printf("%d",pdat->ptclr->blue);


 return TRUE;
/*
}
gboolean
colormenu (GtkWidget *widget, gpointer dat)
{
  progdata *pdat = (progdata *) dat;
  GtkWidget *dialog, *btn, *mainbox, *box2, *rgbbox, *rgbfrm, *rgbbar[3], *tbl;
  GdkColor base[3];
  int i;

  dialog = gtk_dialog_new_with_buttons ("Menu de Cores",
                                         GTK_WINDOW(pdat->window),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_NONE,
                                         NULL);

  tbl = gtk_table_new (3 , 2, TRUE);
  gtk_table_set_row_spacings(GTK_TABLE(tbl), 5);
  gtk_table_set_col_spacings(GTK_TABLE(tbl), 5);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), tbl);

  rgbbox =  gtk_hbox_new (FALSE, 0);
  rgbfrm = gtk_frame_new ("RGB");
  gtk_box_pack_start(GTK_BOX(mainbox),rgbfrm,TRUE,TRUE,0);
  gtk_container_add (GTK_CONTAINER (rgbfrm), rgbbox);

  pdat->combocolor = gtk_combo_box_text_new();
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pdat->combocolor),
				 "Lente Convergente");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pdat->combocolor),
				 "Lente Divergente");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pdat->combocolor),
				 "Primeira Imagem");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pdat->combocolor),
				 "Segunda Imagem ");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pdat->combocolor),
				 "Raios Reais");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pdat->combocolor),
				 "Raios Virtuais");

  /*gtk_table_attach_defaults (GTK_TABLE (tbl), pdat->combocolor, 
			     0, 1, 0, 1);
  
  box2 = gtk_vbox_new (FALSE,0);
  gtk_box_pack_start(GTK_BOX(box2),pdat->combocolor,TRUE,TRUE,0);
  btn = gtk_toggle_button_new_with_label ("Restaurar Cores");
  gtk_box_pack_start(GTK_BOX(box2),btn,TRUE,TRUE,0);

  gtk_box_pack_start(GTK_BOX(mainbox),box2,TRUE,TRUE,0);
  


/*
  gdk_color_parse ("red", &base[0]);
  gdk_color_parse ("green", &base[1]);
  gdk_color_parse ("blue", &base[2]);

  for(i=0;i<3;i++)
    {
      pdat->rgbscl[i].adj = gtk_adjustment_new (100, 0, 255, 0.01, 1.0, 1.0);
      rgbbar[i] = gtk_vscale_new (GTK_ADJUSTMENT (pdat->rgbscl[i].adj));
      gtk_container_add (GTK_CONTAINER (rgbbox), rgbbar[i]);
      
      /*
      gtk_widget_modify_bg (rgbbar[i],
			    GTK_STATE_NORMAL, &base[i]);
      gtk_widget_modify_bg (rgbbar[i],
			    GTK_STATE_PRELIGHT, &base[i]);
      gtk_widget_modify_bg (rgbbar[i],
			    GTK_STATE_ACTIVE, &base[i]);
      
    }

  g_signal_connect (G_OBJECT (pdat->rgbscl[0].adj), "value-changed",
		    G_CALLBACK (colorchange), pdat);
  g_signal_connect (G_OBJECT (pdat->rgbscl[1].adj), "value-changed",
		    G_CALLBACK (colorchange), pdat);
  g_signal_connect (G_OBJECT (pdat->rgbscl[2].adj), "value-changed",
		    G_CALLBACK (colorchange), pdat);

  g_signal_connect (G_OBJECT (pdat->combocolor), "changed",
		    G_CALLBACK (colorselec), pdat);

  g_signal_connect (GTK_STOCK_OK, "clicked", G_CALLBACK (gtk_widget_destroy), 
		    dialog);

  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), tbl);

  gtk_widget_show_all (dialog);

}*/
