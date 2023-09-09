/*
============================================================================
Name : 18_3.c
Author : Aakash Bhardwaj
Description : Implement record locking(read).
Date: 21th Aug, 2023.
============================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	struct Ticket
	{
	int ticket_no;
	}ticket;
	int fd=open("file_for_18_data.txt", O_RDWR | O_APPEND);
	if(fd==-1)
	{
		perror("error in opening file");
		close(fd);
		return 0;
	}
	struct flock readl;
	int recordID;
	printf("Enter record id you want to lock(read lock): ");
	scanf("%d",&recordID);
	off_t recordoff=(off_t)(sizeof(struct Ticket) * (recordID -1));
	readl.l_type= F_RDLCK;
	readl.l_whence= SEEK_SET;
	readl.l_start=recordoff;
	readl.l_len=sizeof(struct Ticket);
	printf("You are going to access your record\n");
	fcntl(fd, F_SETLKW, &readl);
	printf("You are inside record no %d\n", recordID);
	lseek(fd,(recordID-1)*sizeof(struct Ticket), SEEK_SET);
	ssize_t readb=read(fd, &ticket, sizeof(ticket));
	printf("your ticket no. is %d\n",ticket.ticket_no);
	printf("enter to get out of record\n");
	getchar();
	getchar();
	readl.l_type=F_UNLCK;
	fcntl(fd, F_SETLKW, &readl);
	printf("read lock unlocked\n");
	
	close(fd);
	
	
	
return 0;
}



