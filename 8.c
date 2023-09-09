/*
============================================================================
Name : 8.c
Author : Aakash Bhardwaj
Description : Write a program to open a file in read only mode, read line by line and display line as it is read.
		Close the file when end of file is reached.
Date: 12th Aug, 2023.
============================================================================
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<stdio.h>
int main(){
	char buffer[10000];
	int fd=open("6.c",O_RDONLY);
	char c;
	int count=0;
	while(read(fd,&c,1)){
		
		if(c=='\n'){
			printf("\n ---------------\n");
			fflush(stdout);
		}
		write(STDOUT_FILENO,&c,1);
	
  }   
 }
