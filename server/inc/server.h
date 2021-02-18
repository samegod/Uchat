#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdbool.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sqlite3.h>

#include <sys/stat.h>
#include <fcntl.h>

#define TRUE   1  
#define FALSE  0

int client_socket[100];
struct sockaddr_in address;  


sqlite3 *db;
int rc;
char signedInUser[100][17];

//utils
int mx_atoi(const char *str);
char *mx_itoa(int n);
bool mx_isdigit(int c);
char* addX(char *name);
char* addzr(int id);
void SendMessageToBoth(char chatName[32], char text[], int ID, char ms[4]);

void MakingProgramWorksAsDaemon();

//Socket
int Socket(int domain, int type, int protocol);
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void Inet_pton(int af, const char *src, void *dst);
void Quit(int fd, int I);

//Database
void Decrypt(char buf[], int fd, int I);
int DataBase();
bool InsertUser(char name[16], char password[8]);
bool GetUser(char name[16], char password[8]);
void NewChat(char name1[16], char name2[16]);
void NewMesage(char chatName[32], char text[]);
int NewMesageID(char chatName[32]);
void UpdateChats(char username[16], int fd);
void UpdateMessages(char chatName[32], char id[4], int fd);
int SearchUser(char username[16]);
void MessageEdit(char chatName[33], char id[5], char text[]);

#endif
