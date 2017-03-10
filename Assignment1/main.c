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

int main(int argc, char *argv[]) {

	int pid;
	printf("\nProgram Running");
    logInfoMessage("program started");
	pid = fork();
	
    // struct config_struct config;
    // config = read_config_file();
    // int seconds_diff = getSeconds(config.backup_time);

    // printf("TBPL = %d\n", config.bytes_per_line);
    // printf("BACKUP = %d\n", config.backup_on);
    // printf("BKUP_TIME = %s\n", config.backup_time);
    // printf("SOURCE = %s\n", config.backup_source);
    // printf("TARGET = %s\n", config.backup_target);

    // int seconds_diff = getSeconds(config.backup_time);

    // printf("seconds_diff = %d\n", seconds_diff);

	if( pid > 0) {
		printf("\nMe Parent");
		sleep(10);
        logInfoMessage("Parent exited");
		exit(EXIT_SUCCESS);
	
	} else {
		
		//Step1: Create Orphan
		
		//Step2: Set sessionID
		if(setsid() < 0) { 
            logErrorMessage("set sessionID not working");
            exit(EXIT_FAILURE);
        }
		
		//Step3: unmask - file priveledges to read and write
		umask(0);
		
		//Step4: Change file directoy
		if(chdir("/") < 0 ) { 
            logErrorMessage("not changing file root directoy");
            exit(EXIT_FAILURE);
        }
		
		//Step5: close file descriptors
		int x;
		for(x = sysconf(_SC_OPEN_MAX); x>=0; x--) {
			close(x);
		}
		
		while(1) {

            struct config_struct config;
            config = read_config_file();
            if (config.backup_on == 1) {
                logInfoMessages("backup set for ",config.backup_time);
                
                int seconds_diff = getSeconds(config.backup_time);
			
			    sleep(seconds_diff);
			    backup_folder(config.backup_source, config.backup_target );
            }
		}
	}
}