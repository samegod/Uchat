#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <stdio.h>  
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sqlite3.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <time.h>


#define MX_GUI_PATH "client/src/gui.glade"
#define MX_ROOM_CTRL 0
#define MX_MSG_CTRL 1
#define MX_IMGS_PATH "client/img/gui/"
#define MX_IMG_EYE MX_IMGS_PATH "visibility.png"
#define MX_IMG_CLOSEDEYE MX_IMGS_PATH "unvisibility.png"
#define MX_LOCAL_ROOMS "listbox_rooms"
#define MX_GLOBAL_ROOMS "listbox_global_rooms"

struct ChatsBase {
    char chatName[33];
    char username[17];
};


GtkCssProvider *provider;
GdkDisplay *display;
GdkScreen *screen;
GtkBuilder *builder;
GtkWidget *wnd_main, *dialog_auth, *window;
GtkButton *ll;
GtkLabel *label_autherror_login, *label_autherror_signup;

sqlite3 *db;
int rc;
pthread_t rd, wr;
int fd;
char LOGIN[17];
char PASSWORD[9];
char address[9];
int port;
int sginInt;
int sgupInt;
int regInt;
int edmsInt;
int search_responce_int;
int update_responce_int;
int chosen_chat_id;
int chatnumber;
int isconnected;
int maxmessages;

pthread_t rd;

struct sockaddr_in adr;

struct ChatsBase* ChatsArray;


//--------------------------------------------------------------------------------------------------
//------------------------------------------   Connection ------------------------------------------
//--------------------------------------------------------------------------------------------------

void* Read(void* FD);

//Connections
void SGIN(char* login, char* password);
int SGUP();
void UPDATECHATS();
void SEARCHUSER();
void UPDATEMESSAGES(char chatName[32], int ID);
void NewDialog();
void SENDMESSAGE(char text[]);
void MESSAGEEDIT(char chatName[32], int id, char text[]);

//utils
int mx_atoi(const char *str);
bool mx_isdigit(int c);
char* addX(char* name);
char* addY(char* name);
void AddSpace(char *name);
bool CmpLogsWithoutAddy(char log1[17], char log2[17] );
char* addzr(int id);
int my_strlen(const unsigned char* s);

//errproc
void Socket(int domain, int type, int protocol);
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void Inet_pton(int af, const char *src, void *dst);

//Comands
void SendMesage(char chatName[32], char* text);
void NEWCHAT(char chatname[32]);
void MessageRecieveComand(char buf[]);
void MessageShow(char text[]);

//database
void Decrypt(char buf[], int fd);
int DataBase();
void NewChat(char chatname[32]);
void MesageRecieve(char id[5], char chatName[32], char text[]);
void NewChatReciveComand(char buf[]);
int CheckChat();
void CheckMessages();
int NewMesageID(char chatName[32]);
int MessageEdit(char chatName[32], char id[4], char text[]);

//--------------------------------------------------------------------------------------------------
//------------------------------------------   GUI  ------------------------------------------------
//--------------------------------------------------------------------------------------------------

typedef struct s_groom t_groom;
typedef struct s_chat t_chat;
typedef struct s_signal_data t_signal_data;

struct s_groom {
    GtkListBox *box_rooms;
    GtkScrolledWindow *page;
    GtkListBoxRow *row_room;
    GtkStack *stack_msg;
    GtkListBox *box_messages;
    GtkLabel *label_name;
    GHashTable *members;
    gboolean is_watched;
    guint64 id;
    char *room_name;
    char *customer;
    guint64 customer_id;
    guint64 date;
    char *desc;
    gboolean is_updated;
    gint uploaded;
    gdouble power;
    gboolean select_all;
    gboolean select_own;
    gboolean select_another;
    gboolean select_notedit;
};

struct s_chat {
    char *auth_token;
    char *login;
    gchar *desc;
    gint argc;
    char **argv;
    gsize id;
    t_groom *curr_room;
    gboolean upl_old_msgs;
    GtkBuilder *builder;
    gboolean valid;
    struct s_chat *chat;
    gboolean msg_placeholder;
    gboolean shift_hold;
};

struct s_signal_data {
    t_groom *groom;
    t_chat *chat;
    GtkListBoxRow *row_msg;
};

void mx_widget_set_visibility(GtkWidget *widget, gboolean is_visible);
void mx_widget_switch_visibility(GtkWidget *btn, GtkWidget *widget);
void mx_widget_set_visibility_by_name(GtkBuilder *builder, gchar *name, gboolean is_visible);
void mx_search_delim_set_visibility(GtkBuilder *builder, gboolean is_visible);
void mx_set_room_widgets_visibility(GtkBuilder *builder, gboolean visibility);

void mx_switch_room_header(GtkBuilder *builder, gint page_index);
gboolean my_delete_event();
void mx_confirm_login(GtkButton *btn);
void mx_close_auth(GtkButton *btn, GtkDialog *dialog);
void mx_show_password(GtkEntry *entry, GtkEntryIconPosition icon_pos, GdkEvent *event, 
                                                                gpointer *entry_second);
void mx_entry_set_icon_by_path(GtkEntry *entry, gchar *path, GtkEntryIconPosition icon_pos);
t_groom *mx_get_selected_groom(GtkBuilder *builder, gchar *list_name);
void mx_clear_buffer_text(gchar *buff_name, GtkBuilder *builder);
gchar *mx_get_buffer_text(gchar *buff_name, GtkBuilder *builder);
void LoadChats();
void create_row(char chatName[32]);
void EditMessage(char text[], int id);
//void mx_reset_addroom(GtkButton *btn, GtkBuilder *builder);
//void mx_hide_msg_editing(GtkButton *btn, GtkBuilder *builder);
//void mx_set_unsensetive_confirm(GtkEntryBuffer *buff, guint pos, guint n_chars, GtkEntry *entry);

#endif
