
/*
============================================================================
Name : 3.c
Author : Aakash Bhardwaj
Description : Write a program to create a file and print the file descriptor value. Use creat ( ) system call
Date: 5th Aug, 2023.
============================================================================
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
int main()
{
       int fd= creat("Sample_file_for_Prog3.txt", 0666);
       if(fd==-1){
       printf("File not created");
       }
       else
       printf("file discriptor is %d\n",fd);

return 0;
}
