/**
 * Example program demonstrating UNIX pipes.
 *
 * Figures 3.23 & 3.24
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts  - Eighth Edition
 * Copyright John Wiley & Sons - 2008.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1

int main(void)
{
	char write_msg[BUFFER_SIZE] = "Greetings";
	char read_msg[BUFFER_SIZE];
	pid_t pid;
	int fd1[2];
	int fd2[2];

	/** create the pipe */
	if (pipe(fd1) == -1) {
		fprintf(stderr,"Pipe 1 failed");
		return 1;
	}
	if (pipe(fd2) == -1) {
		fprintf(stderr,"Pipe 2 failed");
		return 1;
	}

	/** now fork a child process */
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
		return 1;
	}

	if (pid > 0) {  /* parent process */
		/* close the unused end of the pipe */
		close(fd[READ_END]);

		/* write to the pipe */
		write(fd[WRITE_END], write_msg, strlen(write_msg)+1); 

		/* close the write end of the pipe */
		close(fd[WRITE_END]);
	}
	else { /* child process */
		/* close the unused end of the pipe */
		close(fd[WRITE_END]);

		/* read from the pipe */
		read(fd[READ_END], read_msg, BUFFER_SIZE);
		printf("child read %s\n",read_msg);

		/* close the read end of the pipe */
		close(fd[READ_END]);
	}

	return 0;
}

