#include "client.h"

void mx_widget_set_visibility(GtkWidget *widget, gboolean is_visible) {
    if (is_visible)
        gtk_widget_show(widget);
    else
        gtk_widget_hide(widget);    
}

void mx_widget_switch_visibility(GtkWidget *btn, GtkWidget *widget) {
    mx_widget_set_visibility(widget, !gtk_widget_is_visible(widget));
    (void)btn;;
}

void mx_widget_set_visibility_by_name(GtkBuilder *builder,
                                      gchar *name, gboolean is_visible) {
    GtkWidget *widget = GTK_WIDGET(gtk_builder_get_object(builder, name));

    mx_widget_set_visibility(widget, is_visible);
}

void mx_search_delim_set_visibility(GtkBuilder *builder,
                                    gboolean is_visible) {
    mx_widget_set_visibility_by_name(builder,
                                     "label_search_delim_global", is_visible);
    mx_widget_set_visibility_by_name(builder,
                                     "label_search_delim_local", is_visible);
}

void mx_set_room_widgets_visibility(GtkBuilder *builder,
                                    gboolean visibility) {
    GObject *box_entry = gtk_builder_get_object(builder, "box_entry_field");
    GObject *box_header = gtk_builder_get_object(builder, "box_room_header");
    GObject *box_editing = gtk_builder_get_object(builder, "box_editing_msg");

    if (visibility) {
        mx_switch_room_header(builder, MX_ROOM_CTRL);
        gtk_widget_show_all(GTK_WIDGET(box_entry));
        gtk_widget_show_all(GTK_WIDGET(box_header));
    }
    else {
        gtk_widget_hide(GTK_WIDGET(box_entry));
        gtk_widget_hide(GTK_WIDGET(box_header));
        gtk_widget_hide(GTK_WIDGET(box_editing));
    }
}

