#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	
	int fd[2], nbytes;
	pid_t pid;
	char string[] = "Howya Friend!!!\n";
	char readbuffer[100];
	
	// Create the pipe
	pipe(fd);
	
	//Create a child process
	pid = fork();
	
	if(pid == 0) {
		
		close(fd[0]);
		
		write(fd[1], string, (strlen(string) + 1));
		exit(0);
	
	} else {
		
		close(fd[1]);
		
		nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
		printf("Messages from child: %s", readbuffer);
		
	}
		
}