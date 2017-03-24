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
#include <dirent.h>

#include <linux/inotify.h>

#include "logger.h"

#define MAX_LEN 1024 /*Path length for a directory*/
#define MAX_EVENTS 1024 /*Max. number of events to process at one go*/
#define LEN_NAME 16 /*Assuming that the length of the filename won't exceed 16 bytes*/
#define EVENT_SIZE  ( sizeof (struct inotify_event) ) /*size of one event*/
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME )) /*buffer to store the data of events*/
#define EVENT_BUF_LEN  (1024 * (EVENT_SIZE + 16))

void add_watches(int fd, char *root, char * watch_logs)
{
  int wd;
  char *abs_dir;
  struct dirent *entry;
  DIR *dp;
 
  dp = opendir(root);
  if (dp == NULL)
    {
      perror("Error opening the starting directory");
      exit(0);
    }
 
  /* add watch to starting directory */
  wd = inotify_add_watch(fd, root, IN_CREATE | IN_MODIFY | IN_DELETE);
  write_watch_file(root, wd);
//   if (wd == -1)
//     {
//       //fprintf(fp_log,"Couldn't add watch to %s\n",root);
//     }
//   else
//     {
//       //printf("Watching:: %s\n",root);
//     }
 
  /* Add watches to the Level 1 sub-dirs*/
  abs_dir = (char *)malloc(MAX_LEN);
  memset(abs_dir, MAX_LEN, 0);
  while((entry = readdir(dp)))
    { 
      log_data(watch_logs, "Adding watches");
      /* if its a directory, add a watch*/
      if (entry->d_type == DT_DIR)
      {
          if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
              continue;
          strcpy(abs_dir,root);
          strcat(abs_dir,entry->d_name);
          
           wd = inotify_add_watch(fd, abs_dir, IN_CREATE | IN_MODIFY | IN_DELETE);
           if (wd == -1) {
              log_data(watch_logs, "Could not add watch");
           }
           else {
                write_watch_file(abs_dir, wd);
                log_data(watch_logs, "Add watch");
           }
        }
    }
   
  closedir(dp);
  free(abs_dir);
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

	openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}

