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
  char val1[20], val2[20];
  GtkObject *adj1, *adj2;
  GtkWidget *lbl1, *lbl2;
  tbtn btnbarra;
  double adjsave;
} progdata;


#endif
