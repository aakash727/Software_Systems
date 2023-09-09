/*
============================================================================
Name : 11a.c
Author : Aakash Bhardwaj
Description : Write a program to open a file, duplicate the file descriptor and append the file with both the
	      descriptors and check whether the file is updated properly or not.
	      a. use dup
Date: 28th Aug, 2023.
============================================================================
*/

#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
     
int main(){
//using dup to automatically assign duplicate file discriptor to old file.
int fd=open("7.c",O_RDONLY);
printf("old file discriptor is %d \n", fd);
int new_fd=dup(fd);
printf("new file discriptor is %d \n", new_fd);
struct stat sb1,sb2;
fstat(fd, &sb1);
fstat(new_fd, &sb2);

printf("Inode of old fd=%ld\n",sb1.st_ino);
printf("Inode of new fd=%ld\n",sb2.st_ino);

return 0;
}
