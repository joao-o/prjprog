#ifndef _PHYSINC
#define _PHYSINC

extern double fconj       (double, double);
extern double lin         (double, double, double, double, double);
extern double alin        (double, double, double, double, double);
extern void   calcs       (draw*, lens* , lens*);
extern void   erroluneta  (progdata*);
    
#endif
