#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUFF 1024

int main(int argc, char *argv[]) {
	
	int fd;
	char * fifoFile = "/tmp/fifoFile";
	char buff[MAX_BUFF];
	
	fd = open(fifoFile, O_RDONLY );
	read(fd, buff, MAX_BUFF);
	printf("Message in: %s\n", buff);
	
	close(fd);
	
	unlink(fifoFile);
	
	return 0;
}