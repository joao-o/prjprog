#ifndef _STRUCTSINC
#define _STRUCTSINC

typedef struct {
  char label[15];
  gint state; // 0 = Unlocked 1 = Locked
  GdkColor coloron;
  GdkColor coloroff;
  GdkColor colorhigh;
  GtkWidget *name;
} tbtn;

typedef struct
{
  char val[20];
  GtkObject *adj;
  GtkWidget *lbl;
  tbtn btnbarra;
  double adjsave;
} progdata;


#endif
