#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <linux/inotify.h>

#include "logger.h"

#define EVENT_SIZE (sizeof (struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

static FILE *log_stream;

char* string_date_time(char * buffer_time)
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime(buffer_time, 30, "[%Y-%m-%d %H:%M:%S]", timeinfo);

    return buffer_time;
}

static void skeleton_daemon() {
	pid_t pid;

	/* Fork off the parent process */
	pid = fork();

	/* An error occurred */
	if (pid < 0){
		//printf("pid<0 error");
		exit(EXIT_FAILURE);
	}
	/* Success: Let the parent terminate */
	if (pid > 0){
		//printf("pid>0 parent terminate");
		exit(EXIT_SUCCESS);
	}

	/* On success: The child process becomes session leader */
	if (setsid() < 0){
		//printf("setsid <0");
		exit(EXIT_FAILURE);
	}

	/* Catch, ignore and handle signals */
	//TODO: Implement a working signal handler */
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	/* Fork off for the second time*/
	pid = fork();

	/* An error occurred */
	if (pid < 0){
		//printf("pid<0");
		exit(EXIT_FAILURE);
	}
	/* Success: Let the parent terminate */
	if (pid > 0){
		//printf("pid > 0 ");
		exit(EXIT_SUCCESS);
	}
	/* Set new file permissions */
	umask(0);

	/* Change the working directory to the root directory */
	/* or another appropriated directory */
	chdir("/");

	/* Close all open file descriptors */
	int x;
	for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
	{
		close (x);
	}

	/* Open the log file */
	openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}

