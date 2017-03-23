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
#include "file_changes.h"

//#define EVENT_SIZE (sizeof (struct inotify_event))
//#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

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

void listdir(const char *name, int level, int notif)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;
    if (!(entry = readdir(dir)))
        return;

    do {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            //printf("%*s[%s]\n", level*2, "", entry->d_name);
            int wd = inotify_add_watch( notif, entry->d_name, IN_CREATE | IN_DELETE );
            write_watch_file(entry->d_name, wd);
            listdir(path, level + 1, notif);
        }
    } while (entry = readdir(dir));
    closedir(dir);
}

int main(int argc, char *argv[]) {
	
	int length, i = 0;
	int fd;

	skeleton_daemon();

	/*creating the INOTIFY instance*/
	fd = inotify_init();

	/*checking for error*/
	if ( fd < 0 ) {
	  perror( "inotify_init" );
	}

	while (1)
	{  		
        listdir(".", 0, fd);
		sleep (20);
	}

	/*removing the directory from the watch list.*/
	inotify_rm_watch( fd, wd );
	
	return EXIT_SUCCESS;
}