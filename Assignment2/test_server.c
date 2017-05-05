/*
    C socket server example, handles multiple clients using threads
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include<strings.h>

#include "sendFile.h"

pthread_mutex_t lock_x;

//the thread function
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock, n;
    struct sockaddr_in server , client;
    char buffer[1000];
	char writeBuffer[1000];
    char username[50];
	char pass[50];
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        while( (n = recv(client_sock, buffer, 1000, 0)) > 0)
		{
			//exception has occured
			if(n<0)
			{
				break;
			}
			if(strstr(buffer, "\r\n\r\n") != NULL)
			{
                memset(username, 0, 50);
                memset(pass, 0, 50);
                strcpy(username, "");
                strcpy(pass, "");

				sscanf(buffer, "username: %s password: %s\r\n\r\n", username, pass);

                printf("Username %s\n", username);
                printf("Password %s \n", pass);

				break;
			}
		}
		if(n<0)
		{
			break;
		}

		//authenticate user
		if ( authenticate_file(username, pass) == 1 ) {
			
            puts("ACCESSED");
			write(client_sock, writeBuffer, strlen(writeBuffer));
			
            pthread_t sniffer_thread;
            new_sock = malloc(1);
            *new_sock = client_sock;
            
            if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
            {
                perror("could not create thread");
                return 1;
            }
            
                //Now join the thread , so that we dont terminate before the thread
            //pthread_join( sniffer_thread , NULL);
            puts("Handler assigned");
        
            if (client_sock < 0)
            {
                perror("accept failed");
                return 1;
            }
            
            snprintf(writeBuffer, sizeof(writeBuffer), "ACCESSED\n\r\n");
			write(client_sock, writeBuffer, strlen(writeBuffer));

		} else {
            puts("DENIED");
			snprintf(writeBuffer, sizeof(writeBuffer), "DENIED\n\r\n");
			write(client_sock, writeBuffer, strlen(writeBuffer));
		}
    }
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char client_message[2000];
    char type[50];
	char file_message[2000];
    char directory_name[2000];
     
    memset(client_message, 0, 2000);
     
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {   
        memset(type, 0, 50);
        memset(file_message, 0, 2000);

        sscanf(client_message, "type: %s message: %s\r\n\r\n", type, file_message);
        printf("Client message %s: \n", client_message);

        if(strcmp(type, "directory" ) ==0 ) {

            printf("%s - ", type);
            printf("%s\n",file_message);
            strcat(directory_name,file_message);
            write(sock , "File path recieved" , strlen("File path recieved"));

        } else {

            pthread_mutex_lock(&lock_x);

            printf("%s - ", type);
             if ( receive_file( directory_name, client_message ) == 1) {
                printf("\nFile sent: %s\n", client_message);
                write(sock , "File Sent" , strlen("File Sent"));
                memset(directory_name, 0, 2000);
            }

            pthread_mutex_unlock(&lock_x);
        }

        memset(client_message, 0, 2000);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}