#include "server.h"

void MessageEdit(char chatName[33], char id[5], char text[]) {
   char sql[1056];
   char* zErrMsg = 0;
   bzero(sql, 1056);

   strcat(sql, "UPDATE ");
   strncat(sql, chatName, 32);
   strcat(sql, " SET mesage = '");
   strncat(sql, text, strlen(text));
   strcat(sql, "' WHERE ID = ");
   strncat(sql, id, 4);
   strcat(sql, ";");
   rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
}

int SearchUser(char username[16]) {
   sqlite3_stmt* stmt;
   char sql[37];
   const unsigned char* _username_search;
   char username_search[17];
   bzero(username_search, 17);
   bzero(sql, 37);
   strcat(sql, "select user from users;");
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
   while (sqlite3_step(stmt) != SQLITE_DONE) {
      _username_search = sqlite3_column_text(stmt, 0);
      
      for (int i = 0; i < 16; i++) {
         username_search[i] = _username_search[i];
      }
      if (!strncmp(username, username_search, 16)) {

         return 1;
      }
      
   }

   return 0;
}

int my_strlen(const unsigned char* s) {
   int i = 0;

   while(s[i]) {
      i++;
   }

   return i;
}

void UpdateMessages(char chatName[32], char id[4], int fd) {
   sqlite3_stmt* stmt;
   char sql[200];
   const unsigned char* mesg;
   

   bzero(sql, 200);
   strcat(sql, "SELECT ID, mesage from ");
   strncat(sql, chatName, 32);
   strcat(sql, " WHERE ID > ");
   strncat(sql, id, 4);
   fd = 0;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
   while (sqlite3_step(stmt) != SQLITE_DONE) {
      int ID = sqlite3_column_int(stmt, 0);
      mesg = sqlite3_column_text(stmt, 1);
      char mesg1[my_strlen(mesg)];
      for (int i = 0; i < my_strlen(mesg); i++) {
         mesg1[i] = mesg[i];
      }
      mesg1[my_strlen(mesg)] = '\0';
      usleep(25000);
      SendMessageToBoth(chatName, mesg1, ID, "mesg");
   }
}


void UpdateChats(char username[16], int fd) {
   sqlite3_stmt* stmt;
   char sql[43];
   const unsigned char* _chatName;
   char chatName[33];
   char mesg[37];
   int newmesg;

   bzero(chatName, 33);
   bzero(sql, 43);
   strcat(sql, "select chat, newmesg from ");
   strncat(sql, username, 16);

   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
   while (sqlite3_step(stmt) != SQLITE_DONE) {
      newmesg = sqlite3_column_int(stmt, 1);

      bzero(mesg, 36);
      strncat(mesg, "chch", 4);

      _chatName = sqlite3_column_text(stmt, 0);

      for (int i = 0; i < 32; i++) {
         chatName[i] = _chatName[i];
      }
      strncat(mesg, chatName, 32);
      mesg[36] = '\r';
      write(fd, mesg, strlen(mesg));
   }
   write(fd, "udcf\r", 5);
}

int NewMesageID(char chatName[32]) {
   sqlite3_stmt* stmt;
   int n = 0;
   char sql[200];
   bzero(sql, 60);
   strcat(sql, "SELECT MAX(ID) FROM ");
   strncat(sql, chatName, 32);
   strcat(sql, ";");
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

   sqlite3_step(stmt);
   n = sqlite3_column_int(stmt, 0);

   return n;
}

void NewMesage(char chatName[32], char text[]) {
   char* zErrMsg = 0;
   char sql[1056] = "insert into ";
   int id;
   id = NewMesageID(addX(chatName)) + 1;

   strncat(sql, chatName, 32);
   strcat(sql, " VALUES( ");
   strcat(sql, mx_itoa(id));
   strcat(sql, ", '");
   strncat(sql, text, strlen(text));
   strcat(sql, "');");
   rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
   if (rc != SQLITE_OK) {
   }

   bzero(sql, 1056);
   strcat(sql, "select ID, mesage from ");
   strncat(sql, chatName, 32);
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
   const unsigned char* mesg;

   while (sqlite3_step(stmt) != SQLITE_DONE) {
      id = sqlite3_column_int(stmt, 0);
      mesg = sqlite3_column_text(stmt, 1);
      
   }
}

