#include <gtk/gtk.h>
#include <cairo.h>

#include <structs.h>
#include <prjcairo.h>

#include <stdio.h>
#include <stdlib.h>

// Funções de UI não associadas directamente a callbacks

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
  
   g_signal_connect_swapped (dialog, "response", G_CALLBACK (gtk_widget_destroy), dialog);

   gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), label);
   gtk_widget_show_all (dialog);
}

gboolean
colorselec (progdata *pdat)
{
  int i;
  i = gtk_combo_box_get_active(GTK_COMBO_BOX(pdat->combocolor));
  pdat->ptclr = &pdat->color[i];
  gtk_adjustment_set_value (GTK_ADJUSTMENT (pdat->rgbscl[0].adj), 
			    pdat->ptclr->red/255);
  gtk_adjustment_set_value (GTK_ADJUSTMENT (pdat->rgbscl[1].adj), 
			    pdat->ptclr->green/255);
  gtk_adjustment_set_value (GTK_ADJUSTMENT (pdat->rgbscl[2].adj), 
			    pdat->ptclr->blue/255);
  
  return TRUE;
}

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

}
gboolean
colormenu (progdata *dat)
{
  progdata *pdat = (progdata *) dat;
  GtkWidget *dialog, *btn, *tbl, *rgbbox, *rgbfrm, *rgbbar[3];
  GdkColor base[0];
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
  gtk_table_attach_defaults (GTK_TABLE (tbl), rgbfrm, 
			     1, 2, 0, 3);
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

  gtk_table_attach_defaults (GTK_TABLE (tbl), pdat->combocolor, 
			     0, 1, 0, 1);
  
  btn = gtk_toggle_button_new_with_label ("Restaurar Cores");
  gtk_table_attach_defaults (GTK_TABLE (tbl), btn, 
			     0, 1, 2, 3);

  gdk_color_parse ("red", &base[0]);
  gdk_color_parse ("green", &base[1]);
  gdk_color_parse ("blue", &base[2]);

  for(i=0;i<3;i++)
    {
      pdat->rgbscl[i].adj = gtk_adjustment_new (100, 0, 255, 0.01, 1.0, 1.0);
      rgbbar[i] = gtk_vscale_new (GTK_ADJUSTMENT (pdat->rgbscl[i].adj));
      gtk_container_add (GTK_CONTAINER (rgbbox), rgbbar[i]);
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

}
