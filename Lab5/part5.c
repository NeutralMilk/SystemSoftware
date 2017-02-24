//
// Created by Paul Brittain on 24/02/2017.
//
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void exec1();
void exec2();
void exec3();

int pid;
int pipe1[2];
int pipe2[2];

int main(){
	// Create pipe1
	if (pipe(pipe1) == -1){
		perror("bad pipe1");
		exit(1);
	}

	// fork (ps aux)
	if ((pid = fork()) == -1){
		perror("bad fork1");
		exit(1);
	} else if (pid == 0){
		//stdin --> ps --> ps
		exec1();
	}
	//parent

	// create pipe2
	if (pipe(pipe2) == -1){
		perror("bad pipe2");
		exit(1);
	}

	if ((pid = fork()) == -1){
		perror("bad fork2");
		exit(1);
	} else if (pid == 0){
		// pipe1 --> grep --> pid
		exec2();
	}
	// parent

	// close unused fds
	close(pipe1[0]);
	close(pipe1[1]);

	// fork (grep sbin)
	if ((pid = fork()) == -1){
		perror("bad fork3");
		exit(1);
	} else if (pid == 0){
		// pipe2 --> grep -->
		exec3();
	}
	//parent
}

void exec1(){
	// input from stdin (already doen)
	// output from pipe1
	dup2(pipe1[1], 1);
	// close fds
	close(pipe1[0]);
	close(pipe1[1]);
	//exec
	execlp("ls", "ls", "-al", NULL);
	// exec didnt work, exit
	perror("bad exec ps");
	_exit(1);
}

void exec2(){
	//input from pipe1
	dup2(pipe1[0], 0);
	// output to pipe2
	dup2(pipe2[1], 1);
	// close fds
	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[0]);
	close(pipe2[1]);
	// exec
	execlp("awk", "awk", "{ print $1 }", NULL);
	// exec didnt work, exit
	perror("bad exec awk");
	_exit(1);
}

void exec3(){
	// input from pip2
	dup2(pipe2[0], 0);
	// output to stdout (alreayd done)
	// close fds
	close(pipe2[0]);
	close(pipe2[1]);
	// exec
	execlp("sort", "sort", "-u", NULL);
	// exec didnt work, exit
	perror("bad exec sort");
	_exit(1);
}