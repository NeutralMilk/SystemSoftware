#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "configuration.h"

// #define MAX_CONFIG_VARIABLE_LEN 20
// #define CONFIG_LINE_BUFFER_SIZE 100
// #define MAX_LLIST_NAME_LEN 256
// #define MAX_OUT_NAME_LEN 256

int read_int_from_config_line(char* config_line) {    
    char prm_name[20];
    int val;
    sscanf(config_line, "%s %d\n", prm_name, &val);
    return val;
}

void read_str_from_config_line(char* config_line, char* val) {    
    char prm_name[20];
    sscanf(config_line, "%s %s\n", prm_name, val);
}


struct config_struct read_config_file() {

    struct config_struct config;

    FILE *fp;
    char buf[100];
    char config_filename[255] = "/Users/timothybarnard/Documents/SystemSoftware/Assignment1/tim.txt";

    if ((fp=fopen(config_filename, "r")) == NULL) {
        fprintf(stderr, "Failed to open config file %s", config_filename);
        exit(EXIT_FAILURE);
    }
    while(! feof(fp)) {
        fgets(buf, 100, fp);
        if (buf[0] == '#' || strlen(buf) < 4) {
            continue;
        }
        if (strstr(buf, "TBPL ")) {
            config.bytes_per_line = read_int_from_config_line(buf);
        }

        if (strstr(buf, "BACKUP ")) {
            config.backup_on = read_int_from_config_line(buf);
        }

        if (strstr(buf, "BKUP_TIME ")) {
            read_str_from_config_line(buf, config.backup_time);
        }
        if (strstr(buf, "SOURCE ")) {
            read_str_from_config_line(buf, config.backup_source);
        }
        if (strstr(buf, "TARGET ")) {
            read_str_from_config_line(buf, config.backup_target);
        }
    }
    printf("TBPL = %d\n", config.bytes_per_line);
    printf("BACKUP = %d\n", config.backup_on);
    printf("BKUP_TIME = %s\n", config.backup_time);
    printf("SOURCE = %s\n", config.backup_source);
    printf("TARGET = %s\n", config.backup_target);

    fclose(fp);

    return config;
}