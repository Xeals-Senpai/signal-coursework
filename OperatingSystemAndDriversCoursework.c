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
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

void parentsignal_handler(int);
void childsignal_handler (int);
void randomnumbergenerator ();
void informationDisplay();

int pid,number,arr[10],fd[2];			//fd[0] is the file descriptor for the read end of the pipe
										//fd[1] is the file descriptor for the write end of the pipe

void main() {
	pid = fork();
	int file = open("STATUS.txt", O_TRUNC | O_CREAT | O_RDWR, 0777);
	if (pipe(fd) < 0) {
	printf("Pipe error!");
	} 

	if (pid < 0) {
		printf("Fork error!");
	} else if (pid == 0) {							//Child process		
		signal(SIGTSTP, childsignal_handler);		//setting signal handlers per signal for the child process
		signal(SIGINT, childsignal_handler);

		close(fd[0]);
		dup2(fd[1],1);
		close(fd[1]);
		while(sleep(5000)) {}
	} else {										//parent process
		signal(SIGTSTP, SIG_IGN);					//setting signal handlers per signal for the parent process
		signal(SIGINT, SIG_IGN);
		signal(SIGCHLD, parentsignal_handler);

		close(fd[1]);
		dup2(file,STDOUT_FILENO);
		close(file);
		informationDisplay();
		dup2(fd[0],0);
		close(fd[0]);
		pause();									//waits for the child process to finish
	}
}

void parentsignal_handler(int signo) {						//Signal Handler for the parent process
	switch (signo) {
		case SIGCHLD: printf("Child process has been terminated!\n");
			      	  printf("Ending the whole process\n");
			          break;
	}
}

void childsignal_handler(int signo) {						//Signal Handler for the child process
	switch (signo) {
		case SIGINT: printf("Ctrl + c detected...\n");
			         printf("Exiting the program!\n");
			     	 exit(1);
			     	 break;
		case SIGTSTP:printf("Ctrl + z detected...\n");
			     	 printf("Starting the random number generator....\n");
			     	 randomnumbergenerator();
			     	 break;
	}
}

void randomnumbergenerator() {							
	//Random number generator function which runs everytime Ctrl+z is pressed
	//and a random number between 10 and 50 is generated.
	arr[number] = rand() % 40 + 10;
	printf("Welcome to the random number generator!\n");
	printf("Random number generated = %d\n", arr[number]);
}

void informationDisplay() {
	printf("Welcome to the information display\n");
	printf("The parent's pid is: %i\n", getpid());
	printf("The child's pid is: %i\n", pid);
}


