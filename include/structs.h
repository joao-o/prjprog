#ifndef _STRUCTSINC
#define _STRUCTSINC

// struct com dados para butao toggle com cores que mudam 
typedef struct
{
  char label[15];
  gint state;			// 0 = Unlocked 1 = Locked
  GdkColor coloron;
  GdkColor coloroff;
  GdkColor colorhigh;
  GtkWidget *name;
} tbtn;

typedef struct 
{
  
} rbg;

//struct com dados de uma barra de ajuste com label associada

typedef struct
{
  GtkObject *adj;
  GtkWidget *nome, *lbl;
  char str[20];
  double save;
} barradat;

//struct com dados do desenho das lentes

typedef struct
{
  double ylen; //altura das lentes
  double xwid; //espessura das lentes
  double headwid1, headwid2; //espessura dos triangulos
} drawdata;

// struct principal com os dados do programa
typedef struct
{
  barradat barl, barr;
  tbtn btnlock;
  GtkWidget *window, *drawbox;
  drawdata lensdata;
} progdata;

//tentar manter as coisas modulares e nao acrescentar coisas do tipo adj1,adj1 .
#endif
