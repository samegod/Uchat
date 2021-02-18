#include "client.h"

void mx_switch_room_header(GtkBuilder *builder, gint page_index) 
{
    GObject *stack = gtk_builder_get_object(builder, "stack_room_header");
    GObject *page = NULL;

    if (page_index == MX_ROOM_CTRL)
        page = gtk_builder_get_object(builder, "box_room_ctrl");
    else if (page_index == MX_MSG_CTRL)
        page = gtk_builder_get_object(builder, "box_msg_ctrl");
    gtk_stack_set_visible_child(GTK_STACK(stack), GTK_WIDGET(page));
}
