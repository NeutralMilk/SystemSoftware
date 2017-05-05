/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<strings.h>

#include "sendFile.h"

char* serverDirectory( char* index ) {
    if(strcmp(index, "1") == 0) {
        return "/Users/timothybarnard/Documents/SystemSoftware/Assignment2/ServerFiles/";
    }
    if(strcmp(index, "2") == 0) {
        return "/Users/timothybarnard/Documents/SystemSoftware/Assignment2/ServerFiles/Sales/";
    }
    if(strcmp(index, "3") == 0) {
        return "/Users/timothybarnard/Documents/SystemSoftware/Assignment2/ServerFiles/Promotions/";
    }
    if(strcmp(index, "4") == 0) {
        return "/Users/timothybarnard/Documents/SystemSoftware/Assignment2/ServerFiles/Marketing/";
    }
    if(strcmp(index, "5") == 0) {
        return "/Users/timothybarnard/Documents/SystemSoftware/Assignment2/ServerFiles/Offers/";
    }
    return "/Users/timothybarnard/Documents/SystemSoftware/Assignment2/ServerFiles/";
}

int main(int argc , char *argv[])
{
    int sock, n;
    struct sockaddr_in server;
    char directory[512];
    char file_name[512];
    char serverFile[1000];
    char message[1000] , server_reply[2000];
    char sendbuff[1000], recvbuff[1000], buffer[1000];

    char username[50];
    char password[50];

    memset(username, 0, 50);
    memset(password, 0, 50);
    strcpy(username, "");
    strcpy(password, "");

    strcat(username, argv[1]);
    strcat(password, argv[2]);
    
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

    snprintf(sendbuff, sizeof(sendbuff), "username: %s password: %s\r\n\r\n", username, password);
    
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
	
    if(strcmp(recvbuff,"DENIED") == 0) {
        return 1;
    }

	if (n < 0) {
        perror("read error");
         return 1;
    }

    puts("Connected\n");
     
    //keep communicating with server
    while(1) {

        puts("1: Root");
        puts("2: Sales");
        puts("3: Promotions");
        puts("4: Marketing");
        puts("5: Offers\n\n");

        scanf("%s" , directory);

        char *file_path = serverDirectory(directory);
        
        strcat(message, file_path);

        printf("\nWhat file name? ");
        scanf("%s" , file_name);

        strcat(message, file_name);

        strcat(serverFile,message);
         
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
        //Send some data
        if( send(sock , file_contents , strlen(file_contents) , 0) < 0) {
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

        log_file(serverFile, username);
    }
     
    //close(sock);
    return 0;
}