void NewChat(char name1[16], char name2[16]) {
   char* zErrMsg = 0;
   char chatName[33];
   bzero(chatName, 33);
   strcat(chatName, name1);
   strcat(chatName, name2);
   
   //add name of chat to the table for the first user
   char sql[100] = "insert into ";
   strncat(sql, addX(name1), 16);
   strcat(sql, " VALUES('");
   strncat(sql, addX(name1), 16);
   strncat(sql, addX(name2), 16);
   strcat(sql, "', 1);");
   rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
   if (rc != SQLITE_OK) {
   }

   //add name of chat to the table for the second user
   strcpy(sql, "insert into ");
   strncat(sql, addX(name2), 16);
   strcat(sql, " VALUES('");
   strncat(sql, addX(name1), 16);
   strncat(sql, addX(name2), 16);
   strcat(sql, "', 1);");
   rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
   if (rc != SQLITE_OK) {
   }

   //create table for chat
   strcpy(sql, "CREATE TABLE IF NOT EXISTS ");
   strncat(sql, addX(name1), 16);
   strncat(sql, addX(name2), 16);
   strcat(sql, "(ID INT, mesage varchar);");
   rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
   if (rc != SQLITE_OK) {
   }

   SendMessageToBoth(chatName, "", 0, "nwch");
}

bool InsertUser(char name[16], char password[8]) {
   //check if user exist
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, "select user, password from users", -1, &stmt, 0);

   const unsigned char *_name;
   char __name[16];
   while (sqlite3_step(stmt) != SQLITE_DONE) {

      _name = sqlite3_column_text(stmt, 0);

      for (int i = 0; i < 16; i++) {
         __name[i] = _name[i];
      }

      if (!(strncmp(addX(name), __name, 16))) {
         return false;
      }
   }

   //add user to the table of users
   char* zErrMsg = 0;
   char sql[100] = "insert into users VALUES('";
   strncat(sql, addX(name), 16);
   strcat(sql, "', '");
   strncat(sql, password, 8);
   strcat(sql, "');");

   rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
   if (rc != SQLITE_OK) {
      return false;
   }

   //table of chats for new user
   bzero(sql, 100);
   strcpy(sql, "CREATE TABLE IF NOT EXISTS ");
   strncat(sql, addX(name), 16);
   strcat(sql, "(chat varchar, newmesg int);");
   rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);

   if (rc != SQLITE_OK) {
      return false;
   }

   return true;
}

bool GetUser(char name[16], char password[8]) {
   sqlite3_stmt* stmt;
   sqlite3_prepare_v2(db, "select user, password from users", -1, &stmt, 0);

   const unsigned char *_name;
   const unsigned char *_password;

   char __name[16];
   char __password[8];

   while (sqlite3_step(stmt) != SQLITE_DONE) {

      _name = sqlite3_column_text(stmt, 0);
      _password = sqlite3_column_text(stmt, 1);

      for (int i = 0; i < 16; i++) {
         __name[i] = _name[i];
      }

      for (int i = 0; i < 8; i++) {
         __password[i] = _password[i];
      }


      if (!(strncmp(addX(name), __name, 16))) {
         if (!(strncmp(addX(password), __password, 8))) {
            return true;
         }
      }

   }

   return false;
}

int DataBase() {
   char *zErrMsg = 0;
   char *sql;

   rc = sqlite3_open("server/test.db", &db);

   if( rc ) {
      
      return(0);
   }

   sql = "CREATE TABLE IF NOT EXISTS users(user varchar, password varchar);";
   rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);

   return 0;
}




