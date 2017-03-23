//
// Created by Timothy Barnard on 10/03/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "logger.h"
#include <syslog.h>

void logInfoMessage( char* message )
{
   openlog("dt228Yr4",LOG_PID|LOG_CONS, LOG_USER);
   syslog(LOG_INFO, "%s", message);
   closelog();
}

void logInfoMessages( char* message1, char* message2 )
{
   openlog("dt228Yr4",LOG_PID|LOG_CONS, LOG_USER);
   syslog(LOG_INFO, "%s: %s", message1, message2);
   closelog();
}


void logWarningMessage( char* message )
{
   openlog("dt228Yr4",LOG_PID|LOG_CONS, LOG_USER);
   syslog(LOG_WARNING, "%s", message);
   closelog();
}

void logWarningMessages( char* message1, char* message2 )
{
   openlog("dt228Yr4",LOG_PID|LOG_CONS, LOG_USER);
   syslog(LOG_WARNING,  "%s: %s", message1, message2);
   closelog();
}

void logErrorMessage( char* message )
{
   openlog("dt228Yr4",LOG_PID|LOG_CONS, LOG_USER);
   syslog(LOG_ERR, "%s", message);
   closelog();
}

void logErrorMessages( char* message1, char* message2 )
{
   openlog("dt228Yr4",LOG_PID|LOG_CONS, LOG_USER);
   syslog(LOG_ERR,  "%s: %s", message1, message2);
   closelog();
}

void log_data_two( FILE *fp ,char * message, char * message1) {

    char str_message[200];
    char date[30];
    
    char* time_buff = string_date_time(date);

    strcat(str_message,time_buff);
    strcat(str_message,message);
    strcat(str_message,message1);

    fprintf(fp, "%s", str_message);
}

void log_data( FILE *fp,char * message) {

    char str_message[200];
    char date[50];
    
    char* time_buff = string_date_time(date);

    strcat(str_message,time_buff);
    strcat(str_message,message);

    fprintf(fp, "%s", str_message);
}