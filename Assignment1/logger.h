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
void logInfoMessageInt( char* message1, int message2);

void log_data_two( char * log_path,char * message, char * message1, int watch_id );
void log_data(char * log_path, char * message);
void write_log_file( char * log_path, char* log_message, int watch_id );

void read_str_from_log(char* config_line, char* val);
char *getUserName();

void read_from_line(char* line, char* val);
void read_watcher_file(char* directory, int watch_id, char* result );

void push_modified_files( char* destintation, char* log_path );

#endif //ASSIGNMENT1_LOGGER_H