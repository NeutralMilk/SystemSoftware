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
				sscanf(buffer, "username: %s password: %s\r\n\r\n", username, pass);
				break;
			}
		}
		if(n<0)
		{
			break;
		}
		//printf("%s",buffer);
		//printf("pass %s",pass);

		//authenticate user
		if(strstr(username, "admin") != NULL && strstr(pass, "pass") != NULL) {
			
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
    char *message , client_message[2000];
     
    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        write(sock , client_message , strlen(client_message));
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