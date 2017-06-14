#ifndef CALL_BACKS_H_INCLUDED
#define CALL_BACKS_H_INCLUDED

#include <gtk/gtk.h>
#include <slope/slope.h>

typedef struct node {
    float val_c,val_t,val_p;
    struct node * next;
} node_t;

typedef struct
{
    GtkWidget **p_widg;
    char **dato;
}arguments;

typedef struct
{
    GtkLabel **label1,**label2,**label3;
}labels_args;

typedef struct
{
    node_t **node_ptr;
    int *fd ;
    float *f1,*f2,*f3;
    labels_args labels;
    GIOChannel **channel;
}floats_arg;

typedef struct
{
    GtkWidget **p_widg;
    GtkWidget **p_widg_l;
    int *fd ;
}arguments_com;

typedef struct
{
    char **f_outC,**f_outP,**f_outZ;
    char *comando;
    int *fd;
    GtkWidget **bt1,**bt2,**bt3;

}arguments_out;

typedef struct
{
	GtkCheckButton **auto_check;
	GtkWidget **view;
    SlopeScale **scl;
    node_t **head;
    float **yc,**yt,**yp,**x;
}gr_data;


void add_node(node_t *ptr);

void llenar_lista(GtkComboBoxText *cmb_box);

void actualizar_lista(GtkWidget *widget, gpointer *data);

void set_dato_corriente(GtkWidget *widget, arguments *data);
void set_dato_potencia(GtkWidget *widget, arguments *data);
void set_dato_impedancia(GtkWidget *widget, arguments *data);

void abrir_com(GtkWidget *widget, arguments_com *data);

gboolean* data_recibida(GIOChannel *source,GIOCondition condition,floats_arg *data);

void start_stop(GtkWidget *widget, arguments_out *data);

void command_d(GtkWidget *widget, gpointer *data);
void command_p(GtkWidget *widget, gpointer *data);
void command_z(GtkWidget *widget, gpointer *data);

gboolean new_data(gr_data *user_data);
gboolean get_new_data(floats_arg *user_data);

void save_file(GtkWidget *widg, node_t *data);

#endif // CALL_BACKS_H_INCLUDED
