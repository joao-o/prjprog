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
colorchange(GtkWidget * widget, progdata *pdat)
{
  int i;
  gtk_color_selection_get_current_color(GTK_COLOR_SELECTION
					(pdat->colwhl), pdat->ptclr);
 
  for(i=0;i<6;i++)
    {      
      gtk_widget_modify_bg (pdat->btn[i],
			    GTK_STATE_NORMAL, &pdat->color[i]);
      gtk_widget_modify_bg (pdat->btn[i],
			    GTK_STATE_PRELIGHT, &pdat->color[i]);
      gtk_widget_modify_bg (pdat->btn[i],
			    GTK_STATE_ACTIVE, &pdat->color[i]);
    }
 
  gtk_widget_queue_draw (pdat->window);
  return TRUE;
}
 
gboolean
colorreset (GtkWidget * widget, progdata *pdat)
{
  int i;

  gdk_color_parse ("#FF8C00", &pdat->color[0]);
  gdk_color_parse ("#3636FF", &pdat->color[1]);
  gdk_color_parse ("#00CC33", &pdat->color[2]);
  gdk_color_parse ("#66CC33", &pdat->color[3]);
  gdk_color_parse ("#FFFF00", &pdat->color[4]);
  gdk_color_parse ("#00B3FF", &pdat->color[5]);

  for(i=0;i<6;i++)
    {      
      gtk_widget_modify_bg (pdat->btn[i],
			    GTK_STATE_NORMAL, &pdat->color[i]);
      gtk_widget_modify_bg (pdat->btn[i],
			    GTK_STATE_PRELIGHT, &pdat->color[i]);
      gtk_widget_modify_bg (pdat->btn[i],
			    GTK_STATE_ACTIVE, &pdat->color[i]);
    }

  gtk_widget_queue_draw (pdat->window);

  return TRUE;
}

gboolean
colorselec (GtkWidget *widget, progdata *pdat)
{
  int i;
  for(i = 0;i<6;i++)
    {
      if((GTK_TOGGLE_BUTTON(pdat->btn[i]))->active)
	{
	  pdat->ptclr = &pdat->color[i];

	  gtk_color_selection_set_current_color(GTK_COLOR_SELECTION
						(pdat->colwhl),
						pdat->ptclr);

	  break;
	}
    }
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
  
  gtk_window_set_default_size (GTK_WINDOW(dialog), 500, 300);
  gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

  mainbox = gtk_hbox_new(FALSE, 30);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG(dialog)->vbox), 
		      mainbox, TRUE, TRUE, 30);
  btnbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start (GTK_BOX (mainbox), btnbox, TRUE, TRUE, 20);
  colorbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_end (GTK_BOX (mainbox), colorbox, TRUE, TRUE, 20);

 
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

  pdat->colwhl = gtk_color_selection_new();
  gtk_box_pack_end (GTK_BOX (colorbox), pdat->colwhl, TRUE, TRUE, 0);
  gtk_color_selection_set_current_color(GTK_COLOR_SELECTION
						(pdat->colwhl),
						&pdat->color[0]);
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
  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pdat->btn[0]), TRUE);

  g_signal_connect (G_OBJECT (reset), "clicked",
		    G_CALLBACK (colorreset), pdat);

  g_signal_connect (G_OBJECT (pdat->colwhl), "color-changed",
		    G_CALLBACK (colorchange), pdat);

  g_signal_connect_swapped (dialog, "response", 
			    G_CALLBACK (gtk_widget_destroy), dialog);

  gtk_widget_show_all (dialog);
  return TRUE;
}
