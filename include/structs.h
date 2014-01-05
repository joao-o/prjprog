#ifndef _STRUCTSINC
#define _STRUCTSINC

#define TOL 20			//Distância ente eixo optico e borda da drawbox

// struct com dados para butao toggle com cores que mudam 
typedef struct
{
  char label[15];
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

//stuct com dados das lentes
typedef struct
{
  double *pos,*focus;
} lens;

//struct com valores físicos do programa
typedef struct
{
  double focalc; //distancia focal da lente convergente
  double focald; //distancia focal da lente divergente (em modulo)
  double imgmidy, imgmidx; //altura/posicao da imagem intermédia
  double imgfiny, imgfinx; //altura/posicao da imagem final
} physdata;

//struct com coisas do rato
typedef struct
{
  unsigned char trap; //trap = [1,4] rato clicado, 0 otherwise
  double nestx;
  double nesty; //posição x e y onde é clicado
  double path1;
  double path2;
} rodent;

typedef struct
{
  unsigned char virt :1;
  unsigned char dist :1;
  unsigned char rem  :6;
} flag;

// struct principal com os dados do programa
typedef struct
{
  barradat barl, barr, barfc, barfd, barang, barxx, baryy;
  tbtn btnlock;
  GtkWidget *window, *drawbox;
  drawdata lensdata;
  lens lnsc,lnsd;
  physdata lensvalue;
  flag flg;
  rodent mouse;
} progdata;

//tentar manter as coisas modulares e nao acrescentar coisas do tipo adj1,adj1 .
#endif
