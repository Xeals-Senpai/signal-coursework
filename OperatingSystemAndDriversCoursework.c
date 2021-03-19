/* 
 * Operating System and Drivers Coursework 2
 * Name: Jan Ayag
 * Student ID: w1651125
 * Lecturer: Chee Yeung
 * Due: April 1st 2021
 *
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void parentsignal_handler(int);

void main() 
{
	int pid;
	int fd[2];
	const int MAXLINE=4096;
	char line[MAXLINE];

	pid = fork();

	if (pipe(fd) < 0) {
		printf("Pipe Error!");
	} 

	if (pid < 0) {
		printf("Fork Error!");
	} else if (pid > 0) {
		wait(NULL);
		printf("This is the parent process\n");
		printf("Hello! This is the parent process speaking\n");
		pause();
	} else {
		printf("This is the Child process\n");
		printf("Hello! This is the child process speaking\n");
	}
}

void parentsignal_handler(int sig) 
{
	printf("The signal in the parent process has been captured\n");
}

void childsignal_handler(int sig) {
	printf("The signal in the child process has been captured");
}

