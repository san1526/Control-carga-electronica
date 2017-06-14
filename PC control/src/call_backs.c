#include <dirent.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <gdk/gdk.h>

#include "call_backs.h"
#include "coms.h"

void llenar_lista(GtkComboBoxText *cmb_box)
{
  DIR           *d;
  struct dirent *dir;
  d = opendir("/dev/");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
        if(!strncmp("ttyACM",dir->d_name,5) || !strncmp("ttyUSB",dir->d_name,6))
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(cmb_box),NULL,dir->d_name);
        //printf("%s \n",dir->d_name);
    }

    closedir(d);
  }
}

void actualizar_lista(GtkWidget *widget, gpointer *data)
{
    gtk_combo_box_text_remove_all(*data);
    llenar_lista(*data);
}



void set_dato_corriente(GtkWidget *widget, arguments *data)
{
    *(data->dato) = gtk_entry_get_text(*(data->p_widg));
    //g_print("%s",*(data->dato));
}

void set_dato_potencia(GtkWidget *widget, arguments *data)
{
    *(data->dato) = gtk_entry_get_text(*(data->p_widg));
    //g_print("%s",*(data->dato));
}

void set_dato_impedancia(GtkWidget *widget, arguments *data)
{
    *(data->dato) = gtk_entry_get_text(*(data->p_widg));
    //g_print("%s",*(data->dato));
}

void abrir_com(GtkWidget *widget, arguments_com *data)
{
    char p_name[20]="/dev/";
    char *buff= gtk_combo_box_text_get_active_text(*(data->p_widg));;
    strcat(p_name,buff);
    open_port(p_name,data->fd);
    if(*(data->fd) > 0)
    {
        gtk_label_set_text(*(data->p_widg_l),"Conexion establecida");
    }else
    {
        gtk_label_set_text(*(data->p_widg_l),"Error al establecer la conexión");
    }
}

gboolean get_new_data(floats_arg *data)
{
	char c = 's';
	if(*(data->fd) > 0)
	{
		if(*(data->channel) == NULL)
	    *(data->channel) = g_io_channel_unix_new(*(data->fd));
	    g_io_add_watch(*(data->channel),G_IO_IN,data_recibida,data);
	    send_command(*(data->fd), &c, "00.00");
	}
    return TRUE;
}

gboolean* data_recibida(GIOChannel *source,GIOCondition condition,floats_arg *data)
{

    char buffer[5] = {0};
    char buffer2[5] = {0};
    char buffer3[5] = {0};
    char buff_to_float[100];
    float d;
    char c = '\0';

    node_t *nd = *(data->node_ptr);

    receive_data(*(data->fd),&c,1);
    if(c != 'p')
    {
        flush(*(data->fd));
        return TRUE;
    }

    receive_data(*(data->fd),buffer,4);
    receive_data(*(data->fd),buffer2,4);
    receive_data(*(data->fd),buffer3,4);

    receive_data(*(data->fd),&c,1);
    if(c != 'p')return TRUE;

    memcpy(&d, buffer, 4);
    *(data->f1) = d;
    nd->val_c= d;
    snprintf(buff_to_float,100, "%.2f", d);
    gtk_label_set_text(*(data->labels.label2),buff_to_float);

    memcpy(&d, buffer2, 4);
    *(data->f2) = d;
    nd->val_t= d;
    snprintf(buff_to_float,100, "%.2f", d);
    gtk_label_set_text(*(data->labels.label1),buff_to_float);

    memcpy(&d, buffer3, 4);
    *(data->f3) = d;
    nd->val_p= d;
    snprintf(buff_to_float,100, "%.2f", d);
    gtk_label_set_text(*(data->labels.label3),buff_to_float);

    nd->next = malloc(sizeof(node_t));
    nd->next->next = NULL;
    *(data->node_ptr) = nd->next;

    return FALSE;
}


