#include "client.h"

//--------------------------------------------------------------------------------------------------
//------------------------------------------   Connection ------------------------------------------
//--------------------------------------------------------------------------------------------------



void PreDecrypt(char buf[], int fd) {
    char mesg[1050];
    bzero(mesg, 1050);
    int j = 0;

    for(unsigned long i = 0; i < strlen(buf); i += 1) {
        if ( buf[i] == '\0' || buf[i] == '\r') {
            printf("STRING: %s\n", mesg);
            Decrypt(mesg, fd);
            bzero(mesg, j + 1);
            j = 0;
        }else {
            mesg[j] = buf[i];
            if (buf[i] != '')
                j++;
        }
    }
}

void* Write(void* FD) {
    int fld = *(int *) FD;
    fld = 1;
    char chatname[32] = "Andrew__________Dangeonmaster___";
    

    char buf[1024];
    int n;

    while(1) {
        bzero(buf, sizeof(buf));
        n = 0;

        while((buf[n++] = getchar()) != '\n');

        if (buf[0] == '!')
            SendMesage(chatname, buf);
        else 
        send(fd, buf, sizeof(buf), 0);

        pthread_testcancel();
    }
    
    pthread_exit(NULL);
}

void* Read(void* FD) {
    int fd = * (int *) FD;
    char buf[100000];
    while(1) {
        if (isconnected == 1) {
            int rcv = read(fd, buf, 100000);
            if (rcv > 0) {
                PreDecrypt(buf, fd);
                bzero(buf, sizeof(buf));
            }
            else if (rcv == -1){
                isconnected = 0;
                perror("You are disconnected!\n");
            }
        }
        pthread_testcancel();
    }
    pthread_exit(NULL);
}

void* Connection() {
    if (Connect(fd, (struct sockaddr *) &adr, sizeof adr) > 0) {
        isconnected = 1;
    }
    while(1) {
        if (isconnected == 0) {
            GObject *dialog_auth_head = gtk_builder_get_object(builder, "dialog_auth_head");
            GObject *wnd_main_header = gtk_builder_get_object(builder, "wnd_main_header");
            gtk_header_bar_set_title(GTK_HEADER_BAR(dialog_auth_head), "Disconnected");
            gtk_header_bar_set_title(GTK_HEADER_BAR(wnd_main_header), "Disconnected");
            close(fd);
            fd = socket(AF_INET, SOCK_STREAM, 0);
            if (Connect(fd, (struct sockaddr *) &adr, sizeof adr) > 0) {
                isconnected = 1;
                gtk_header_bar_set_title(GTK_HEADER_BAR(dialog_auth_head), "Authentication");
                gtk_header_bar_set_title(GTK_HEADER_BAR(wnd_main_header), "UCHAT");
            }
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

//--------------------------------------------------------------------------------------------------
//------------------------------------------   Load CSS  -------------------------------------------
//--------------------------------------------------------------------------------------------------

static void load_css(void)//   Connecting CSS file
{
    const gchar *css_style_file = "client/css/theme.css";

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_path(provider, css_style_file, NULL);

    g_object_unref(provider);
}

//--------------------------------------------------------------------------------------------------
//-------------------------------   Connect glade and create window   ------------------------------
//--------------------------------------------------------------------------------------------------

static GtkWidget* create_window (void)
{
    GError* error = NULL;

    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file (builder, MX_GUI_PATH, &error))
    {
             g_critical ("Не могу загрузить файл: %s", error->message);
             g_error_free (error);
    }
    gtk_builder_connect_signals (builder, builder);
    
    wnd_main = GTK_WIDGET (gtk_builder_get_object (builder, "wnd_main"));
    dialog_auth = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_auth"));
    GObject *listbox = gtk_builder_get_object(builder, "listbox_rooms");
    gtk_widget_hide(wnd_main);
    gtk_widget_hide(GTK_WIDGET(listbox));

    
    if (!dialog_auth)
    {
            g_critical ("Ошибка при получении виджета окна");
    }
    
    if (!wnd_main)
    {
            g_critical ("Ошибка при получении виджета окна");
    }

    return dialog_auth;
}

//--------------------------------------------------------------------------------------------------
//-----------------------------------------   Events   ---------------------------------------------
//--------------------------------------------------------------------------------------------------

gboolean my_delete_event()
{
    write(fd, "quit", strlen("quit"));
    pthread_join(rd, NULL);
    pthread_cancel(wr);
    close(fd);
    gtk_main_quit();
    printf("Programm closed\n");
    return FALSE;
}

/*
void mx_reset_addroom(GtkButton *btn, GtkBuilder *builder) 
{
    mx_clear_buffer_text("buffer_roomname", builder);
    (void)btn;
}
*/

/*
void mx_hide_msg_editing(GtkButton *btn, GtkBuilder *builder) 
{
    GObject *box_editing = gtk_builder_get_object(builder, "box_editing_msg");
    GObject *btn_apply = gtk_builder_get_object(builder, "btn_edit_msg_apply");
    GObject *btn_send = gtk_builder_get_object(builder, "btn_send_msg");

    gtk_widget_hide(GTK_WIDGET(box_editing));
    gtk_widget_hide(GTK_WIDGET(btn_apply));
    gtk_widget_show(GTK_WIDGET(btn_send));;
    mx_clear_buffer_text("buffer_message", builder);
    (void)btn;
}
*/

/*
void mx_switch_to_msg_ctrl(GtkButton *btn, GtkBuilder *builder) 
{
    mx_switch_room_header(builder, MX_MSG_CTRL);
    (void)btn;
}
*/

/*
void mx_set_unsensetive_confirm(GtkEntryBuffer *buff, guint pos, guint n_chars, GtkEntry *entry) 
{
    char *buffer = (char*)gtk_entry_buffer_get_text(buff);

    if (strlen(buffer) == 0)
        gtk_widget_set_sensitive(GTK_WIDGET(entry), 0);
    (void)buff;
    (void)pos;
    (void)n_chars;
}
*/

//--------------------------------------------------------------------------------------------------
//-------------------------------------   Main Function   ------------------------------------------
//--------------------------------------------------------------------------------------------------


int main(int adc, char* adv[]) 
{
    if (adc != 3) {
       fprintf(stderr,"usage %s IP-address port\n", adv[0]);
       exit(0);
    }

    gtk_init (NULL, NULL);
    load_css();
    window = create_window ();
    gtk_widget_show_all(window);

    DataBase();

    sginInt = -1;
    sgupInt = -1;
    search_responce_int = -1;
    edmsInt = -1;
    
    chatnumber = 0;
    adc = 1;

    
    pthread_t wr;
    pthread_t cn;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        perror("Socket1");
    
    adr.sin_family = AF_INET;
    adr.sin_port = htons(atoi(adv[2]));
    adr.sin_addr.s_addr = inet_addr( adv[1]);

    pthread_create(&cn, NULL, Connection, NULL);
    pthread_create(&rd, NULL, Read, &fd);
    pthread_create(&wr, NULL, Write, &fd);

    gtk_main ();
    
    return 0;
}
