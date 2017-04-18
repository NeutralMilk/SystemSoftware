//
// Created by Timothy Barnard on 18/04/2017.
//
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "sendFile.h"

///Users/timothybarnard/Documents/SystemSoftware/Assignment2/ClientFiles

void send_file( char * sourceFile , int socketID ) {

    char file_buffer[512]; 
    printf("[Client] Sending %s to the Server... ", sourceFile);
    
    FILE *file_open = fopen(sourceFile, "r");
    bzero(file_buffer, 512); 
    int block_size, i=0; 
    
    while((block_size = fread(file_buffer, sizeof(char), 512, file_open)) > 0) {
        printf("Data Sent %d = %d\n",i,block_size);
        if(send(socketID, file_buffer, block_size, 0) < 0) {
            exit(1);
        }
        bzero(file_buffer, 512);
        i++;
    }
}

///Users/timothybarnard/Documents/SystemSoftware/Assignment2/ServerFiles
void receive_file( char * targetFile, int socketID ) {

    char file_buffer[512]; // Receiver buffer
    //char* file_name = "/home/jmccarthy/Documents/Apps/week9/socketExample/serverFiles/list.txt";
    
    FILE *file_open = fopen(targetFile, "w");
    if(file_open == NULL)
        printf("File %s Cannot be opened file on server.\n", targetFile);
    else {
        bzero(file_buffer, 512); 
        int block_size = 0;
        int i=0;
        while((block_size = recv(socketID, file_buffer, 512, 0)) > 0) {
            printf("Data Received %d = %d\n",i,block_size);
            int write_sz = fwrite(file_buffer, sizeof(char), block_size, file_open);
            bzero(file_buffer, 512);
            i++;
        }
        
    }
    printf("Transfer Complete!\n");
    fclose(file_open);
}