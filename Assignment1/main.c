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

void exec1();
void exec2();
void exec3();

int pid_f;
int pipe1[2];
int pipe2[2];
int nBytes;

int backup_running;
int push_live;

char readBuffer[10];

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
    backup_running = 1;
    push_live = 1;
    nBytes = -1;

    skeleton_daemon();

    while(1) {  

        nBytes = read(pipe2[0], readBuffer, sizeof(readBuffer));

        if (strcmp(readBuffer,"1") == 0) {
            push_live = 1;
        } 

        if (strcmp(readBuffer,"0") == 0) {
            logInfoMessage("push was not success");
        } 

        memset(readBuffer, 0, 10);

        if (push_live == 1) {
            if ((pid_f = fork()) == -1) {
                logInfoMessage("bad fork for exec2");
            } else if (pid_f == 0) {
                push_live = 0;
                exec2();
            }
        } 

        nBytes = read(pipe1[0], readBuffer, sizeof(readBuffer));

        if (strcmp(readBuffer,"1") == 0) {
            backup_running = 1;
        }
        if (strcmp(readBuffer,"0") == 0) {
            logInfoMessage("backup was not success");
        } 

        struct config_struct config;
        config = read_config_file();

        if(config.backup_now ==1 ) {
             exec3( config.backup_source,config.backup_target  );
        }

        if ((config.backup_on == 1 ) && (backup_running == 1)) {
            logInfoMessages("backup set for ",config.backup_time);
            
            int seconds_diff = getSeconds(config.backup_time);
            char str[10];
            sprintf(str, "%d", seconds_diff);
            logInfoMessages("seconds until backup ",str);

            if ((pid_f = fork()) == -1) {
                logInfoMessage("bad fork for exec1");
            } else if (pid_f == 0) {
                backup_running = 0;
                exec1( seconds_diff, config.backup_source,config.backup_target  );
            }
        }
        sleep(30);
	}
}

void exec3( char* source, char* target ) {

    logInfoMessage("backup starting");
    backup_folder(source, target );
    logInfoMessage("updating and backing completed");
}

void exec1( int seconds, char* source, char* target ) {
    sleep(seconds);
    logInfoMessage("backup starting");
    backup_folder(source, target );

    //logInfoMessage("updating live starting");
    //update_folder(config.backup_source, config.live_site);

    logInfoMessage("updating and backing completed");

     close(pipe1[0]);
     write(pipe1[1], "1", strlen("1") );

    _exit(0);
}

void exec2() {
    char log_directory[50] = "logFolder";
    char live_site[50] = "/root/live/";

    push_modified_files(live_site, log_directory);

    close(pipe2[0]);
    write(pipe2[1], "1", strlen("1") );
    
    _exit(0);
}