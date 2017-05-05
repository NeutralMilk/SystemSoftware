#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "permissions.h"
#include "logger.h"


int lockAllFiles(char* path) {
    // int status;
    // struct stat buffer;
    // struct stat st;

    // mode_t mode;
	// stat(path, &st);

	// mode = st.st_mode & 11111;
	// mode &= ~(S_IRUSR); 
	// mode |= S_IXUSR;  
    //status = stat(path, &buffer);

    logInfoMessages("locking folder", path);

    int value = 0;
	char mode[] = "11111";
	int i;
	i = strtol(mode, 0, 8);
	if ( chmod(path, i) < 0)
	{
		return 0;
	}
	return 1;
}

int unlockAllFiles(char* path) {

    // struct stat buffer;
    // struct stat st;

    //sleep(10);

    // mode_t mode;
	// stat(path, &st);

	// mode = st.st_mode & 07777;
	// mode &= ~(S_IRUSR); 
	// mode |= S_IXUSR;  

    // chmod(path, mode);
    // status = stat(path, &buffer);

    logInfoMessages("unlocking folder", path);
    
    int value = 0;
	char mode[] = "07777";
	int i;
	i = strtol(mode, 0, 8);
	if ( chmod(path, i) < 0)
	{
		return 0;
	}
	return 1;    
}

// int main(int argc, char *argv[]) {
//     lockAllFiles("");
// }