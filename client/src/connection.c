#include "client.h"

void SGIN(char* login, char* password) {
    char mesg[29];
    AddSpace(login);
    bzero(mesg, 29);
    strcat(mesg, "sgin");
    strncat(mesg, login, 16);
    strncat(mesg, password, 8);
    mesg[strlen(mesg)] = '\r';
    write(fd, mesg, strlen(mesg));
}

int SGUP(char* login, char* password, char* password2) {
    char mesg[29];
    if (strcmp(password, password2))
        return 1;

    for(unsigned int i = 0; i < strlen(login); i++) {
        if (!isalpha(login[i])) {
            if (!isdigit(login[i])) {
                return 2;
            }
        }
    }

    AddSpace(login);
    bzero(mesg, 29);
    strcat(mesg, "sgup");
    strncat(mesg, login, 16);
    strncat(mesg, password, 8);
    mesg[strlen(mesg)] = '\r';
    write(fd, mesg, strlen(mesg));

    return 0;
}

void NEWCHAT(char chatname[32]) {
    char str[37];
    bzero(str, 37);
    strcat(str, "nwch");
    strncat(str, chatname, 32);
    str[36] = '\r';
    write(fd, str, strlen(str));
    bzero(str, 37);
}

void UPDATECHATS() {
    char mesg[22];
    bzero(mesg, 22);
    strcat(mesg, "updc");
    strncat(mesg, LOGIN, 16);
    strcat(mesg, "\r");
    write(fd, mesg, 21);
}

void UPDATEMESSAGES(char chatName[32], int ID) {
    char mesg[41];
    char* id = addzr(ID);
    bzero(mesg, 37);

    strcat(mesg, "updm");
    strncat(mesg, chatName, 32);
    strncat(mesg, id, 4);
    mesg[40] = '\r';

    write(fd, mesg, 41);
}

void SEARCHUSER(gchar* search_members) {
    char member[17];
    char mesg[21];
    bzero(mesg, 21);
    bzero(member, 17);
    for (unsigned long i = 0; i < strlen(search_members); i+=1) {
        member[i] = search_members[i];
    }
    AddSpace(member);

    strcat(mesg, "srur");
    strncat(mesg, member, 16);
    mesg[20] = '\r';
    write(fd, mesg, strlen(mesg));
}

void SENDMESSAGE(char text[]) {
    int len = strlen(text);
    char buf[len + 37 + 16 + 7];

    struct tm *ptr;
    time_t It;
    It = time(NULL);
    ptr = localtime(&It);
    char* _time = asctime(ptr);

    char mesgTime[7];
    mesgTime[0] = '(';
    for (int i = 11; i < 16; i++) {
        mesgTime[i - 10] = _time[i];
    }
    mesgTime[6] = ')';

    bzero(buf, len + 37 + 17 + 6);
    strcat(buf, "mesg");
    strncat(buf, ChatsArray[chosen_chat_id].chatName, 32);
    strncat(buf, LOGIN, 16);
    strncat(buf, mesgTime, 7);
    strcat(buf, "\n");
    strncat(buf, text, (strlen(text)));
    buf[len + 36 + 16 + 8] = '\r';
    write(fd, buf, strlen(buf));
}

void MESSAGEEDIT(char chatName[32], int id, char text[]) {
    int len = strlen(text);
    char mesg[len + 37];
    bzero(mesg, len + 37);
    strcat(mesg, "msed");
    strncat(mesg, chatName, 32);
    strncat(mesg, addzr(id), 4);
    strncat(mesg, text, len);
    strcat(mesg, "\r");

    write(fd, mesg, strlen(mesg));
}
