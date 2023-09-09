/*
============================================================================
Name : 5.c
Author : Aakash Bhardwaj
Description : Write a program to create five new files with infinite loop. 
		Execute the program in the background and check the file descriptor table at /proc/pid/fd
Date: 5th Aug, 2023.
============================================================================
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
int main()
{

	int fd1=open("file1.txt",O_CREAT|O_RDWR,0666);
	int fd2=open("file2.txt",O_CREAT|O_RDWR,0666);
	int fd3=open("file3.txt",O_CREAT|O_RDWR,0666);
	int fd4=open("file4.txt",O_CREAT|O_RDWR,0666);
	int fd5=open("file5.txt",O_CREAT|O_RDWR,0666);
	
	printf("\nFile dis - %d\n",fd1);
	printf("\nFile dis - %d\n",fd2);
	printf("\nFile dis - %d\n",fd3);
	printf("\nFile dis - %d\n",fd4);
	printf("\nFile dis - %d\n",fd5);
	
	while(1);
	
}
/* 
use ls -ll in proc directoory
*/
