/*
============================================================================
Name : 9.c
Author : Aakash Bhardwaj
Description : Write a program to print the following information about a given file.
		a. inode
		b. number of hard links
		c. uid
		d. gid
		e. size
		f. block size
		g. number of blocks
		h. time of last access
		i. time of last modification
		j. time of last change
Date: 12th Aug, 2023.
============================================================================
*/
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>



int main(int argc, char *argv[]){

	int fd=open(argv[1],O_RDONLY);
	struct stat sb;
	if(fd==-1){
	printf("Error\n");
	return 0;
	}
	
	stat(argv[1],&sb);
		
	
	printf("inode no               %ld \n",sb.st_ino); 
	printf("no. of hard links      %ld \n",sb.st_nlink);
	printf("uid                    %u \n",sb.st_uid);
	printf("gid                    %u \n",sb.st_gid); 
	printf("block size             %ld\n",sb.st_blksize); 
	printf("No. of blocks          %ld\n",sb.st_blocks); 
	printf("Last access time       %ld\n",sb.st_atime); 
	printf("last modification time %ld \n",sb.st_mtime);
	printf("last change time       %ld\n",sb.st_ctime);
	
	close(fd);
}
