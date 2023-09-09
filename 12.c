/*
============================================================================
Name : 12.c
Author : Aakash Bhardwaj
Description : Write a program to find out the opening mode of a file. Use fcntl.
Date: 12th Aug, 2023.
============================================================================
*/
#include <unistd.h>
#include <fcntl.h>
#include<stdio.h>
int main(int argc,char* argv[])
{
	printf("1.Read mode \n2.Read-Write Mode\n3.Write Mode\n");
	int n,fd=-1;
	scanf("%d",&n);
	if(n==1) fd= open(argv[1],O_RDONLY);
	else if(n==2) fd= open(argv[1],O_RDWR);
	else if(n==3) fd= open(argv[1],O_WRONLY);


	if(fd==-1){
		perror("error in opening file \n");
		return -1;
		}


	int flag=fcntl(fd,F_GETFL);

	if((flag&O_RDWR)==O_RDWR)
		printf("read-write mode");

	else if((flag&O_RDONLY)==O_RDONLY)
		printf("read only mode");

	else if((flag&O_WRONLY)==O_WRONLY)
		printf("read only mode");
	else
		printf("error");

return 0;
}
