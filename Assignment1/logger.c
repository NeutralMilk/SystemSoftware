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

void log_data_two( char * message, char * message1) {

    char str_message[300];

    strcat(str_message,message);
    strcat(str_message,message1);

   write_log_file(str_message);
}

void log_data( char * message) {
    write_log_file(message);
}

void write_log_file( char* log_message ) {

    FILE *log_file = NULL;
	int ret = -1;
    char log_file_name[50] = "/root/logs/files.log";
    char str_message[300];
    char date[50];
    char* time_buff = string_date_time(date);

	if (log_file_name == NULL) {
        syslog(LOG_ERR, "Log file string empty");
    }

	log_file = fopen(log_file_name, "a+");

	if (log_file == NULL) {
		syslog(LOG_ERR, "Can not open log file: %s, error: %s",
				log_file_name, strerror(errno));
	}	

    strcat(str_message,time_buff);
    strcat(str_message,log_message);
    strcat(str_message, "\n");

    ret = fprintf(log_file, "%s", str_message);

    if (ret < 0) {
        syslog(LOG_ERR, "Can not write to log stream: %s, error: %s",
            (log_file == stdout) ? "stdout" : "daemon1", strerror(errno));
    }
    ret = fflush(log_file);
    if (ret != 0) {
        syslog(LOG_ERR, "Can not fflush() log stream: %s, error: %s",
            (log_file == stdout) ? "stdout" : "daemon1", strerror(errno));
    }

    fclose(log_file);
}