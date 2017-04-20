/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<strings.h>

#include "sendFile.h"

int main(int argc , char *argv[])
{
    int sock, n;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    char sendbuff[1000], recvbuff[1000], buffer[1000];
    
    if (argc != 3) {
        perror("usage: ./authenclient <Username> <Password>");
        return 1;
    }
    
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    snprintf(sendbuff, sizeof(sendbuff), "username: %s password: %s\r\n\r\n", argv[1], argv[2]);
    
    send(sock, sendbuff, strlen(sendbuff), 0); //write contents of sendbuff to socket

    while ( (n = recv(sock, recvbuff, 1000, 0)) > 0)
	{
		recvbuff[n] = 0;

		if (fputs(recvbuff, stdout) == EOF) {
            perror("fputs error");
            return 1;
        }

		if(strstr(recvbuff, "\r\n") > 0)
			break;
	}
	
	if (n < 0) {
        perror("read error");
         return 1;
    }

    puts("Connected\n");
     
    //keep communicating with server
    while(1) {
 
        printf("\nWhat directory? ");
        scanf("%s" , message);
         
        snprintf(buffer, sizeof(buffer), "type: directory message: %s\r\n\r\n", message);

        //Send some data
        if( send(sock , buffer , strlen(buffer) , 0) < 0) {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0) {
            puts("recv failed");
            break;
        }
         
        puts("Server reply :");
        puts(server_reply);
        bzero(buffer, 1000); 
        bzero(server_reply, 2000);
        bzero(message, 1000);  

        printf("\nWhat file? ");
        scanf("%s" , message); 

        char file[512];
        char* file_contents = send_file( message, file );

        snprintf(buffer, sizeof(buffer), "type: file message: %s\r\n\r\n",file_contents);

        //Send some data
        if( send(sock , buffer , strlen(buffer) , 0) < 0) {
            puts("Send failed");
            return 1;
        }

         //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0) {
            puts("recv failed");
            break;
        }
         
        puts("Server reply :");
        puts(server_reply);

        bzero(server_reply, 2000); 
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0) {
            puts("recv failed");
            break;
        }
         
        puts("Server reply :");
        puts(server_reply);
    }
     
    //close(sock);
    return 0;
}