#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string.h>

#include "configuration.h"
#include "logger.h"
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
    char config_filename[255] = "/root/config.txt"; 
    //"/Users/timothybarnard/Documents/SystemSoftware/Assignment1/tim.txt";

    if ((fp=fopen(config_filename, "r")) == NULL) {
        logErrorMessages("Failed to open config file", config_filename);
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

         if (strstr(buf, "LIVE ")) {
            read_str_from_config_line(buf, config.live_site);
        }
    }
    char str[10];
    sprintf(str, "%d", config.backup_on);

    logInfoMessages("BACKUP " ,str);
    logInfoMessages("BKUP_TIME ",  config.backup_time);
    logInfoMessages("SOURCE ",  config.backup_source);
    logInfoMessages("TARGET ",  config.backup_target);
    logInfoMessages("LIVE SITE ",  config.live_site);

    fclose(fp);

    return config;
}