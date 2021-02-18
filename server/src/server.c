#include "server.h"
     

void PreDecrypt(char buf[], int fd, int I) {
    char mesg[1050];
    bzero(mesg, 1050);
    int j = 0;

    for(unsigned long i = 0; i < strlen(buf); i += 1) {
        if (buf[i] == '\r' || buf[i] == '\0') {
            Decrypt(mesg, fd, I);
            bzero(mesg, j + 1);
            j = 0;
        }else {
            mesg[j] = buf[i];
            j++;
        }
    }
}

int main(int adc, char* adv[])   
{
    if (adc != 3) {
       exit(EXIT_FAILURE);
    }

    MakingProgramWorksAsDaemon();

    DataBase();
    
    
    adc = 1;
    int PORT = atoi(adv[2]);
    int opt = TRUE;   
    int master_socket , addrlen , new_socket,  
        max_clients = 30 , activity, i , valread , sd;   
    int max_sd;   
    //int result;
    //pthread_t thread1;  
         
    char buffer[1024];  //data buffer of 1K  
    
    //type of socket created  
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = inet_addr(adv[1]);   
    address.sin_port = htons( PORT );  
         
    //set of socket descriptors  
    fd_set readfds;   
          
     
    //initialise all client_socket[] to 0 so not checked  
    for (i = 0; i < max_clients; i++)   
    {   
        client_socket[i] = 0;   
    }   
         
    //create a master socket  
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)   
    {    
        exit(EXIT_FAILURE);   
    }   
     
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,  
          sizeof(opt)) < 0 )   
    {    
        exit(EXIT_FAILURE);   
    }   
         
    //bind the socket to localhost port 8888  
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)   
    {   
        exit(EXIT_FAILURE);   
    }   
         
    //try to specify maximum of 3 pending connections for the master socket  
    if (listen(master_socket, 3) < 0)   
    {   
        exit(EXIT_FAILURE);   
    }   
         
    //accept the incoming connection  
    addrlen = sizeof(address);     
         
    while(1 > 0)   
    {   

        //clear the socket set  
        FD_ZERO(&readfds);   
     
        //add master socket to set  
        FD_SET(master_socket, &readfds);   
        max_sd = master_socket;   
             
        //add child sockets to set  
        for ( i = 0 ; i < max_clients ; i++)   
        {   
            //socket descriptor  
            sd = client_socket[i];   
                 
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely  
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {    
        }   
             
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds))   
        {   
            if ((new_socket = accept(master_socket,  
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)   
            {    
                exit(EXIT_FAILURE);   
            }
              

            //add new socket to array of sockets  
            for (i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;    
                         
                    break;   
                }   
            }   
        }   

        //else its some IO operation on some other socket 
        for (i = 0; i < max_clients; i++)   
        {   
            sd = client_socket[i];   
                 
            //read_file(buffer);
            if (FD_ISSET( sd , &readfds))   
            {   
                //Check if it was for closing , and also read the incoming message  
                if ((valread = recv( sd , buffer, 1024, 0)) <= 0)   
                {  
                    
                    //Somebody disconnected , get his details and print  
                    getpeername(sd , (struct sockaddr*)&address , 
                        (socklen_t*)&addrlen);     
                    Quit(sd, i);
                }
                //Echo back the message that came in  
                else 
                {   
                    //set the string terminating NULL byte on the end  
                    //of the data read  
                    buffer[valread] = '\0';   
                    PreDecrypt(buffer, sd, i);
                }   
            }   
        }

        
    }   
         
    return 0;   
}   
