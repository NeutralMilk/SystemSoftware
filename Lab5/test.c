#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

void exec1();
void exec2();
void exec3();

int pid;
int pipe1[2];         
int pipe2[2];            


int main(void) {
	
	//create pipe1
	if(pipe(pipe1) == -1 ) {
		perror("bad pipe !");
		exit(1);
	}
	
	if((pid = fork()) == -1) {
		perror("bad fork !");
		exit(1);
	} else if (pid == 0) {
		exec1();
	}
	
	//parent 
	
	//create pipe2
	
	if(pipe(pipe2) == -1) {
		perror("bad pipe 2!");
		exit(1);
	}
	
	if((pid == fork()) == -1) {
		perror("bad fork2");
		exit(1);
	} else if (pid == 0) {
		exec2();
	}
	
	close(pipe1[0]);
	close(pipe1[1]);
	
	if((pid == fork()) == -1) {
		perror("bad fork3!");
		exit(1);
	} else if (pid == 0) {
		exec3();
	}
}

void exec1() {

	dup2(pipe1[1], 1);
	
	close(pipe1[0]);
	close(pipe1[1]);
	
	execlp("ps", "ps", "-aux ", NULL);
	
	perror("bad execl ls");
	_exit(1);
}

void exec2() {

	dup2(pipe1[0], 0);
	dup2(pipe2[1], 1);
	
	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[0]);
	close(pipe2[1]);
	
	execlp("awk", "awk", "'{print $1}'", NULL);
	perror("bad sort");
	_exit(1);
}

void exec3() {

	dup2(pipe2[0], 0);
	
	close(pipe2[0]);
	close(pipe2[1]);
	execlp("sort", "sort", "sort -u ", NULL);
	perror("bad sort");
	_exit(1);
}