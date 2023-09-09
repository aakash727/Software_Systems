/*
============================================================================
Name : 11c.c
Author : Aakash Bhardwaj
Description : Write a program to open a file, duplicate the file descriptor and append the file with both the
	      descriptors and check whether the file is updated properly or not.
	      c. use fcntl
Date: 28th Aug, 2023.
============================================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
int main(){
	int fdold=open("dup.txt",O_WRONLY |O_APPEND,0777);
	if(fdold == -1){
		perror("Failed to open the file");
		return 1;
	}
	
	int fdnew=fcntl(fdold,F_DUPFD,5);
	if(fdnew == -1){
		perror("Failed to open the file");
		return 1;
	}
	
	printf("File Descriptor old: %d\n",fdold);
        printf("File Descriptor new: %d\n",fdnew);
        	
        return 0;
}