void start_stop(GtkWidget *widget, arguments_out *data)
{
    if(gtk_toggle_button_get_active(widget))
    {
        gtk_button_set_label(widget,"ON");

        gtk_widget_set_sensitive (*(data->bt1),FALSE);
        gtk_widget_set_sensitive (*(data->bt2),FALSE);
        gtk_widget_set_sensitive (*(data->bt3),FALSE);
        if(*(data->comando) == 'c')send_command(*(data->fd),data->comando,*(data->f_outC));
        if(*(data->comando) == 'p')send_command(*(data->fd),data->comando,*(data->f_outP));
        if(*(data->comando) == 'z')send_command(*(data->fd),data->comando,*(data->f_outZ));

    }else
    {
        gtk_button_set_label(widget,"OFF");

        gtk_widget_set_sensitive (*(data->bt1),TRUE);
        gtk_widget_set_sensitive (*(data->bt2),TRUE);
        gtk_widget_set_sensitive (*(data->bt3),TRUE);
        send_command(*(data->fd),"o","00.00");
    }
}

void command_d(GtkWidget *widget, gpointer *data)
{
    *data = 'c';

}

void command_p(GtkWidget *widget, gpointer *data)
{
    *data = 'p';
}

void command_z(GtkWidget *widget, gpointer *data)
{
    *data = 'z';
}

gboolean new_data(gr_data *user_data)
{
    SlopeItem *seriesC, *seriesP, *seriesT;
    node_t *nd = *(user_data->head);
    int i = 0, ct = 0;

    double *c,*t,*p,*x;

    while(nd != NULL)
    {
        ct++;
        nd = nd->next;
    }

    nd = *(user_data->head);
    //g_print("BF DATA IN C: %d\n",*(user_data->cont));

    if(ct == 1)return TRUE;

    c = g_malloc(ct * sizeof(double));
    t = g_malloc(ct * sizeof(double));
    p = g_malloc(ct * sizeof(double));
    x = g_malloc(ct * sizeof(double));

    while(nd->next != NULL)
     {
        //g_print("BCL I: %d\n",i);;
        c[i] = nd->val_c;
        t[i] = nd->val_t;
        p[i] = nd->val_p;
        x[i] = i;
        nd = nd->next;
        i++;
    }

    slope_scale_remove_item_by_name(*(user_data->scl),"Corriente");
    slope_scale_remove_item_by_name(*(user_data->scl),"Potencia");
    slope_scale_remove_item_by_name(*(user_data->scl),"Tension");

    seriesC = slope_xyseries_new_filled("Corriente",x,c,i,"b-");
    seriesP = slope_xyseries_new_filled("Potencia",x,p,i,"g-");
    seriesT = slope_xyseries_new_filled("Tension",x,t,i,"r-");


    slope_scale_add_item(*(user_data->scl),seriesC);
    slope_scale_add_item(*(user_data->scl),seriesT);
    slope_scale_add_item(*(user_data->scl),seriesP);

    if(gtk_toggle_button_get_active(*(user_data->auto_check)))
    slope_view_redraw(*(user_data->view));

    g_free(*(user_data->yc));
    g_free(*(user_data->yt));
    g_free(*(user_data->yp));
    g_free(*(user_data->x));

    *(user_data->yc) = c;
    *(user_data->yt) = t;
    *(user_data->yp) = p;
    *(user_data->x) = x;

    return TRUE;
}

void save_file(GtkWidget *widget, node_t *data)
{
	GtkWidget *dialog;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
	gint res;

	node_t *nd = data;

	FILE *f;

	dialog = gtk_file_chooser_dialog_new ("Save File",gtk_widget_get_toplevel (widget), action,("_Cancel"),GTK_RESPONSE_CANCEL,"_Save", GTK_RESPONSE_ACCEPT,NULL);
	chooser = GTK_FILE_CHOOSER (dialog);

	gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	  {
	    char *filename;

	    filename = gtk_file_chooser_get_filename (chooser);

	    f = fopen(filename, "w");

	    if (f == NULL)
	    {
	        printf("Error opening file!\n");
	        g_print("Error al abrir archivo de guardado");
	    }

	    fprintf(f, "Corriente;Tension;Potencia\r\n");

	    while(nd->next != NULL)
	    {
	    	fprintf(f, "%.4f;%.4f;%.4f \r\n",nd->val_c,nd->val_t,nd->val_p);
	    	nd = nd->next;
	    }
	    g_free (filename);
	    fclose(f);
	  }

	gtk_widget_destroy (dialog);
}

