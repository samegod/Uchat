#include "client.h"

void CheckChatComand(char buf[]) {
    char chatName[33];
    bzero(chatName, 32);
    for (int i = 4; i < 36; i++) {
        chatName[i - 4] = buf[i];
    }
    chatName[32] = '\0';
    if (CheckChat(chatName) == 1) {
        NewChatReciveComand(buf);
    }
    bzero(chatName, 33);
    
}

void SendMesage(char chatName[32], char* text) {
    char mesg[1054];
    bzero(mesg, 1054);
    strcat(mesg, "mesg");
    strncat(mesg, chatName, 32);
    strcat(mesg, LOGIN);
    strcat(mesg, "\n");
    strncat(mesg, text, strlen(text));
    mesg[strlen(mesg)] = '\r';

    write(fd, mesg, strlen(mesg));
}

void MessageDelete(int fd, char buf[]) {
    char sender[8];
    for (int i = 4; i < 12; i++) {
        sender[i - 4] = buf[i];
    }
    write(fd, sender, strlen(sender));
}

void Quit(int fd) {
    close( fd );
}

void MessageRecieveComand(char buf[]) {
    int len = strlen(buf);
    char chatName[33];
    char text[len - 39];
    char msID[5];

    for (int i = 4; i < 36; i++) {
        chatName[i - 4] = buf[i];
    }
    chatName[32] = '\0';
    addX(chatName);

    for (int i = 36; i < 40; i++) {
        msID[i - 36] = buf[i];
    }
    msID[4] = '\0';

    for (int i = 40; i < len; i++) {
        text[i - 40] = buf[i];
    }
    text[len - 40] = '\0';

    MesageRecieve(msID, chatName, text);
    if (!strncmp(chatName, ChatsArray[chosen_chat_id].chatName, 32))
    {
        MessageShow(text);
    }
        
}

void MessageEditComand(char buf[]) {
    int len = strlen(buf);
    char chatName[33];
    char id[5];
    char text[len - 39];

    for (int i = 4; i < 36; i++) {
        chatName[i - 4] = buf[i];
    }
    chatName[32] = '\0';

    for (int i = 36; i < 40; i++) {
        id[i - 36] = buf[i];
    }
    id[4] = '\0';

    for (int i = 40; i < len; i++) {
        text[i - 40] = buf[i];
    }
    text[len - 40] = '\0';
    if (MessageEdit(chatName, id, text) && !strncmp(chatName, ChatsArray[chosen_chat_id].chatName, 16))
        edmsInt = 1;
}

void NewChatReciveComand(char buf[]) {
    char chatName[33];
    for (int i = 4; i < 36; i++) {
        chatName[i - 4] = buf[i];
    }
    chatName[32] = '\0';
    addX(chatName);

    NewChat(chatName);
    create_row(chatName);
}

void Decrypt(char buf[], int fd) {

    char fst_word[4];
    for(int i = 0; i < 4; ++i) {
        fst_word[i] = buf[i];
    }


    if (!strncmp(fst_word, "quit", 4))  {
        write(fd, "Disconnect\n", strlen("Disconnect\n"));
        Quit(fd);
    }
    else if (!strncmp(fst_word, "mesg", 4)) {
        MessageRecieveComand(buf);
    }
    else if (!strncmp(fst_word, "msed", 4)) {
        MessageEditComand(buf);
    }else if (!strncmp(fst_word, "nwch", 4)) {
        NewChatReciveComand(buf);
    }else if (!strncmp(fst_word, "siny", 4)) {
        sginInt = 1;
    }else if (!strncmp(fst_word, "supy", 4)) {
        sgupInt = 1;
    }else if (!strncmp(fst_word, "supf", 4)) {
        sgupInt = 0;
    }else if (!strncmp(fst_word, "sinf", 4)) {
        sginInt = 0;
    }else if (!strncmp(fst_word, "chch", 4)) {
        CheckChatComand(buf);
    }else if (!strncmp(fst_word, "sury", 4)) {
        search_responce_int = 1;
    }else if (!strncmp(fst_word, "surf", 4)) {
        search_responce_int = 0;
    }else if (!strncmp(fst_word, "udcf", 4)) {
        update_responce_int = 1;
    }
    else if (!strncmp(fst_word, "msdl", 4)) {
        MessageDelete(fd, buf);
    }
    
}
