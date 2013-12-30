#include <gtk/gtk.h>
#include <cairo.h>

#include <structs.h>
#include <prjcairo.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Funcoes internas e funcoes fisicas */

//Funcao dos focos conjugados

double
fconj (double a, double b)
{
  /* (1/c) = (1/a) + (1/b) retorna f */
  return 1 / (1 / a + 1 / b);
}

//Para uma recta que passa em (x1,y1) e (x2,y2) dá y3 de um x3

double
lin (double x1, double y1, double x2, double y2, double x3)
{
  return ((y2 - y1) / (x2 - x1)) * (x3 - x2) + y2;
}

//Para uma recta que passa em (x1,y1) e (x2,y2) dá x3 de um y3

double
alin (double x1, double y1, double x2, double y2, double y3)
{
  return ((x2 - x1) / (y2 - y1)) * (y3 - y2) + x2;
}

void
calcs (draw * pts, lens * lens1, lens * lens2)
{
  pts->px[0]=0;
  pts->px[1]=*(lens1->pos);

  pts->pye[1]=*(pts->ldn)/2;
  pts->pye[0]=(pts->pye[1])-(tan(pts->ang)*pts->px[1]);

  pts->px[2]=*(lens2->pos);
  pts->px[3]=pts->px[1]+*(lens1->focus);

  pts->pye[2]=(tan(pts->ang))*pts->px[2]+pts->pye[0];
  pts->pye[3]=(tan(pts->ang))*pts->px[3]+pts->pye[0];
  pts->pyp[4]=pts->pyp[3]=pts->pyp[2]=pts->pyp[1]=pts->pye[3];
  pts->pyp[0]=(pts->pyp[1])-(tan(pts->ang)*pts->px[1]);

  pts->px[4]=pts->px[2];
  pts->pye[4]=pts->pye[1];
  pts->px[5]=*(lens2->pos)+(pts->px[3]-pts->px[2])*(-*(lens2->focus))/
             (-*(lens2->focus)+pts->px[3]-pts->px[2]);
  pts->pye[5]=lin(pts->px[3],pts->pye[3],pts->px[2],pts->pye[1],pts->px[5]);
  pts->pyp[5]=lin(pts->px[2],pts->pyp[2],
              *(lens2->pos)-*(lens2->focus),pts->pye[1],pts->px[5]);
  return;
}

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
