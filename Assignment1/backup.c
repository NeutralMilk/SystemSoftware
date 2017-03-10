//
// Created by Timothy Barnard on 10/03/2017.
//
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "backup.h"
#include "timestamp.h"
#include "logger.h"

void backup_folder( char * sourceFile, char * targetFile )
{   
    char date[50];
    char* time_buff = string_date(date);
    //char source_file[255] =  "/Users/timothybarnard/Documents/SystemSoftware/Assignment1/html";
    //char target_file[255] =  "/Users/timothybarnard/Documents/SystemSoftware/Assignment1/folder/";
    strcat(targetFile,time_buff);

    char* args[] = {"cp","-R", sourceFile, targetFile, NULL};
    execv("/bin/cp",args); 
    logInfoMessage("backup completed");
}
