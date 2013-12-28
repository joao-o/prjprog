#ifndef _CAIROFUNCINC
#define _CAIROFUNCINC

extern void draw_line (cairo_t*, double, double, double, double);
extern double dsign(double); 
extern gboolean expose_evv(GtkWidget*,GdkEventExpose*,gpointer);
extern void draw_varrow(double,double,double,double,cairo_t*);

#endif
