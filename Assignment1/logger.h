//
// Created by Timothy Barnard on 10/03/2017.
//

#ifndef ASSIGNMENT1_LOGGER_H
#define ASSIGNMENT1_LOGGER_H

void logInfoMessage( char* message );
void logWarningMessage( char* message );
void logErrorMessage( char* message );

void logErrorMessages( char* message1, char* message2 );
void logWarningMessages( char* message1, char* message2 );
void logInfoMessages( char* message1, char* message2 );

void log_data_two( char * log_path, char * message, char * message1);
void log_data(char * log_path, char * message);
void write_log_file( char* log_path, char* log_message );

void get_list_of_files();
void read_str_from_log(char* config_line, char* val);
char *getUserName();

#endif //ASSIGNMENT1_LOGGER_H