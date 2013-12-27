#ifndef _CAIROFUNCINC
#define _CAIROFUNCINC

extern gboolean expose_evv(GtkWidget*,GdkEventExpose*,gpointer);
extern void draw_varrow(double,double,double,double,cairo_t*);
extern void draw_axis(cairo_t*,progdata*);

#endif
