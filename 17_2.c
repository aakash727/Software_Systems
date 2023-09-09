/*
============================================================================
Name : 17_2.c
Author : Aakash Bhardwaj
Description :write a program to open a file, implement write lock, read ticket no,increment asndd print new ticket no
Date: 21th Aug, 2023.
============================================================================
*/

#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

int main(){
	int fd = open("ticket_file.txt", O_RDWR);
	struct flock mylock;
        mylock.l_type = F_WRLCK;
        mylock.l_whence = SEEK_SET;
        mylock.l_start = 0;
        mylock.l_len = 0;
        mylock.l_pid = getpid();
        printf("Before entering CS: ");
	//acquire the lock 
        fcntl(fd, F_SETLKW, &mylock);
        printf("Entered into the critical section :\n ");
        
	int buf;
	read(fd, &buf, sizeof(buf));
	printf("The current ticket number is: %d\n", buf);
	buf = buf+1;
	lseek(fd, 0, SEEK_SET);
	printf("Now ticket number is: %d\n", buf);
	write(fd, &buf, sizeof(buf));
	
	
        mylock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &mylock);
        printf("unlocked \n"); 

	close(fd);
}
