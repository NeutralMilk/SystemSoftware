#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/stat.h>

void set_file_perm();

void set_file_perm(){
	
	struct stat st;
	mode_t mode;
	const char *path = "test.txt";

	stat(path, &st);

	mode = st.st_mode & 07777;

	// modify mode
	mode &= ~(S_IRUSR);    /* Clear this bit */
	mode |= S_IXUSR;       /* Set this bit   */

	chmod(path, mode);
}

int main(int argc, char *argv[]) {
	
	time_t now = time(NULL);
	struct tm newyear;
	newyear = *localtime(&now);
	newyear.tm_hour = 14;
	newyear.tm_min = 45;
	newyear.tm_sec = 0;
	
	char* c_time_string = ctime(&now);
	printf("Time now: %s", c_time_string);
	
 	int time_diff = difftime(mktime(&newyear), now);
	
	printf("Time diff: %d\n", time_diff);
	
	set_file_perm();
}

