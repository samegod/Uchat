#include "server.h"

void SearchUserComand(char buf[], int fd) {
    char username[16];
    bzero(username, 16);

    for (int i = 4; i < 20; i++) {
        username[i - 4] = buf[i];
    }
    if (SearchUser(addX(username))) {
        write(fd, "sury\r", 5);
    }else   
        write(fd, "surf\r", 5);
}

void UpdateMesgComand(char buf[], int fd) {
    char chatName[33];
    char id[4];

    bzero(chatName, 33);
    bzero(id, 4);

    for (int i = 4; i < 36; i++) {
        chatName[i - 4] = buf[i];
    }
    for (int i = 36; i < 40; i++) {
        id[i - 36] = buf[i];
    }
    UpdateMessages(chatName, id, fd);
}

void UpdateChatsComand(char buf[], int fd) {
    char username[17];
    bzero(username, 16);

    for (int i = 4; i < 20; i++) {
        username[i - 4] = buf[i];
    }
    addX(username);
    UpdateChats(username, fd);
}

void NewChatComand(char buf[]) {
    char name1[17];
    char name2[17];
    char chatName[33];

    for (int i = 4; i < 20; ++i) {
        name1[i - 4] = buf[i];
        chatName[i - 4] = buf[i];
    }
    name1[16] = '\0';
    for (int i = 20; i < 36; ++i) {
        name2[i - 20] = buf[i];
        chatName[i - 4] = buf[i];
    }
    name2[16] = '\0';

    NewChat(name1, name2);
    
}

void SignIn(char buf[], int fd, int I) {
    char name[17];
    char password[9];
    for (int i = 4; i < 20; ++i) {
        name[i - 4] = buf[i];
    }
    name[16] = '\0';
    for (int i = 20; i < 28; ++i) {
        password[i - 20] = buf[i];
    }
    password[8] = '\0';

    if (GetUser(name, password)) {
        strncat(signedInUser[I], name, 17);
        write (fd, "siny\r", 5);
    }
    else
        write(fd, "sinf\r", 5);
}

void SignUp(char buf[], int fd) {
    char name[17];
    char password[9];

    for (int i = 4; i < 20; ++i) {
        name[i - 4] = buf[i];
    }
    name[16] = '\0';
    for (int i = 20; i < 28; ++i) {
        password[i - 20] = buf[i];
    }
    password[8] = '\0';

    if (InsertUser(name, password)) 
        write(fd, "supy\r", 5);
    else
        write(fd, "supf\r", 5);
}

void MessageDelete(int fd, char buf[]) {
    char sender[9];
    char reciever[9];

    for (int i = 4; i < 12; i++) {
        sender[i - 4] = buf[i];
    }
    
    for (int i = 12; i < 20; i++) {
        reciever[i - 12] = buf[i];
    }
    sender[8] = '\0';
    reciever[8] = '\0';

    write(fd, sender, strlen(sender));
}

void MessageEditComand(char buf[]) {
    char chatName[33];
    char id[5];
    int len = strlen(buf);
    char text[len];

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

    MessageEdit(chatName, id, text);
    SendMessageToBoth(chatName, text, atoi(id), "msed");
}

void Quit(int fd, int I) {
    close( fd );   
    client_socket[I] = 0; 
    bzero(signedInUser[I], 17);
}

void SendMessageToBoth(char chatName[32], char text[], int ID, char ms[4]) {
    int len = strlen(text) + 4 + 32;
    char mesg[len + 14];
    char reciever[17];
    char id[5];
    bzero(id, 5);
    if (ID == -1)
        strcat(id, addzr(NewMesageID(chatName)));
    else {
        strcat(id, addzr(ID));
    }

    bzero(reciever, 17);
    bzero(mesg, len + 10);
    strncat(mesg, ms, 4);
    strncat(mesg, chatName, 32);
    strncat(mesg, id, 4);
    strncat(mesg, text, strlen(text));

    for(int i = 0; i < 2; i++) {
        for (int k = 0; k < 16; k++) {
            reciever[k] = chatName[k + 16 * i];
        }

        for (unsigned int i = 0; i < (sizeof(signedInUser) / sizeof(*signedInUser)); i++) {
            if (!(strncmp(reciever, signedInUser[i], 16))) {
                mesg[strlen(mesg)] = '\r';
                write(client_socket[i], mesg, strlen(mesg));
                break;
            }
        }
    }
}

void SendMessage(char buf[]) {
    char chatName[33];    
    int len = strlen(buf);
    char text[len - 35];
    for (int i = 4; i < 36; i++) {
        chatName[i - 4] = buf[i];
    }
    chatName[32] = '\0';

    for (int i = 36; i < len; i++) {
        text[i - 36] = buf[i];
    }
    text[len - 36] = '\0';

    NewMesage(chatName, text);
    
    SendMessageToBoth(chatName, text, -1, "mesg");
}

void Decrypt(char buf[], int fd, int I) {
    
    char fst_word[4];
    for(int i = 0; i < 4; ++i) {
        fst_word[i] = buf[i];
    }

    if (!strncmp(fst_word, "quit", 4))  {
        Quit(fd, I);
    }
    else if (!strncmp(fst_word, "mesg", 4)) {
        SendMessage(buf);
    }
    else if (!strncmp(fst_word, "msed", 4)) {
        MessageEditComand(buf);
    }
    else if (!strncmp(fst_word, "msdl", 4)) {
        MessageDelete(fd, buf);
    }else if (!strncmp(fst_word, "sgup", 4)) {
        SignUp(buf, fd);
    }else if (!strncmp(fst_word, "sgin", 4)) {
        SignIn(buf, fd, I);
    }else if (!strncmp(fst_word, "nwch", 4)) {
        NewChatComand(buf);
    }else if (!strncmp(fst_word, "updc", 4)) {
        UpdateChatsComand(buf, fd);
    }else if (!strncmp(fst_word, "updm", 4)) {
        UpdateMesgComand(buf, fd);
    }else if (!strncmp(fst_word, "srur", 4)) {
        SearchUserComand(buf, fd);
    }else
        write(fd, buf, strlen(buf));

    
}
