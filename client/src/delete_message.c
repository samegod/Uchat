#include "client.h"

void mx_del_msg()
{
    GObject *messages = gtk_builder_get_object(builder, "listbox_found_msgs");
    GtkListBoxRow *mesg_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(messages));

    GObject *btn_del_msg = gtk_builder_get_object(builder, "btn_del_msg");
    
    if(gtk_list_box_row_is_selected(GTK_LIST_BOX_ROW(mesg_row)))
    {
        int numbers = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(mesg_row));

        MESSAGEEDIT(ChatsArray[chosen_chat_id].chatName, maxmessages - numbers, "Message deleted");
        EditMessage("Message deleted", numbers);
    }
    
    (void)btn_del_msg;
}
