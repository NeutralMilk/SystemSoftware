//
// Created by Timothy Barnard on 18/04/2017.
//
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <string.h>
#include <stdio.h>

#include "sendFile.h"
#include "timestamp.h"

///Users/timothybarnard/Documents/SystemSoftware/Assignment2/ClientFiles
char* send_file( char * sourceFile, char * file_buffer ) {
    
    char* file_name = "/Users/timothybarnard/Documents/SystemSoftware/Assignment2/ClientFiles/";
    char file_path[512];

    strcat(file_path,file_name);
    strcat(file_path,sourceFile);

    printf("[Client] Sending %s to the Server... ", file_path);

    FILE *fp;    
    char buf[100];

    if ((fp=fopen(file_path, "r")) == NULL) {
        return "Error";
    }
    
    while(! feof(fp)) {
        fgets(buf, 100, fp);
        strcat(file_buffer, buf);
        continue;
    }

    printf("The file contains this text\n\n%s", file_buffer);
    fclose(fp);
    return file_buffer;
}

///Users/timothybarnard/Documents/SystemSoftware/Assignment2/ServerFiles
int receive_file( char * targetFile, char*file_buffer ) {

    int result;    
    result = 1;
    
    FILE *file_open = fopen(targetFile, "ab+");
    if(file_open == NULL) {
        printf("File %s Cannot be opened file on server.\n", targetFile);
        result = 0;
    
    } else {

        fwrite(file_buffer , 1 , 2000 , file_open );
        result = 1;
    }
    printf("Transfer Complete!\n");
    fclose(file_open);
    return result;
}

void log_file( char * file_path, char * username ) {
    
    char* file_name = "/Users/timothybarnard/Documents/SystemSoftware/Assignment2/log.txt";
    char message[2000];
    memset(message, 0, 2000);

    char log_date[50];
    char* log_date_buff = string_date_time(log_date);

    strcat(message,"------\n");
    strcat(message,"Timestamp: ");
    strcat(message, log_date_buff);
    strcat(message,"\nFile path: ");
    strcat(message, file_path);
    strcat(message,"\nUser: ");
    strcat(message, username);
    strcat(message,"\n------");

    FILE *file_open = fopen(file_name, "ab+");
    if(file_open == NULL) {
        printf("File %s Cannot be opened file on server.\n", file_name);
    
    } else {
        fwrite(message , 1 , sizeof(message) , file_open );
    }
    printf("Logged!\n");
    fclose(file_open);
}