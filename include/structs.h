#ifndef _STRUCTSINC
#define _STRUCTSINC

#include <stdint.h> 
#define TOL 20			//Distância ente eixo optico e borda da drawbox

// struct com dados para butao toggle com cores que mudam 
typedef struct tbtn
{
  char label[15];
  GdkColor coloron;
  GdkColor coloroff;
  GdkColor colorhigh;
  GtkWidget *name;
} tbtn;

//struct com dados de uma barra de ajuste com label associada

typedef struct bardat bardat;
struct bardat
{
  GtkObject *adj;
  GtkWidget *nome, *lbl;
  char str[40];
  double save;
  bardat *alt;
};


//struct com dados do desenho das lentes

typedef struct drawdata
{
  double ylen; //altura das lentes
  double xwid; //espessura das lentes
  double headwid1, headwid2; //espessura dos triangulos
} drawdata;

//stuct com dados das lentes
typedef struct lens
{
  double *pos, *focus;
} lens;

//struct com coisas do rato
typedef struct rodent
{
  unsigned char trap; //trap = [1,4] rato clicado, 0 otherwise
  double nestx;
  double nesty; //posição x e y onde é clicado
  double path1;
  double path2;
} rodent;

typedef struct flag
{
  uint8_t virt :1;
  uint8_t dist :1;
  uint8_t lock :1;
  uint8_t rem  :5;
} flag;

typedef struct physdata
{
  double poslc;
  double posld;
  double fc;
  double fd;
  double axis;
  double ldist;
} physdata;

// struct principal com os dados do programa
typedef struct progdata
{
  bardat barl, barr;
  bardat barfc, barfd;
  bardat barang;
  bardat barxx;
  bardat rgbscl[3];
  tbtn btnlock;
  GtkWidget *window, *drawbox, *lenstype, 
    *virtbtn, *distbtn, *combocolor;
  drawdata lensdata;
  lens lnsc,lnsd;
  flag flg;
  rodent mouse;
  physdata phys;
  GdkColor color[6], *ptclr;
} progdata;

//tentar manter as coisas modulares e nao acrescentar coisas do tipo adj1,adj1 .
#endif
