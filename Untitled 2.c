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


/* We are taking first argument as initial path name. */
int main(int argc, const char* argv[])
{
	 DIR* srcdir = opendir("/root/html");
		struct dirent* dent;

		if (srcdir == NULL)
		{
			perror("opendir");
			return -1;
		}

		while((dent = readdir(srcdir)) != NULL)
		{
			struct stat st;

			if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
				continue;

			if (fstatat(dirfd(srcdir), dent->d_name, &st, 0) < 0)
			{
				perror(dent->d_name);
				continue;
			}

			if (S_ISDIR(st.st_mode)) {
				wd = inotify_add_watch( fd,  dent->d_name    , IN_ALL_EVENTS );
			}
		}
		closedir(srcdir);
}