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

///Users/timothybarnard/Documents/SystemSoftware/Assignment2/ClientFiles
char* send_file( char * sourceFile, char * file_buffer ) {
    
    char* file_name = "/Users/timothybarnard/Documents/SystemSoftware/Assignment2/ClientFiles/";
    char file_path[512];
    memset(file_buffer, 0, 512);

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

    return file_buffer;
}

///Users/timothybarnard/Documents/SystemSoftware/Assignment2/ServerFiles
int receive_file( char * targetFile, char*file_buffer ) {

    int result;
    char* file_name = "/Users/timothybarnard/Documents/SystemSoftware/Assignment2/ServerFiles/";
    char file_path[512];
    memset(file_path, 0, 512);
    
    result = 1;

    strcat(file_path,file_name);
    strcat(file_path,targetFile);
    
    FILE *file_open = fopen(file_path, "ab+");
    if(file_open == NULL) {
        printf("File %s Cannot be opened file on server.\n", file_path);
        result = 0;
    
    } else {
        fwrite(file_buffer , 1 , sizeof(file_buffer) , file_open );
        result = 1;
    }
    printf("Transfer Complete!\n");
    fclose(file_open);
    return result;
}