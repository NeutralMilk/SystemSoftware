#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#include "backup.h"
#include "configuration.h"
#include "timestamp.h"
#include "logger.h"


// struct config_struct { 
//     int bytes_per_line;
//     int backup_on;
//     char backup_time[MAX_LLIST_NAME_LEN];
//     char backup_source[MAX_LLIST_NAME_LEN];
//     char backup_target[MAX_LLIST_NAME_LEN];
// } config;

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
}

int main(int argc, char *argv[]) {

	int pid;
	printf("\nProgram Running");
    
    skeleton_daemon();

    while(1) {
       
        struct config_struct config;
        config = read_config_file();
        if (config.backup_on == 1) {
            logInfoMessages("backup set for ",config.backup_time);
            
            int seconds_diff = getSeconds(config.backup_time);
            char str[10];
            sprintf(str, "%d", seconds_diff);
            logInfoMessages("seconds until backup ",str);

            sleep(seconds_diff);
            logInfoMessage("backup starting");
            backup_folder(config.backup_source, config.backup_target );

            logInfoMessage("updating live starting");
            update_folder(config.backup_source, config.live_site);
            
            logInfoMessage("updating and backing completed");
        }
	}
}