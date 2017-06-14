#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>


#include <slope/slope.h>
#include <math.h>

#include "call_backs.h"


int main (int argc, char *argv[])
{
    GtkBuilder *constructor;
    GtkWindow *ventana;
    GtkBox *box;
    GtkButton *bt_setCorriente,*bt_setPotencia,*bt_setImpedancia,*bt_conectar,*bt_r_serial,*bt_save;
    GtkRadioButton *modoCorriente,*modoPotencia,*modoImpedancia;
    GtkToggleButton *boton_on_of;
    GtkEntry *corriente_entry,*potencia_entry,*impedancia_entry;
    GtkLabel *tension_label,*corriente_label,*potencia_label,*estado_conexion_label;
    GtkComboBoxText *list_puertos;
    GtkCheckButton *autoUpdate_check;

    SlopeScale *scale1;
    GtkWidget *view;
    SlopeFigure *fig;

    node_t *head = NULL, *current = NULL;
    double *corr = NULL,*tens = NULL,*pot = NULL,*x = NULL;

    char corriente_d[] = "00.00",potencia_d[] = "00.00",impedancia_d[] = "00.00";
    float tension_ent = 0,corriente_ent = 0,potencia_ent = 0;
    arguments corriente_arg={&corriente_entry,&corriente_d},potencia_arg={&potencia_entry,&potencia_d},impedancia_arg={&impedancia_entry,&impedancia_d};

    int port = 0;
    GIOChannel *c_puerto = NULL;

    labels_args labels_dt = {&tension_label,&corriente_label,&potencia_label};
    floats_arg datos_in = {&current,&port,&tension_ent,&corriente_ent,&potencia_ent,labels_dt,&c_puerto};
    arguments_com com_arg ={&list_puertos,&estado_conexion_label,&port};

    char comando = 'c';
    arguments_out datos_out = {&corriente_d,&potencia_d,&impedancia_d,&comando,&port,&modoCorriente,&modoImpedancia,&modoPotencia};

    gr_data graf_data = {&autoUpdate_check,&view,&scale1, &head,&corr,&tens,&pot,&x};


    gtk_init(&argc, &argv);

    head = malloc(sizeof(node_t));
    head->next = NULL;
    current = head;

    scale1 = slope_xyscale_new_axis("Muestras", "Amplitud", "Parametros");
    fig = slope_figure_new();
    slope_figure_add_scale(fig,scale1);
    view = slope_view_new_with_figure(fig);


    constructor = gtk_builder_new();
    gtk_builder_add_from_file(constructor, "proyecto_final.glade", NULL);

    ventana = GTK_WINDOW(gtk_builder_get_object(constructor,"ventana_principal"));
    g_signal_connect(G_OBJECT(ventana),"destroy",G_CALLBACK(gtk_main_quit),NULL);

    bt_setCorriente = GTK_BUTTON(gtk_builder_get_object(constructor,"set_corriente"));
    bt_setPotencia = GTK_BUTTON(gtk_builder_get_object(constructor,"set_potencia"));
    bt_setImpedancia = GTK_BUTTON(gtk_builder_get_object(constructor,"set_impedancia"));
    bt_r_serial = GTK_BUTTON(gtk_builder_get_object(constructor,"refresh_serial"));
    bt_conectar = GTK_BUTTON(gtk_builder_get_object(constructor,"boton_conectar_serial"));
    bt_save = GTK_BUTTON(gtk_builder_get_object(constructor, "save"));

    modoCorriente = GTK_RADIO_BUTTON(gtk_builder_get_object(constructor,"boton_modo_corriente"));
    modoPotencia = GTK_RADIO_BUTTON(gtk_builder_get_object(constructor,"boton_modo_potencia"));
    modoImpedancia = GTK_RADIO_BUTTON(gtk_builder_get_object(constructor,"boton_modo_impedancia"));


    boton_on_of = GTK_TOGGLE_BUTTON(gtk_builder_get_object(constructor,"boton_on_off"));

    autoUpdate_check = GTK_CHECK_BUTTON(gtk_builder_get_object(constructor, "checkbutton1"));

    corriente_entry = GTK_ENTRY(gtk_builder_get_object(constructor,"ent_corriente"));
    potencia_entry = GTK_ENTRY(gtk_builder_get_object(constructor,"ent_potencia"));
    impedancia_entry = GTK_ENTRY(gtk_builder_get_object(constructor,"ent_impedancia"));

    corriente_label = GTK_LABEL(gtk_builder_get_object(constructor,"corriente_entrada"));
    potencia_label = GTK_LABEL(gtk_builder_get_object(constructor,"potencia_entrada"));
    tension_label = GTK_LABEL(gtk_builder_get_object(constructor,"tension_entrada"));
    estado_conexion_label = GTK_LABEL(gtk_builder_get_object(constructor,"estado_conexion"));

    list_puertos = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(constructor,"lista_seriales"));

    box = GTK_BOX(gtk_builder_get_object(constructor,"box12"));
    gtk_box_pack_start(box, view, TRUE, TRUE, (guint)0);
    gtk_box_reorder_child(box, view, 0);

    gtk_widget_set_hexpand (view,TRUE);
    gtk_widget_set_hexpand (view,TRUE);

    g_signal_connect(G_OBJECT(bt_setCorriente),"clicked",G_CALLBACK(set_dato_corriente),&corriente_arg);
    g_signal_connect(G_OBJECT(bt_setPotencia),"clicked",G_CALLBACK(set_dato_potencia),&potencia_arg);
    g_signal_connect(G_OBJECT(bt_setImpedancia),"clicked",G_CALLBACK(set_dato_impedancia),&impedancia_arg);
    g_signal_connect(G_OBJECT(bt_r_serial),"clicked",G_CALLBACK(actualizar_lista),&list_puertos);
    g_signal_connect(G_OBJECT(bt_conectar),"clicked",G_CALLBACK(abrir_com),&com_arg);
    g_signal_connect(G_OBJECT(bt_save), "clicked", G_CALLBACK(save_file), head);

    g_signal_connect(G_OBJECT(modoCorriente),"toggled",G_CALLBACK(command_d),&comando);
    g_signal_connect(G_OBJECT(modoPotencia),"toggled",G_CALLBACK(command_p),&comando);
    g_signal_connect(G_OBJECT(modoImpedancia),"toggled",G_CALLBACK(command_z),&comando);


    g_signal_connect(G_OBJECT(boton_on_of),"toggled",G_CALLBACK(start_stop),&datos_out);



    g_object_unref(G_OBJECT(constructor));

    g_timeout_add_seconds(1,get_new_data,&datos_in);
    g_timeout_add_seconds(3,new_data,&graf_data);

    llenar_lista(list_puertos);
  /* Enter the main loop */

    gtk_widget_show_all (ventana);
    gtk_main ();

    current = head;

    while ((current = head) != NULL) { // set curr to head, stop if list empty.
    head = head->next;          // advance head to next element.
    free (current);                // delete saved pointer.
}

    g_free(corr);
    g_free(tens);
    g_free(pot);
    g_free(x);

    return 0;
}
