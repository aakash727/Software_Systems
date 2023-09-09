/*
============================================================================
Name : 18_1.c
Author : Aakash Bhardwaj
Description : create 3 records in a file.
Date: 21th Aug, 2023.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	struct ticket
	{
	 int ticket_no;
	};
	struct ticket t[3];
	int fd= open("file_for_18_data.txt", O_RDWR | O_CREAT, 0766);
       if(fd==-1)
       {
		perror("error in opening file");
       }
       for(int i=0;i<3;i++)
       {
       printf("Enter ticket number :");
       scanf("%d" ,&t[i].ticket_no);
       write(fd, &t[i], sizeof(struct ticket));
       }
     
        
       	return 0;
}