int main(int argc, char *argv[]) {
	
	int length, ret, i = 0;
	int fd;

	int wd;

	char buffer[EVENT_BUF_LEN];

    char log_file_name[50] = "/root/logs/files.log";

	skeleton_daemon();

	//printf("the argv[] after daemon is %s",argv[1]);

	syslog (LOG_NOTICE, "------------  First testing daemon started.----------");

	/*creating the INOTIFY instance*/
	fd = inotify_init();

	/*checking for error*/
	if ( fd < 0 ) {
	  perror( "inotify_init" );
	}

	/*adding the directory into watch list. */
	wd = inotify_add_watch( fd,  "/root/html"    , IN_ALL_EVENTS  /* IN_IGNORED | IN_OPEN | IN_CLOSE_WRITE | IN_ATTRIB | IN_CREATE | IN_DELETE |IN_CLOSE | IN_MODIFY | IN_ACCESS */ );

	if(wd<0){
	    syslog(LOG_NOTICE, "wd < 0");
	    perror("inotify_add_watch");    
	}

    /* Try to open log file to this daemon */
	if (log_file_name != NULL) {
		log_stream = fopen(log_file_name, "a+");
		if (log_stream == NULL) {
			logErrorMessage("error opening file: /root/logs/x.log");
			log_stream = stdout;
		}
	} else {
		log_stream = stdout;
	}

	while (1)
	{  
        /* Debug print */
		ret = fprintf(log_stream, "Debug: %d\n", 0);
        log_data(log_stream, "**********");
		if (ret < 0) {
			syslog(LOG_ERR, "Can not write to log stream: %s, error: %s",
				(log_stream == stdout) ? "stdout" : log_file_name, strerror(errno));
			//break;
		}
		ret = fflush(log_stream);
		if (ret != 0) {
			syslog(LOG_ERR, "Can not fflush() log stream: %s, error: %s",
				(log_stream == stdout) ? "stdout" : log_file_name, strerror(errno));
			//break;
		}
        
	    /*read to determine the event change happens on directory. Actually this read blocks until the change event occurs*/
	    syslog (LOG_NOTICE, "BEFORE READ IN BUFFER");
		length = read( fd, buffer, EVENT_BUF_LEN );

	    syslog (LOG_NOTICE, "AFTER READ IN BUFFER.");

		/*checking for error*/
		if ( length < 0 ) {
		    perror( "read" );
		}else if(length == 0) {
		    syslog(LOG_NOTICE," length =0 " );
		    continue;
	    }

	/*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
	while ( i < length ) {     
		struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];     
		if ( event->len ) {

		if (event->mask & IN_IGNORED) {
				inotify_rm_watch(fd,wd);
				inotify_add_watch(fd,"/root/html", IN_ALL_EVENTS );
			}       

		if ( event->mask & IN_ATTRIB ) {
					if ( event->mask & IN_ISDIR ) {
						//printf( " directory %s ATTRIB .\n", event->name );

			            syslog (LOG_NOTICE, " directory:chaning     ATTRIB .");
						syslog (LOG_NOTICE, " directory:%s ATTRIB .", event->name);
                        log_data_two(log_stream, "ATTRIB: directory: ",  event->name );
					}   
					else {
			            syslog (LOG_NOTICE, " directory:chaning     ATTRIB .");

						syslog (LOG_NOTICE, " file:%s ATTRIB .", event->name); 
                        log_data_two(log_stream, "ATTRIB: file: ",  event->name );                           
					}   
				}  

		if ( event->mask & IN_MODIFY ) {
				   if ( event->mask & IN_ISDIR ) {
						//printf( " directory %s modified .\n", event->name );
			            syslog (LOG_NOTICE, " dirtory:chaning    !!!!!!! .");
						syslog (LOG_NOTICE, " directory:%s modify.", event->name);
                        log_data_two(log_stream, "MODIFIED file: ",  event->name );
					}
					else {
			            syslog (LOG_NOTICE, " file  modify:chaning!!! .");
						syslog (LOG_NOTICE, " file:%s modify.", event->name);

                        log_data_two(log_stream, "MODIFIED file: ",  event->name );
					}
				}

		if ( event->mask & IN_CLOSE_WRITE ) {
					if ( event->mask & IN_ISDIR ) {
						//printf( " directory %s modified .\n", event->name );
						syslog (LOG_NOTICE, " CLSOE WRITE    !!!!!!! .");
						//syslog (LOG_NOTICE, " file:%s modify.", event->name);
                        log_data_two(log_stream, "CLOSE directory: ",  event->name );
					}
					else {
						//syslog (LOG_NOTICE, " file modify:changing!!! .");
					    syslog (LOG_NOTICE, " CLSOE WRITE    !!!!!!! .");  
                        log_data_two(log_stream, "CLOSE file: ",  event->name );                 
						//syslog (LOG_NOTICE, " file:%s modify.", event->name);
					}
				}

		if ( event->mask & IN_OPEN ) {
					if ( event->mask & IN_ISDIR ) {
						//printf( "New directory %s opened.\n", event->name );
						syslog (LOG_NOTICE, " directory:%s open.", event->name);
                        log_data_two(log_stream, "OPEN directory: ",  event->name );  
					}
					else {
			            syslog (LOG_NOTICE, " file:opening.");

						syslog (LOG_NOTICE, " file:%s open.", event->name);
						//printf( "New file %s open.\n", event->name );

                        log_data_two(log_stream, "OPEN file: ",  event->name );  

					}
				}

			if ( event->mask & IN_CREATE ) {
			if ( event->mask & IN_ISDIR ) {
				//printf( "New directory %s created.\n", event->name );
				syslog (LOG_NOTICE, "new directory:%s created.", event->name);
                log_data_two(log_stream, "NEW directory: ",  event->name );  
			}
			else {
				syslog (LOG_NOTICE, "new file:%s created.", event->name);
				//printf( "New file %s created.\n", event->name );
                log_data_two(log_stream, "NEW file: ",  event->name );  

				}
			}
			else if ( event->mask & IN_DELETE ) {
			if ( event->mask & IN_ISDIR ) {
				syslog (LOG_NOTICE, "dir:%s is deleted.", event->name);
				//printf( "Directory %s deleted.\n", event->name );
                log_data_two(log_stream, "DELETED directory: ",  event->name );  
			}
			else {
				syslog (LOG_NOTICE, "file:%s is deleted.", event->name);
				//printf( "File %s deleted.\n", event->name );
                log_data_two(log_stream, "DELETED file: ",  event->name );  
			}
			}

		else if ( event->mask & IN_ACCESS ) {
					if ( event->mask & IN_ISDIR ) {
						syslog(LOG_NOTICE, "dir:%s is accessed.", event->name);
						//printf( "The directory %s was accessed.\n", event->name );
                        log_data_two(log_stream, "ACCESSED directory: ",  event->name );  
					}
					else {
						syslog(LOG_NOTICE, "file:%s is accessed.", event->name);
						//printf( "The file %s was accessed.\n", event->name );
                        log_data_two(log_stream, "ACCESSED file: ",  event->name );  
					}   
				} 

		if ( event->mask & IN_CLOSE ) {
					if ( event->mask & IN_ISDIR ) {
						//printf( "New directory %s created.\n", event->name );
						syslog (LOG_NOTICE, " directory:%s closed.", event->name);

                        log_data_two(log_stream, "CLOSED directory: ",  event->name ); 
					}
					else {
						syslog (LOG_NOTICE, " file:%s closed.", event->name);
						//printf( "New file %s created.\n", event->name );
                        log_data_two(log_stream, "CLOSED file: ",  event->name ); 

					}
				}

			}
		i += EVENT_SIZE + event->len;
	}

		i=0;    
	    syslog (LOG_NOTICE, "~~~~~~one testing daemon ended.~~~~~~~~~~");
		sleep (20);

        log_data(log_stream, "**********");

        fclose(log_stream);
	}

	/*removing the directory from the watch list.*/
	inotify_rm_watch( fd, wd );

	/*closing the INOTIFY instance*/
	close( fd );

	syslog (LOG_NOTICE, "--------------- first testing daemon terminated.----------------");
	closelog();
	return EXIT_SUCCESS;
	
}