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

void log_data_two( char * log_path,char * message, char * message1, int watch_id ) 
{
    char str_message[300];

    strcat(str_message,message);
    strcat(str_message,message1);

    write_log_file(log_path, str_message, watch_id);
}

void log_data( char * log_path, char * message) {
    write_log_file(log_path, message, 0);
}

void write_log_file( char * log_path, char* log_message, int watch_id ) {

    FILE *log_file = NULL;
	int ret = -1;
    
    char log_file_name[150] = "/root/logs/";
    char log_date[50];
    char* log_date_buff = str_date_log(log_date);
    strcat(log_file_name,log_path);

    //struct stat st = {0};
    //if (stat(log_file_name, &st) == -1) {
     //   mkdir(log_file_name, 0700);
    //}

    strcat(log_file_name,"/");
    strcat(log_file_name,log_date_buff);
    strcat(log_file_name,".log");

    char date[50];
    char* time_buff = string_date_time(date);
    char str_message[700];

    char directory_path[100];
    memset(directory_path, 0, 300);  

	//if (log_file_name == NULL) {
    //    syslog(LOG_ERR, "Log file string empty");
    //}

	log_file = fopen(log_file_name, "a+");

	//if (log_file == NULL) {
	//	syslog(LOG_ERR, "Can not open log file: %s, error: %s",
	//			log_file_name, strerror(errno));
	//}	

    memset(str_message, 0, 700);  

    strcat(str_message,time_buff);
    strcat(str_message," ");
    strcat(str_message,getUserName());
    strcat(str_message," "); 

    //if(watch_id) {
    //    read_watcher_file(log_message, watch_id, directory_path);
    //    strcat(str_message,  "/var/html/");
    //}
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
    char prm_name4[20];
	sscanf(config_line, " %s %s %s %s %s %s\n", prm_name, prm_name1, 
                prm_name2, prm_name3, prm_name4, val);
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

	// if (watch_file_name == NULL) {
    //     syslog(LOG_ERR, "watch file string empty");
    // }

	watch_file = fopen(watch_file_name, "a+");

	// if (watch_file == NULL) {
	// 	syslog(LOG_ERR, "Can not open log file: %s, error: %s",
	// 			watch_file_name, strerror(errno));
	// }	

    memset(str_message, 0, 300);  
    
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

void read_from_line(char* line, char* val) {    
    char prm_name[20];
    sscanf(line, "%s:%s\n",val, prm_name);
}

void read_watcher_file(char* directory, int watch_id, char* result ) {

    FILE *fp;
    char buf[100];

    char watch_file_name[50] = "/root/logs/watcher.txt";

    if ((fp=fopen(watch_file_name, "r")) == NULL) {
        logErrorMessages("Failed to open watch file", watch_file_name);
    }

    char str[10];
    sprintf(str, "%d", watch_id);
    
    while(! feof(fp)) {
        fgets(buf, 100, fp);
        if (strstr(buf, str)) {
           read_from_line(buf, result);
        }
    }

    fclose(fp);
    //return result;
}