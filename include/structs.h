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

//struct com dados de uma barra de ajuste com label associada

typedef struct
{
  GtkObject *adj;
  GtkWidget *nome, *lbl;
  char str[40];
  double save;
} barradat;

//struct com dados do desenho das lentes

typedef struct
{
  double ylen; //altura das lentes
  double xwid; //espessura das lentes
  double headwid1, headwid2; //espessura dos triangulos
} drawdata;

//struct com valores físicos do programa
typedef struct
{
  double focalc; //distancia focal da lente convergente
  double focald; //distancia focal da lente divergente (em modulo)
  double imgmidy, imgmidx; //altura/posicao da imagem intermédia
  double imgfiny, imgfinx; //altura/posicao da imagem final
} physdata;

// struct principal com os dados do programa
typedef struct
{
  barradat barl, barr, barfc, barfd;
  tbtn btnlock;
  GtkWidget *window, *drawbox;
  drawdata lensdata;
  physdata lensvalue;
  char virt; //raios virtuais 1 = On 0 = Off
} progdata;

//tentar manter as coisas modulares e nao acrescentar coisas do tipo adj1,adj1 .
#endif
