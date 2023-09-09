/*
============================================================================
Name : 17_1.c
Author : Aakash Bhardwaj
Description : Write a program to open a file, store a ticket no and exit
Date: 21th Aug, 2023.
============================================================================
*/

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
int main(){
	int fd = open("ticket_file.txt", O_CREAT|O_RDWR,0666);
	int buf  = 1;
	write(fd, &buf, sizeof(buf));
	close(fd);
}

