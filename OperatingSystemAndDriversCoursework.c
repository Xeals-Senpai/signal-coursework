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
int flag = 0;
int pid;

void main() {
	int fd[2];
	const int MAXLINE=4096;
	char line[MAXLINE];

	pid=fork();

	if (pipe(fd) < 0) {
	printf("Pipe error!");
	} 


	if (pid < 0) {
		printf("Fork error!");
	} else if (pid == 0) {							//Child process		
		signal(SIGTSTP, childsignal_handler);
		signal(SIGINT, childsignal_handler);
		printf("This is the Child process\n");
		printf("Its process id is: %i\n", getpid());

		while(sleep(5000)) {}
	} else {
		signal(SIGTSTP, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		signal(SIGCHLD, parentsignal_handler);
		printf("\nThis is the parent process");
		printf("\nThe process id is: %i\n", getpid());			//Parent process
		pause();
	}
}

void parentsignal_handler(int signo) {						//Signal Handler for the parent process
	switch (signo) {
		case SIGINT: printf("\nChild process termination detected!\n");
			     break;
		case SIGCHLD: printf("\nChild process has been terminated!\n");
			      printf("Ending the whole process\n");
			      break;
	}
}

void childsignal_handler(int signo) {						//Signal Handler for the child process
	switch (signo) {
		case SIGINT: printf("\nCtrl + c detected...\n");
			     printf("Exiting the program!\n");
			     exit(0);
			     break;
		case SIGTSTP:printf("\nCtrl + z detected...\n");
			     printf("Starting the random number generator....\n");
			     randomnumbergenerator();
			     break;
	}
}

void randomnumbergenerator() {							
	//Random number generator function which runs everytime Ctrl+z is pressed
	//and a random number between 10 and 50 is generated.
	int number;
	int number2 = 10;
	printf("Welcome to the random number generator!");
	printf("\nRandom number generated = %d\n", rand() % 40 + 10);
}
