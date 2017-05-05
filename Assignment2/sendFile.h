//
// Created by Timothy Barnard on 18/04/2017.
//
#ifndef ASSIGNMENT2_SENDFILE_H
#define ASSIGNMENT2_SENDFILE_H

void log_file( char * file_path, char * username );
char* send_file( char * sourceFile, char * file_buffer );
int receive_file( char * targetFile, char*file_buffer );
int authenticate_file(char* username, char* password);

#endif //ASSIGNMENT2_SENDFILE_H