
/*
============================================================================
Name : 21.c
Author : Aakash Bhardwaj
Description : Write a program, call fork and print the parent and child process id.
Date: 5th september, 2023.
============================================================================
*/

#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
	int main(){
	pid_t returnedPid = fork();

		if(returnedPid!=0)
			printf("parent process: Pid is %d\n",getpid());
		else
			printf("parent process: Pid is %d\n",getpid());


}
