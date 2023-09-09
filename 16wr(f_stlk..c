/*
============================================================================
Name : 18a.c
Author : Boppana Venkatesh
Description : Write a C, Ansi-style program to perform Record locking.
                       a. Implement write lock
Date: 28th Aug, 2023.
============================================================================
*/
#include <sys/file.h>
#include<stdio.h>
#include<fcntl.h>
#include <unistd.h> 
#include<stdlib.h>

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

int val=0;
val=fcntl(fd,F_SETLK,&lock);

if(val==-1){
printf("Lock not set");
exit(1);
}


printf("Inside Critical Section\n");
printf("press enter to exit critical section \n");
getchar();
getchar();
lock.l_type=F_UNLCK;
fcntl(fd,F_SETLK,&lock);
printf("Out of Critical Section");


return 0;
}
