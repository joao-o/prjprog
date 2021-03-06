#ifndef _STRUCTSINC
#define _STRUCTSINC

#include <stdint.h> 
#define TOL 20			//Distância ente eixo optico e borda da drawbox
#define XWID 3                  //espessura das lentes finas
#define RAY 1.5
#define OFFANG 22
#define OFFFOC 21
#define OFFSCL 11
#define OFFPOS 21

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
  GtkWidget *lbl;
  char str[40];
  double save;
  bardat *alt;
};


//struct com dados do desenho das lentes

typedef struct drawdata
{
  double ylen; //altura das lentes
  double headwid1, headwid2; //espessura dos triangulos
} drawdata;

//stuct com dados das lentes
typedef struct lenspt
{
  double *pos, *focus;
} lenspt;

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
  uint8_t ltype:1;
  uint8_t rem  :4;
} flag;

typedef struct lens
{
    double pos;
    double focus;
} lens;

typedef struct physdata
{
  lens c,d;
  double axis;
  double ldist;
  double *scl; 
} physdata;

// struct principal com os dados do programa
typedef struct progdata
{
  bardat barl, barr;
  bardat barfc, barfd;
  bardat barang;
  bardat barxx;
  tbtn btnlock;
  GtkWidget *window, *drawbox, *lenstype, 
    *virtbtn, *distbtn, *btn[8],
    *colwhl,*field;
  drawdata ldat;
  lenspt lnsc,lnsd;
  flag flg;
  rodent mouse;
  physdata phys;
  GdkColor color[8], *ptclr;
} progdata;

typedef struct filestruct
{
  double sbarl,sbarr,sbarfc,sbarfd,sbarang,sbarxx;
  double vbarl,vbarr,vbarfc,vbarfd,vbarang,vbarxx;
  GdkColor color[8];
  flag flg;
  double check;
} filestruct;

#endif
