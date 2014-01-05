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
