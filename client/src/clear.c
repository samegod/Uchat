#include "client.h"

void mx_clear_label_by_name(gchar *label_name, GtkBuilder *builder) 
{
    GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, label_name));

    gtk_label_set_text(label, "\0");
}

/*
void mx_clear_history_set_roomname(GtkButton *btn, GtkBuilder *builder) 
{
    t_groom *groom = mx_get_selected_groom(builder,  MX_LOCAL_ROOMS);
    GObject *label = gtk_builder_get_object(builder, "label_clear_history_roomname");
    
    gtk_label_set_text(GTK_LABEL(label), groom->room_name);
    (void)btn;
}
*/

void mx_clear_buffer_text(gchar *buff_name, GtkBuilder *builder) 
{
    GObject *buffer = gtk_builder_get_object(builder, buff_name);
    GtkTextIter start;
    GtkTextIter end;

    if (GTK_IS_TEXT_BUFFER(buffer)) {
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buffer), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buffer), &end);
        gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer), &start, &end);
    }
    else
        gtk_entry_buffer_delete_text(GTK_ENTRY_BUFFER(buffer), 0, -1);
}
