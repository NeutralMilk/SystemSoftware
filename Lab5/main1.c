#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	
	int fd;
	
	char * fifoFile = "/tmp/fifoFile";
	
	//create the FIFO (named pipe)
	mkfifo(fifoFile, 0666);
	
	fd = open(fifoFile, O_WRONLY );
	write(fd, "This truth is out there", sizeof("This truth is out there"));
	close(fd);
	
	unlink(fifoFile);
	
	return 0;
}