
/*
============================================================================
Name : 24.c
Author : Aakash Bhardwaj
Description : Write a program to create an orphan process.
Date: 5th september, 2023.
============================================================================
*/
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include <sys/wait.h>
int main(){

 	pid_t p=fork(); 
	
 	if(p){						//inside parent process
		//sleep(5);
		printf("Value of P in parent process is %d \n",p);
        	printf("I am parent having PID %d \n", getpid());
        	printf("My child is having PID as  %d \n" , p);
        	
		}

	else{						// inside child process
		//printf("My parent is having PId as %d \n" , getppid());
	        sleep(5); // if I write sleep here and remove above while, my process will be in zombie state. 
		printf("Value of P in child process is %d \n",p);
		printf("I am child having PID %d \n", getpid());
		printf("My parent is having PId as %d \n" , getppid());
		while(1);
   	 }
    return 0;	
}
                                                                                                              
                                                                                                              
                                                                                                             
       