int main(int argc, char *argv[]) {
	
	int length, i = 0;
	int fd;
	int wd;

    char *directory_watch = NULL;
    char *watch_log = NULL;

    if (argc <= 3) {
        printf("Not enough params: fileMonitor -d /root/html -l logFolder");
        return EXIT_FAILURE;
    }

    directory_watch = argv[2];
    watch_log = argv[4];
    printf("%s", watch_log);

	char buffer[EVENT_BUF_LEN];

	skeleton_daemon();

	//printf("the argv[] after daemon is %s",argv[1]);

	syslog (LOG_NOTICE, "------------  First testing daemon started.----------");

	/*creating the INOTIFY instance*/
	fd = inotify_init();

	/*checking for error*/
	if ( fd < 0 ) {
	  perror( "inotify_init" );
	}

	add_watches(fd,directory_watch, watch_log);

	if(wd<0){
	    syslog(LOG_NOTICE, "wd < 0");
	    perror("inotify_add_watch");    
	}

	while (1)
	{  		
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
				//inotify_rm_watch(fd,wd);
				//inotify_add_watch(fd,"/root/html", IN_ALL_EVENTS );
			}       

		if ( event->mask & IN_ATTRIB ) {
					if ( event->mask & IN_ISDIR ) {
						//printf( " directory %s ATTRIB .\n", event->name );

			            syslog (LOG_NOTICE, " directory:chaning     ATTRIB .");
						syslog (LOG_NOTICE, " directory:%s ATTRIB .", event->name);
                        log_data_two(watch_log, "ATTRIB: directory: ",  event->name, event->wd);
					}   
					else {
			            syslog (LOG_NOTICE, " directory:chaning     ATTRIB .");

						syslog (LOG_NOTICE, " file:%s ATTRIB .", event->name); 
                        log_data_two(watch_log, "ATTRIB: file: ",  event->name, event->wd );                           
					}   
				}  

		if ( event->mask & IN_MODIFY ) {
				   if ( event->mask & IN_ISDIR ) {
						//printf( " directory %s modified .\n", event->name );
			            syslog (LOG_NOTICE, " dirtory:chaning    !!!!!!! .");
						syslog (LOG_NOTICE, " directory:%s modify.", event->name);
                        log_data_two(watch_log, "MODIFIED file: ",  event->name, event->wd );
					}
					else {
			            syslog (LOG_NOTICE, " file  modify:chaning!!! .");
						syslog (LOG_NOTICE, " file:%s modify.", event->name);

                        log_data_two(watch_log, "MODIFIED file: ",  event->name , event->wd);
					}
				}

		if ( event->mask & IN_CLOSE_WRITE ) {
					if ( event->mask & IN_ISDIR ) {
						//printf( " directory %s modified .\n", event->name );
						syslog (LOG_NOTICE, " CLSOE WRITE    !!!!!!! .");
						//syslog (LOG_NOTICE, " file:%s modify.", event->name);
                        log_data_two(watch_log, "CLOSE directory: ",  event->name, event->wd);
					}
					else {
						//syslog (LOG_NOTICE, " file modify:changing!!! .");
					    syslog (LOG_NOTICE, " CLSOE WRITE    !!!!!!! .");  
                        log_data_two(watch_log, "CLOSE file: ",  event->name, event->wd );                 
						//syslog (LOG_NOTICE, " file:%s modify.", event->name);
					}
				}

		if ( event->mask & IN_OPEN ) {
					if ( event->mask & IN_ISDIR ) {
						//printf( "New directory %s opened.\n", event->name );
						syslog (LOG_NOTICE, " directory:%s open.", event->name);
                        log_data_two(watch_log, "OPEN directory: ",  event->name, event->wd );  
					}
					else {
			            syslog (LOG_NOTICE, " file:opening.");
                
						syslog (LOG_NOTICE, " file:%s open.", event->name);
						//printf( "New file %s open.\n", event->name );

                        log_data_two(watch_log, "OPEN file: ",  event->name, event->wd );  
					}
				}

			if ( event->mask & IN_CREATE ) {
			if ( event->mask & IN_ISDIR ) {
				//printf( "New directory %s created.\n", event->name );
				syslog (LOG_NOTICE, "new directory:%s created.", event->name);
                log_data_two(watch_log, "NEW directory: ",  event->name, event->wd );  
			}
			else {
				syslog (LOG_NOTICE, "new file:%s created.", event->name);
				//printf( "New file %s created.\n", event->name );
                log_data_two(watch_log, "NEW file: ",  event->name, event->wd );  

				}
			}
			else if ( event->mask & IN_DELETE ) {
			if ( event->mask & IN_ISDIR ) {
				syslog (LOG_NOTICE, "dir:%s is deleted.", event->name);
				//printf( "Directory %s deleted.\n", event->name );
                log_data_two(watch_log, "DELETED directory: ",  event->name, event->wd );  
			}
			else {
				syslog (LOG_NOTICE, "file:%s is deleted.", event->name);
				//printf( "File %s deleted.\n", event->name );
                log_data_two(watch_log, "DELETED file: ",  event->name ,event->wd );  
			}
			}

		else if ( event->mask & IN_ACCESS ) {
					if ( event->mask & IN_ISDIR ) {
						syslog(LOG_NOTICE, "dir:%s is accessed.", event->name);
						//printf( "The directory %s was accessed.\n", event->name );
                        log_data_two(watch_log, "ACCESSED directory: ",  event->name, event->wd);  
					}
					else {
						syslog(LOG_NOTICE, "file:%s is accessed.", event->name);
						//printf( "The file %s was accessed.\n", event->name );
                        log_data_two(watch_log, "ACCESSED file: ",  event->name, event->wd );  
					}   
				} 

		if ( event->mask & IN_CLOSE ) {
					if ( event->mask & IN_ISDIR ) {
						//printf( "New directory %s created.\n", event->name );
						syslog (LOG_NOTICE, " directory:%s closed.", event->name);

                        log_data_two(watch_log, "CLOSED directory: ",  event->name, event->wd ); 
					}
					else {
						syslog (LOG_NOTICE, " file:%s closed.", event->name);
						//printf( "New file %s created.\n", event->name );
                        log_data_two(watch_log, "CLOSED file: ",  event->name ,event->wd ); 

					}
				}

			}
		i += EVENT_SIZE + event->len;
	}

		i=0;    
	    syslog (LOG_NOTICE, "~~~~~~one testing daemon ended.~~~~~~~~~~");
		sleep (20);
	}

	/*removing the directory from the watch list.*/
	inotify_rm_watch( fd, wd );
	
	syslog (LOG_NOTICE, "--------------- first testing daemon terminated.----------------");
	closelog();
	return EXIT_SUCCESS;
}