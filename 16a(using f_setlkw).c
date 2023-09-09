/*
============================================================================
Name : 16a.c
Author : Aakash Bhardwaj
Description : Write a program to perform mandatory locking.
		a. Implement write lock
Date: 21th Aug, 2023.
============================================================================
*/
#include <sys/file.h>
#include<stdio.h>
#include<fcntl.h>
#include <unistd.h> 
int main(void){

int count=0;

struct flock lock;
lock.l_type=F_WRLCK;
lock.l_whence=SEEK_SET;
lock.l_start=0;
lock.l_len=0;
lock.l_pid=getpid();

int fd=open("16file.txt",O_RDWR);
if(fd==-1){
perror("File not opened\n");
}
printf("Before Critical Section\n");

fcntl(fd,F_SETLKW,&lock);
printf("Inside Critical Section\n");
printf("press enter to exit critical section \n");
getchar();

lock.l_type=F_UNLCK;
fcntl(fd,F_SETLK,&lock);
printf("Out of Critical Section");


return 0;
}
