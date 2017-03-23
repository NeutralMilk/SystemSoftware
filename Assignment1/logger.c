//
// Created by Timothy Barnard on 10/03/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "timestamp.h"
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

    char str_message[300];
    char date[50];
    int ret = 0;
    
    char* time_buff = string_date_time(date);

    strcat(str_message,time_buff);
    strcat(str_message,message);
    strcat(str_message,message1);
    strcat(str_message, "\n");

    fprintf(fp, "%s", str_message);

    if (ret < 0) {
        syslog(LOG_ERR, "Can not write to log stream: %s, error: %s",
            (fp == stdout) ? "stdout" : "daemon1", strerror(errno));
        //break;
    }
    ret = fflush(fp);
    if (ret != 0) {
        syslog(LOG_ERR, "Can not fflush() log stream: %s, error: %s",
            (fp == stdout) ? "stdout" : "daemon1", strerror(errno));
        //break;
    }
}

void log_data( FILE *fp,char * message) {

    char str_message[200];
    char date[50];
    int ret = 0;
    
    char* time_buff = string_date_time(date);

    strcat(str_message,time_buff);
    strcat(str_message,message);
    strcat(str_message, "\n");

    fprintf(fp, "%s", str_message);

    if (ret < 0) {
        syslog(LOG_ERR, "Can not write to log stream: %s, error: %s",
            (fp == stdout) ? "stdout" : "daemon1", strerror(errno));
        //break;
    }
    ret = fflush(fp);
    if (ret != 0) {
        syslog(LOG_ERR, "Can not fflush() log stream: %s, error: %s",
            (fp == stdout) ? "stdout" : "daemon1", strerror(errno));
        //break;
    }
}