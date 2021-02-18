#include "client.h"

void create_row(char chatName[32])
{
    GObject *listbox = gtk_builder_get_object(builder, "listbox_rooms");
    GtkListBoxRow  *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox));//создание строки 
    


    chatnumber += 1;
    
    if(chatnumber < 2) {
        struct ChatsBase*  arr = malloc(sizeof(struct ChatsBase));
        bzero(arr[0].chatName, 33);
        bzero(arr[0].username, 17);
        for (int i = 0; i < 16; i++) {
            if (chatName[i] == '_')
                strcat(arr[0].username, " ");
            else
                strncat(arr[0].username, &chatName[i], 1);
        }
        arr[0].username[16] = '\0';
        if (!strncmp(arr[0].username, LOGIN, 16)) {
            bzero(arr[0].username, 17);
            for (int i = 16; i < 32; i++) {
                if (chatName[i] == '_')
                    strcat(arr[0].username, " ");
                else
                    strncat(arr[0].username, &chatName[i], 1);
            }
        }
        
        strncat(arr[0].chatName, chatName, 32);
        ChatsArray = arr;
    }
    else {
        NewDialog();
        bzero(ChatsArray[0].chatName, 33);
        bzero(ChatsArray[0].username, 17);
        strncat(ChatsArray[0].chatName, chatName, 32);
        for (int i = 0; i < 16; i++) {
            if (chatName[i] == '_')
                strcat(ChatsArray[0].username, " ");
            else
                strncat(ChatsArray[0].username, &chatName[i], 1);
        }
        ChatsArray[0].username[16] = '\0';
        if (!strncmp(ChatsArray[0].username, LOGIN, 16)) {
            bzero(ChatsArray[0].username, 17);
            for (int i = 16; i < 32; i++) {
                if (chatName[i] == '_')
                    strcat(ChatsArray[0].username, " ");
                else
                    strncat(ChatsArray[0].username, &chatName[i], 1);
            }
        }
        ChatsArray[0].chatName[32] = '\0';
    }

    GtkWidget *username = gtk_label_new(ChatsArray[0].username);//создание лейбла с именем
    /*
    GObject *chatname = gtk_builder_get_object(builder, "buffer_chatname");
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(chatname), chatName, -1);
    //gchar *ChatName = mx_get_buffer_text("buffer_chatname", builder);
    */  
    gtk_list_box_select_row(GTK_LIST_BOX(listbox), row);//выбор строки в лист боксе
    gtk_list_box_prepend(GTK_LIST_BOX(listbox), username);//добавление лейбла в лист бокс
    gtk_widget_show_all(GTK_WIDGET(listbox));//показ лист бокса со всем содержимым
    mx_widget_set_visibility_by_name(builder, "label_search_nothing_local", FALSE);//скрытие лейбла о том что ничего не найдено
}

void LoadChats() {
    UPDATECHATS();

    while(1) {
        if (update_responce_int == 1) {
            //GObject *listbox = gtk_builder_get_object(builder, "listbox_rooms");
            sqlite3_stmt* stmt;
            const unsigned char* _chatName;
            char chatName1[33];
            char sql[43];
            char name[17];

            bzero(name, 17);
            bzero(sql, 43);
            bzero(chatName1, 32);
            strcat(sql, "select chatName from chats");

            sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
            while (sqlite3_step(stmt) != SQLITE_DONE) {
                int j = 0;
                _chatName = sqlite3_column_text(stmt, 0);

                for (int i = 0; i < 32; i++) {
                    chatName1[i] = _chatName[i];
                }
                for (int i = 0; i < 16; i++) {
                    name[i] = chatName1[i + 16 * j];
                }

                if(CmpLogsWithoutAddy(name, LOGIN)) 
                    j = 1;
                
                for (int i = 0; i < 16; i++) {
                    name[i] = chatName1[i + 16 * j];
                }
                
                addY(name);
                create_row(chatName1);
            }
            update_responce_int = 0;
            break;
        }
    }
    CheckMessages();
}

int search_member(gchar* search_members) {
    while(1) {
        if (isconnected == 0) {
            break;
        }
        if (search_responce_int == 1) {
            char chatName[33];
            char name1[17];
            char name2[17];

            bzero(name1, 17);
            bzero(name2, 17);
            bzero(chatName, 33);
            strncat(name1, LOGIN, 16);
            strncat(name2, search_members, 16);
            AddSpace(name2);
            strcat(chatName, name1);
            strcat(chatName, name2);
            addX(chatName);

            NEWCHAT(chatName);
            //create_row(search_members, listbox, chatName);

            search_responce_int = 0;
            return 1;
        }
        if (search_responce_int == 0) {
            GObject *label_search_nothing_local = gtk_builder_get_object(builder, "label_search_nothing_local");
            mx_widget_set_visibility_by_name(builder, "label_search_nothing_local", TRUE);//скрытие лейбла о том что ничего не найдено
            gtk_label_set_text(GTK_LABEL (label_search_nothing_local), "No such users");
            
            search_responce_int = 0;
            return 1;
        }
    }
    return 0;
}

void mx_search_members(GtkButton *btn, GObject *listbox)
{
    listbox = gtk_builder_get_object(builder, "listbox_rooms");//создание лист бокса
    gchar *search_members = mx_get_buffer_text("buffer_members", builder);//выкачка имени из буфера

    mx_widget_set_visibility_by_name(builder, "label_search_nothing_local", FALSE);//скрытие лейбла о том что ничего не найдено

    SEARCHUSER(search_members);

    search_member(search_members);
    
    (void)btn;
}

void mx_add_room()
{
    sqlite3_stmt* stmt;
    char sql[200];
    const unsigned char* _mesg;

    GObject *listbox = gtk_builder_get_object(builder, "listbox_rooms");
    GtkListBoxRow  *row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox));
    chosen_chat_id = gtk_list_box_row_get_index(row);
    GObject *messagess = gtk_builder_get_object(builder, "listbox_found_msgs");
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(messagess));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
    {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    bzero(sql, 200);

    strcat(sql, "SELECT mesage from ");
    strncat(sql, ChatsArray[chosen_chat_id].chatName, 32);
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        _mesg = sqlite3_column_text(stmt, 0);

        char mesg[my_strlen(_mesg)];

        for (int i = 0; i < my_strlen(_mesg); i++) {
            mesg[i] = _mesg[i];
        }
        mesg[my_strlen(_mesg)] = '\0';
        MessageShow(mesg);
    }

    maxmessages = NewMesageID(ChatsArray[chosen_chat_id].chatName);
    (void)messagess;
}
