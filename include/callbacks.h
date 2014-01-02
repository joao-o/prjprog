#ifndef _CALLBACKS_PROG
#define _CALLBACKS_PROG

extern gboolean upd_adj     (GtkWidget*, gpointer);
extern void     upd_mod     (barradat*);
extern gboolean upd_adj_free(GtkWidget*, gpointer);
extern gboolean cfg_event   (GtkWidget*, gpointer);
extern gboolean set_val     (GtkWidget*, gpointer);
extern gboolean lchange     (GtkWidget*, gpointer);
extern gboolean virtchange  (GtkWidget*, gpointer);
extern gboolean distchange  (GtkWidget*, gpointer);
extern gboolean luneta      (GtkWidget*, gpointer);
extern gboolean titanmouse  (GtkWidget*, GdkEvent*, gpointer);

#endif
