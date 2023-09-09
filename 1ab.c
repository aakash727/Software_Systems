/*
============================================================================
Name : 1ab.c
Author : Aakash Bhardwaj
Description : 1. Create the following types of a files using (i) shell command (ii) system call
 		a. soft link (symlink system call)
 		b. Hard Link
 		c. FIFO
Date: 5th Aug, 2023.
============================================================================
*/



#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
	// take input from command line
	int v = symlink(argv[1],argv[2]);
	if(v<0)
	{
		perror("failed");return 1;	
	}
	int f = link(argv[1],argv[3]);
	if(f<0)
	{
		perror("failed");return 1;
	}
	int e = mknod("fileFIFO",S_IFIFO,0);
        if(e<0)
	{
		perror("failed");return 1;
	}	
	return 0;
}
