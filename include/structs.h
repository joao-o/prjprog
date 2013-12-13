#ifndef _STRUCTSINC
#define _STRUCTSINC

typedef struct
{
  char val[20];
  GtkObject *adj;
  GtkWidget *lbl, *lock;
  char locklabel[15];
  gint lockind; // 0 = Unlocked 1 = Locked
  GdkColor color1;
  GdkColor color2;
} progdata;
#endif
