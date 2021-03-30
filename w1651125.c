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
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
//#include </usr/include/python3.9> 						//Tried to implement python.h but because I use Manjaro which is arch based there is an error which is arch specific which makes it unable to compile and use the header file

void parentsignal_handler(int);
void childsignal_handler (int);
void randomnumbergenerator ();
void informationDisplay();

int file,pid,number,arr[1024],fd[2];	//fd[0] is the file descriptor for the read end of the pipe
time_t times;							//fd[1] is the file descriptor for the write end of the pipe
struct stat filestat;

void main() {
	pid = fork();
	file = open("STATUS.txt", O_TRUNC | O_CREAT | O_RDWR, 0777);	//opens the file called STATUS.txt and if the file does not exist it is created
	if (pipe(fd) < 0) {												//Trunc resets the file everytime the c program is run
	printf("Pipe error!");											//the file privilege is set to read and write				
	} 																//0777 sets the file to full privilege

	if (pid < 0) {
		printf("Fork error!");
	} else if (pid == 0) {							//Child process		
		signal(SIGTSTP, childsignal_handler);		//setting signal handlers per signal for the child process
		signal(SIGINT, childsignal_handler);
		close(fd[0]);								//closes the file descriptor for the read end of the pipe as its not needed
		dup2(fd[1],1);								//maps the writing end of the pipe to STDOUT
		close(fd[1]);
		while(sleep(5000)) {}
	} else {										//parent process
		signal(SIGTSTP, SIG_IGN);					//setting signal handlers per signal for the parent process
		signal(SIGINT, SIG_IGN);
		signal(SIGCHLD, parentsignal_handler);
		close(fd[1]);								//closes the file descriptor for the write end of the pipe as its not needed
		dup2(file,STDOUT_FILENO);					//maps STDOUT to the STATUS.txt file
		close(file);								
		informationDisplay();
		dup2(fd[0],0);								//maps the reading end of the pipe to stdout
		close(fd[0]);
		pause();									//waits for the child process to finish
	}
}

void parentsignal_handler(int signo) {						//Signal Handler for the parent process
	time(&times);
	switch (signo) {
		//SIGCHLD is the signal generated which informs the parent process everytime the child process terminates
		case SIGCHLD: printf("Child process has been terminated!\n");
					  printf("The child process stopped at: %s\n", ctime(&times));
			          break;
	}
}

void childsignal_handler(int signo) {						//Signal Handler for the child process
	switch (signo) {
		//SIGINT is the signal generated everytime CTRL + c is entered to interrupt the process
		//SIGTSTP is the signal generated evertime CTRL + z is entered to suspend the process
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
	//Displays various information regarding the process and the STATUS.txt file
	//such as Inode number, the parent's process id, the child's process id
	//the time the process began and the time the process ended
	//This will be written to the STATUS.txt file
	time(&times);
	stat("STATUS.txt", &filestat);
	printf("Welcome to the information display\n");
	printf("The parent's pid is: %i\n", getpid());
	printf("The child's pid is: %i\n", pid);
	printf("The child process begun at %s", ctime(&times));
	printf("This file's inode number is: %o\n", filestat.st_ino);
	printf("The ID location number of this file is: %o\n", filestat.st_dev);
	printf("The owner ID number of this file is: %o\n", filestat.st_uid);
}

