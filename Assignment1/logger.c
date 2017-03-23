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

char *getUserName()
{
    char *p=getenv("USER");
    if(p==NULL)  {
        return "";
    } else {
        return p;
    }
   // printf("%s\n",p);
}

void log_data_two( char * message, char * message1) 
{
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

    char log_file_name[50] = "/root/logs/";
    char log_date[50];    
    char* log_date_buff = str_date_log(log_date);
    strcat(log_file_name,log_date_buff);
    strcat(log_file_name,".log");

    char date[50];
    char* time_buff = string_date_time(date);
    char str_message[300];

	if (log_file_name == NULL) {
        syslog(LOG_ERR, "Log file string empty");
    }

	log_file = fopen(log_file_name, "a+");

	if (log_file == NULL) {
		syslog(LOG_ERR, "Can not open log file: %s, error: %s",
				log_file_name, strerror(errno));
	}	

    strcat(str_message,time_buff);
    strcat(str_message," ");
    strcat(str_message,getUserName());
    strcat(str_message," ");
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


void read_str_from_log(char* config_line, char* val) {    
	char prm_name[20];
	char prm_name1[20];
	char prm_name2[20];
	char prm_name3[20];
	sscanf(config_line, " %s %s %s %s %s\n", prm_name, prm_name1, prm_name2,  prm_name3, val);
}


void get_list_of_files() {

    char **to_be_returned[1000][50];
  
    FILE *fp;
    char buf[100];
    
    char log_file_name[50] = "/root/logs/";
    char log_date[50];    
    char* log_date_buff = str_date_log(log_date);
    strcat(log_file_name,log_date_buff);
    strcat(log_file_name,".log");

    if ((fp=fopen(log_file_name, "r")) == NULL) {
        logErrorMessages("Failed to open config file", log_file_name);
        //exit(EXIT_FAILURE);
    }

    int i = 0;
    while(! feof(fp)) {
		fgets(buf, 100, fp);
		
		if (strstr(buf, "MODIFIED")) {
			char tester[50];
			read_str_from_log(buf, tester);

			//to_be_returned[i] = tester;
            i++;
		}
	}

    fclose(fp);

    //return to_be_returned;
}

void write_watch_file( char* directory_name, int watcher ) {

    FILE *watch_file = NULL;
	int ret = -1;

    char watch_file_name[50] = "/root/logs/watcher.txt";
    char str_message[300];    

	if (watch_file_name == NULL) {
        syslog(LOG_ERR, "watch file string empty");
    }

	watch_file = fopen(watch_file_name, "a+");

	if (watch_file == NULL) {
		syslog(LOG_ERR, "Can not open log file: %s, error: %s",
				watch_file_name, strerror(errno));
	}	

    char str[10];
    sprintf(str, "%d", watcher);

    strcat(str_message,directory_name);
    strcat(str_message, ":");
    strcat(str_message,str);
    strcat(str_message,"\n");

    ret = fprintf(watch_file, "%s", str_message);

    if (ret < 0) {
        syslog(LOG_ERR, "Can not write to log stream: %s, error: %s",
            (watch_file == stdout) ? "stdout" : "daemon1", strerror(errno));
    }
    ret = fflush(watch_file);
    if (ret != 0) {
        syslog(LOG_ERR, "Can not fflush() log stream: %s, error: %s",
            (watch_file == stdout) ? "stdout" : "daemon1", strerror(errno));
    }

    fclose(watch_file);
}
