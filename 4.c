/*
============================================================================
Name : 4.c
Author : Aakash Bhardwaj
Description : Write a program to open an existing file with read write mode. Try O_EXCL flag also.
Date: 5th Aug, 2023.
============================================================================
*/

       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
       #include<stdio.h>
	int main(){
       int fd= open("3.c", O_RDWR|O_CREAT|O_EXCL,0666);
       if(fd==-1)
       printf("file not created\n");
       
       else 
       printf("fd of file is %d \n" ,fd);
	}
