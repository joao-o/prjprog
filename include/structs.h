#ifndef _STRUCTSINC
#define _STRUCTSINC

#define TOL 20			//Distância ente eixo optico e borda da drawbox

// struct com dados para butao toggle com cores que mudam 
typedef struct
{
  char label[15];
  char state;			// 0 = Unlocked 1 = Locked
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

//struct c/ os pontos a desenhar
typedef struct
{
  double px1[4],pe1[4],pp1[4];
  double px2[4],pe2[4],pp2[4];
  //px posição em x dos raios 
  //pe posiçao em y do raio que passa no eixo
  //pp posiçao em y do raio que sai paralelo da lente1
  //1 calculos da lente 1, 2 ...
  gint *lup,*ldn,*llt,*lrt;
  double ang,ylen;
  char vmask;
  char i;
} draw;

//struct com coisas do rato
typedef struct
{
  char trap; //trap = [1,4] rato clicado, 0 otherwise
  double nestx;
  double nesty; //posição x e y onde é clicado
  double path1;
  double path2;
} rodent;

// struct principal com os dados do programa
typedef struct
{
  barradat barl, barr, barfc, barfd, barang;
  tbtn btnlock;
  GtkWidget *window, *drawbox;
  drawdata lensdata;
  draw pts;
  lens lnsc,lnsd;
  physdata lensvalue;
  char virt; //raios virtuais 1 = On 0 = Off
  char dist;
  rodent mouse;
} progdata;

//tentar manter as coisas modulares e nao acrescentar coisas do tipo adj1,adj1 .
#endif
