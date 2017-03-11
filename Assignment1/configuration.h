//
// Created by Timothy Barnard on 10/03/2017.
//
#ifndef ASSIGNMENT1_CONFIGURATION_H
#define ASSIGNMENT1_CONFIGURATION_H

struct config_struct { 
    int bytes_per_line;
    int backup_on;
    char backup_time[256];
    char backup_source[256];
    char backup_target[256];
    char live_site[256];
};

// #define MAX_CONFIG_VARIABLE_LEN 20
// #define CONFIG_LINE_BUFFER_SIZE 100
// #define MAX_LLIST_NAME_LEN 256
// #define MAX_OUT_NAME_LEN 256

int read_int_from_config_line(char* config_line);
void read_str_from_config_line(char* config_line, char* val);
struct config_struct read_config_file();

#endif //ASSIGNMENT1_CONFIGURATION_H