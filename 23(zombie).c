
/*
============================================================================
Name : 23(ZOMBIE).c
Author : Aakash Bhardwaj
Description : Write a program to create a Zombie state of the running program.
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
		sleep(5);
		printf("Value of P in parent process is %d \n",p);
        	printf("I am parent having PID %d \n", getpid());
        	printf("My child is having PID as  %d \n" , p);
        	while(1);
		}

	else{						// inside child process
	
	// sleep(20); // if I write sleep here and remove above while, my process will be in zombie state. 
		printf("Value of P in child process is %d \n",p);
		printf("I am child having PID %d \n", getpid());
		printf("My parent is having PId as %d \n" , getppid());
   	 }
    return 0;	
}
                                                                                                              
                                                                                                              
                                                                                                             
       
