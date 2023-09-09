/*
============================================================================
Name : 18_2.c
Author : Aakash Bhardwaj
Description : Implement record locking(write).
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
	struct flock writel;
	int recordID;
	printf("Enter record id you want to lock(write lock) out of 1,2,3: ");
	scanf("%d",&recordID);
	off_t recordoff=(off_t)(sizeof(struct Ticket) * (recordID -1));
	writel.l_type= F_WRLCK;
	writel.l_whence= SEEK_SET;
	writel.l_start=recordoff;
	writel.l_len=sizeof(struct Ticket);
	printf("You are going to access your record\n");
	fcntl(fd, F_SETLKW, &writel);
	printf("You are inside record no %d\n", recordID);
	lseek(fd,(recordID-1)*sizeof(struct Ticket), SEEK_SET);
	ssize_t readb=read(fd, &ticket, sizeof(ticket));
	printf("your ticket no. is %d\n",ticket.ticket_no);
	printf("enter to get out of record\n");
	getchar();
	getchar();
	writel.l_type=F_UNLCK;
	fcntl(fd, F_SETLKW, &writel);
	printf("write lock unlocked\n");
	
	close(fd);
	
	
	
return 0;
}

