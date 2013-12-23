#ifndef _PRJCAIROINC
#define _PRJCAIROINC

extern void obj_draw();

gboolean 
on_expose_event (GtkWidget       *widget ,
		 GdkEventExpose  *event  ,
		 gpointer         data   );

gboolean 
on_configure_event (GtkWidget       *widget ,
		    GdkEventExpose  *event  ,
		    gpointer         data   );
gboolean 
change_scale (GtkWidget  *widget,
              gpointer    data );


#endif
