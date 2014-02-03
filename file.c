#include <gtk/gtk.h>
#include <cairo.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <structs.h>

gboolean
savedata (GtkWidget * widget, progdata * pdat)
{
  FILE *fp;
  filestruct *data;
  double *i, *j;
  GtkObject **k;

  fp = fopen (gtk_entry_get_text (GTK_ENTRY (pdat->field)), "wt");
  if (fp == NULL)
    return TRUE;

  data = malloc (sizeof (filestruct));
  data->check = 0;

  for (i = &(pdat->barl.save), j = &data->sbarl;
       j <= &data->sbarxx; i = ((void *) i) + sizeof (bardat), j++)
    {
      *j = *i;
      data->check += *i;
    }

  for (k = &(pdat->barl.adj), j = &data->vbarl;
       j <= &data->vbarxx; k = ((void *) k) + sizeof (bardat), j++)
    {
      *j = GTK_ADJUSTMENT (*k)->value;
      data->check += GTK_ADJUSTMENT (*k)->value;
    }
  memcpy (&data->color, &pdat->color, sizeof (GdkColor) * 8);
  memcpy (&data->flg, &pdat->flg, 1);

  fwrite (data, sizeof (filestruct), 1, fp);
  fclose (fp);
  free (data);
  return TRUE;
}

gboolean
loaddata (GtkWidget * widget, progdata * pdat)
{
  FILE *fp;
  filestruct *data;
  double *i, *j;
  GtkObject **k;
  double check = 0;

  fp = fopen (gtk_entry_get_text (GTK_ENTRY (pdat->field)), "rt");
  if (fp == NULL)
    return TRUE;

  data = malloc (sizeof (filestruct));
  fread (data, sizeof (filestruct), 1, fp);

  for (i = &data->sbarl; i <= &data->vbarxx; i++)
    check += *i;

  if (check != data->check)
    {
      printf("check falhou\n");
      return TRUE;
    }

  for (i = &(pdat->barl.save), j = &data->sbarl;
       j <= &data->sbarxx; i = ((void *) i) + sizeof (bardat), j++)
    *i = *j;

  for (k = &(pdat->barl.adj), j = &data->vbarl;
       j <= &data->vbarxx; k = ((void *) k) + sizeof (bardat), j++)
    GTK_ADJUSTMENT (*k)->value = *j;

  memcpy (&pdat->color, &data->color, sizeof (GdkColor) * 8);
  memcpy (&pdat->flg, &data->flg, 1);

  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barl.adj), "value-changed");
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfc.adj), "value-changed");
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barfd.adj), "value-changed");
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barr.adj), "value-changed");
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barang.adj), "value-changed");
  g_signal_emit_by_name (GTK_ADJUSTMENT (pdat->barxx.adj), "value-changed");

  fclose (fp);
  free (data);
  return TRUE;
}

gboolean
filemenu (GtkWidget * widget, progdata * pdat)
{
  GtkWidget *mainbox, *dialog, *flabel, *fbox, *btnbox, *svbtn, *ldbtn;

  dialog = gtk_dialog_new_with_buttons ("Menu de Ficheiros",
					GTK_WINDOW (pdat->window),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_STOCK_OK,
					GTK_RESPONSE_NONE, NULL);

  gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

  mainbox = gtk_vbox_new (FALSE, 10);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox),
		      mainbox, TRUE, TRUE, 10);

  fbox = gtk_hbox_new (FALSE, 10);
  flabel = gtk_label_new ("Ficheiro:");
  pdat->field = gtk_entry_new ();
  gtk_box_pack_start (GTK_BOX (mainbox), fbox, FALSE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX (fbox), flabel, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX (fbox), pdat->field, TRUE, TRUE, 10);

  btnbox = gtk_hbox_new (FALSE, 10);
  svbtn = gtk_button_new_with_label ("Save");
  ldbtn = gtk_button_new_with_label ("Load");
  gtk_box_pack_start (GTK_BOX (mainbox), btnbox, FALSE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX (btnbox), svbtn, TRUE, TRUE, 10);
  gtk_box_pack_start (GTK_BOX (btnbox), ldbtn, TRUE, TRUE, 10);

  g_signal_connect_swapped (dialog, "response",
			    G_CALLBACK (gtk_widget_destroy), dialog);
  g_signal_connect (G_OBJECT (ldbtn), "clicked", G_CALLBACK (loaddata), pdat);
  g_signal_connect (G_OBJECT (svbtn), "clicked", G_CALLBACK (savedata), pdat);

  gtk_widget_show_all (dialog);
  return TRUE;
}
