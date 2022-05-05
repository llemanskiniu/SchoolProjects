/*
    Luke Lemanski
    Z1885842
    CSCI-480-PE1

I certify that this is my own work and where appropriate an
 extension of the starter code provided for the assignment.

*/
/********************************************************************
CSCI 480 - Assignment 2 - Semester (Spring) 2022

Progammer: Luke Lemanski
Section:   PE-1
TA:        Miguel Williams
Date Due:  Monday, Febuary 21st, 2022

Purpose:   This program shows a usage for the fork and pipe functions.
*********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#define BUFFER_SIZE 25
#define READ_END	0
#define WRITE_END	1    

int main()
{
    char write_msg[BUFFER_SIZE] = "Go do some chores.";
	char read_msg[BUFFER_SIZE] = "ERROR";
	int fd1[2]; //pipe 1
    int fd2[2]; //pipe 2

    cout << "\n"; //spacing readability indent

    if (pipe(fd1) == -1) {
		fprintf(stderr,"Pipe failed"); //pipe check
		return 1;
	}
    if (pipe(fd2) == -1) {
		fprintf(stderr,"Pipe 2 failed"); //pipe check
		return 1;
	}

    int childid = fork(); 
    if (childid < 0) {
		fprintf(stderr, "Child Fork failed"); //fork check
		return 1;
	}

    int parentid = fork(); 
    if (parentid < 0) {
		fprintf(stderr, "Parent Fork failed"); //fork check
		return 1;
	}

    if(childid == 0) //child of original parent
    {
        if(parentid == 0) //grandchild of original parent
        {
            close(fd2[WRITE_END]);

		    /* read from the pipe */
		    read(fd2[READ_END], read_msg, BUFFER_SIZE);

		    /* close the read end of the pipe */
		    close(fd2[READ_END]);

            cout << "Child: My PID is " << getpid()  << ", my parent's ID is " << getppid() << " I read from pipe B: " << read_msg << "\n\n";

            sleep(3);

            cout << "Child: " << getpid() << " is awake.\n";
        }
        else //child of original parent, parent of grandchild
        {
            close(fd2[READ_END]);

		    /* write to the pipe */
		    write(fd2[WRITE_END], write_msg, strlen(write_msg)+1); 

		    /* close the write end of the pipe */
		    close(fd2[WRITE_END]);

            cout << "Intermediate Parent: My PID is " << getpid() << ", my parent's ID is " << getppid() << ", my child is ";
            cout  << parentid << ". I write to pipe B: " << write_msg << "\n\n";


            sleep(3);

            cout << "Intermediate Parent: " << getpid() << " is awake.\n";
        }

    }
    else //not child of original parent
    {
        if(parentid == 0) //child of original parent, but not grandchild
        {
            close(fd1[WRITE_END]);

		    /* read from the pipe */
		    read(fd1[READ_END], read_msg, BUFFER_SIZE);

		    /* close the read end of the pipe */
		    close(fd1[READ_END]);

            cout << "Child: My PID is " << getpid()  << ", my parent's ID is " << getppid() << " I read from pipe A: " << read_msg << "\n\n";

            sleep(3);

            cout << "Child: " << getpid() << " is awake.\n";
        }
        else //the original parent, parent to both children, grandparent to grandchild.
        {
            close(fd1[READ_END]);

		    /* write to the pipe */
		    write(fd1[WRITE_END], write_msg, strlen(write_msg)+1); 

		    /* close the write end of the pipe */
		    close(fd1[WRITE_END]);

            cout << "Parent: My PID is " << getpid() << ", my parent's ID is " << getppid() << ", my children are ";
            cout << childid << ", " << parentid << ".  I write to pipe A: " << write_msg << " \n\n";

            cout << "Parent: Issuing command: /bin/ps -f --ppid " << getppid() << "," << getpid() << "," << parentid << "," << childid << "\n\n";

            string commandtext = "ps -f --ppid " + to_string(getppid()) + "," + to_string(getpid()) + "," + to_string(parentid) + "," + to_string(childid);
            const char *commandaction = commandtext.c_str();
            system(commandaction);

            cout << "\n\n";

            wait(NULL);

            cout << "Parent: Child processes finished their work.\n\n";
            
        }
    }

    if(parentid != 0)
        wait(NULL);
    

    return 0;
}