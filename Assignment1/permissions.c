#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#include "permissions.h"
#include "logger.h"

int lockAllFiles(char* path) {
    int status;
    struct stat buffer;
    struct stat st;

    mode_t mode;
	stat(path, &st);

	mode = st.st_mode & 07777;
	mode &= ~(S_IRUSR); 
	mode |= S_IXUSR;  

    chmod(path, mode);
    status = stat(path, &buffer);

    logInfoMessages("locking folder", path);

    return status;
}

int unlockAllFiles(char* path) {
    int status;
    struct stat buffer;
    struct stat st;

    sleep(10);

    mode_t mode;
	stat(path, &st);

	mode = st.st_mode & 33188;
	mode &= ~(S_IRUSR); 
	mode |= S_IXUSR;  

    chmod(path, mode);
    status = stat(path, &buffer);

    logInfoMessages("unlocking folder", path);
    
    return status;
}

// int main(int argc, char *argv[]) {
//     lockAllFiles("");
// }