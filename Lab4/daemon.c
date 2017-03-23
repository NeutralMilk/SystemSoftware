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
	const char *path = "/Users/timothybarnard/Documents/SystemSoftware/Lab4/test.txt";

	stat(path, &st);

	mode = st.st_mode & 07777;

	// modify mode
	mode &= ~(S_IRUSR);    /* Clear this bit */
	mode |= S_IXUSR;       /* Set this bit   */

	chmod(path, mode);
}

int main(int argc, char *argv[]) {

	int pid;
	printf("\nProgram Running");
	pid = fork();
	
	if( pid > 0) {
		printf("\nMe Parent");
		sleep(10);
		exit(EXIT_SUCCESS);
	
	} else {
		
		//Step1: Create Orphan
		
		//Step2: Set sessionID
		if(setsid() < 0) { exit(EXIT_FAILURE);}
		
		//Step3: unmask - file priveledges to read and write
		umask(0);
		
		//Step4: Change file directoy
		if(chdir("/") < 0 ) { exit(EXIT_FAILURE);}
		
		//Step5: close file descriptors
		int x;
		for(x = sysconf(_SC_OPEN_MAX); x>=0; x--) {
			close(x);
		}
		
		while(1) {
			
			time_t now = time(NULL);
			struct tm newyear;
			newyear = *localtime(&now);
			newyear.tm_hour = 14;
			newyear.tm_min = 22;
			newyear.tm_sec = 0
				
			//char* c_time_string = ctime(&now);
			//printf("Time now: %s", c_time_string);
				
			int time_diff = difftime(mktime(&newyear), now);
			
			printf("\nMe Child: %i", getpid());	
			
			//printf("Time diff: %d\n", time_diff);			
			sleep(time_diff);
			set_file_perm();
			printf("File changing");
			
		}
	}
}