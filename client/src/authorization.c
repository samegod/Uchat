#include "client.h"

gchar *mx_get_buffer_text(gchar *buff_name, GtkBuilder *builder) {
    GObject *buffer = gtk_builder_get_object(builder, buff_name);
    GtkTextIter start;
    GtkTextIter end;
    if (GTK_IS_TEXT_BUFFER(buffer)) {
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buffer), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buffer), &end);
        return gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer),
                                        &start, &end, FALSE);
    }
    else
        return (gchar*)gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(buffer));
}

void mx_close_auth(GtkButton *btn, GtkDialog *dialog) 
{
    gtk_widget_destroy(GTK_WIDGET(dialog));
    (void)btn;
}

void LogIn(char* Login, char* password) {
    while(1) {
        if (isconnected == 0) {
            break;
        }
        if (sginInt == 1) {
            gtk_widget_hide(GTK_WIDGET(dialog_auth));
            gtk_widget_show_all(GTK_WIDGET(wnd_main));
            sginInt = -1;
            bzero(LOGIN, 17);
            bzero(PASSWORD, 9);
            strncat(LOGIN, Login, 16);
            LOGIN[16] = '\0';
            strcat(PASSWORD, password);
            PASSWORD[strlen(PASSWORD)] = '\0';
            LoadChats();
            pthread_cancel(rd);
            pthread_create(&rd, NULL, Read, &fd);

            break;
        }
        if (sginInt == 0) {
            sginInt = -1;
            label_autherror_login = GTK_LABEL(gtk_builder_get_object (builder, "label_autherror_login"));
            gtk_label_set_text(label_autherror_login, "Invalid login or password");
            break;
        }
    }
}

int SignUp() {
    while(1) {
        if (isconnected == 0) {
            break;
        }
        if (sgupInt == 1) {
            sgupInt = -1;
            return 1;
        }
        if (sgupInt == 0) {
            sgupInt = -1;
            return 0;
        }
    }
    return 2;
}

void mx_confirm_login(GtkButton *btn)
{
    gchar *password = mx_get_buffer_text("buffer_password", builder);
    gchar *Login = mx_get_buffer_text("buffer_login", builder);
    if (strlen(Login) != 0 && strlen(password)!= 0) {
        if (regInt == 0) {
            SGIN(Login, password);
            LogIn(Login, password);
        }
        if (regInt == 1) {
            gchar *password2 = mx_get_buffer_text("buffer_password_confirm", builder);
            int err = SGUP(Login, password, password2);
            int sp;

            switch(err) {
                case 0:
                    sp = SignUp();
                    switch(sp) {
                        case 1:
                            label_autherror_signup = GTK_LABEL(gtk_builder_get_object (builder, "label_autherror_signup"));
                            gtk_label_set_markup (GTK_LABEL (label_autherror_signup), "<span foreground='green'>You have successfully signed up!</span>");
                            break;
                        case 0:
                            label_autherror_signup = GTK_LABEL(gtk_builder_get_object (builder, "label_autherror_signup"));
                            gtk_label_set_text(label_autherror_signup, "User already exist!");
                            break;
                    }
                    
                    break;
                case 1:
                    label_autherror_signup = GTK_LABEL(gtk_builder_get_object (builder, "label_autherror_signup"));
                    gtk_label_set_text(label_autherror_signup, "Passwords dont match");
                    break;
                case 2:
                    label_autherror_signup = GTK_LABEL(gtk_builder_get_object (builder, "label_autherror_signup"));
                    gtk_label_set_text(label_autherror_signup, "You can use only letters and digits in login!");
                    break;
            }

        }
    }
    (void)btn;
}

void mx_show_password(GtkEntry *entry, GtkEntryIconPosition icon_pos,
                      GdkEvent *event, gpointer *entry_second) {
    if (gtk_entry_get_visibility(entry)) {
        mx_entry_set_icon_by_path(entry, MX_IMG_EYE, icon_pos);
        gtk_entry_set_visibility(entry, FALSE);
        if (GTK_IS_ENTRY(entry_second))
            gtk_entry_set_visibility(GTK_ENTRY(entry_second), FALSE);
    }
    else { 
        mx_entry_set_icon_by_path(entry, MX_IMG_CLOSEDEYE, icon_pos);
        gtk_entry_set_visibility(entry, TRUE);
        if (GTK_IS_ENTRY(entry_second))
            gtk_entry_set_visibility(GTK_ENTRY(entry_second), TRUE);
    }
    (void)event;
}

void mx_reg(GtkNotebook *notebook_auth, GtkWidget *box_signup, guint index, GtkButton *btn_auth_confirm)
{
    if(index == 1)
    {
        regInt = 1;
    }
    if(index == 0)
    {
        regInt = 0;
    }
    (void)notebook_auth;
    (void)box_signup;
    (void)index;
    (void)btn_auth_confirm;
}

/*
void mx_set_sensetive_confirm(GtkEntryBuffer *buff, guint pos, gchar *chars,
                              guint n_chars, GtkEntry *entry) {
    char *buffer = (char*)gtk_entry_buffer_get_text(buff);

    if (strlen(buffer))
        gtk_widget_set_sensitive(GTK_WIDGET(entry), 1);
    (void)buff;
    (void)pos;
    (void)chars;
    (void)n_chars;
}
*/
