#include "client.h"

void MessageShow(char text[])
{
    
    GObject *messages = gtk_builder_get_object(builder, "listbox_found_msgs");
    GtkWidget *msg = gtk_label_new(text);
    GtkListBoxRow *mesg_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(messages));
    //GtkWidget *boxxx = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_label_set_line_wrap(GTK_LABEL(msg), TRUE);
    //gtk_container_add(GTK_CONTAINER(boxxx), msg);
    if (!strncmp(text, LOGIN, 16))
    {
        gtk_widget_set_halign(msg, GTK_ALIGN_END);
        gtk_widget_set_margin_start(msg, 200);
    }
    else
    {
        gtk_widget_set_halign(msg, GTK_ALIGN_START);
        gtk_widget_set_margin_end(msg, 200);
    }
    gtk_list_box_select_row(GTK_LIST_BOX(messages), GTK_LIST_BOX_ROW(mesg_row));
    gtk_list_box_prepend(GTK_LIST_BOX(messages), msg);
    gtk_widget_show_all(GTK_WIDGET(messages));

    maxmessages = NewMesageID(ChatsArray[chosen_chat_id].chatName);
}



void mx_send_message(GtkButton *btn)
{
    
    GObject *btn_send = gtk_builder_get_object(builder, "btn_send_msg");
    GObject *textview = gtk_builder_get_object(builder, "msg_entry");
    GObject *messages = gtk_builder_get_object(builder, "listbox_found_msgs");
    GObject *buf_message = gtk_builder_get_object(builder, "buffer_message");
    gchar *message_text = mx_get_buffer_text("buffer_message", builder);
    GtkWidget *mesg_row = GTK_WIDGET(gtk_list_box_get_selected_row(GTK_LIST_BOX(messages)));
    //GtkWidget *msg = gtk_label_new(message_text);
    char* mesg = message_text;
    SENDMESSAGE(mesg);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buf_message), "", 0);
    (void)btn;
    (void)btn_send;
    (void)textview;
    (void)mesg_row;
}
