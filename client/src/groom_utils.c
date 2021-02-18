#include "client.h"

t_groom *mx_get_selected_groom(GtkBuilder *builder, gchar *list_name) 
{
    GtkListBox *list = GTK_LIST_BOX(gtk_builder_get_object(builder,
                                                           list_name));
    GObject *row = G_OBJECT(gtk_list_box_get_selected_row(list));
    t_groom *groom = NULL;

    if (row)
        groom = (t_groom*)g_object_get_data(row, "groom");
    return groom;
}
