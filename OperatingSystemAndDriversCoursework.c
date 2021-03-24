/* 
 * Operating System and Drivers Coursework 2
 * Name: Jan Ayag
 * Student ID: w1651125
 * Lecturer: Chee Yeung
 * Due: April 1st 2021
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void parentsignal_handler(int);
void childsignal_handler (int);
void randomnumbergenerator ();

void main() 
{
	int pid;
	int fd[2];
	const int MAXLINE=4096;
	char line[MAXLINE];

	pid=fork();

	if (pipe(fd) < 0) {
	printf("Pipe error!");
	} 

	signal(SIGCHLD, parentsignal_handler);

	if (pid < 0) {
		printf("Fork error!");
	} else if (pid > 0) {
		printf("This is the parent process!\n");
		printf("The process id is: %i\n", getpid());
		pause();
	} else {
		signal(SIGTSTP, childsignal_handler);
		printf("This is the child process!\n");
		printf("The process id is: %i\n", getpid());
	}

}

void parentsignal_handler(int signo) {
	printf("The signal in the parent process has been captured\n");
}

void childsignal_handler(int signo) {
	printf("\nThe signal in the child process has been captured\n");
	//randomnumbergenerator();
}

void randomnumbergenerator() {
	int number;
	int number2 = 10;
	printf("Welcome to random number generator!");
	
	if (number == number2) {
		number = rand() % 40 + 10;
		number2 = number;
	} else printf("Random Number generated = %d\n", number);
}
