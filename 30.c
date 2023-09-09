/*
============================================================================
Name : 30.c
Author : Aakash Bhardwaj
Description : Write a program to run a script at a specific time using a daemon process.
Date: 5th september, 2023.
============================================================================
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {
    pid_t p = fork();
    if (p) {
        // Parent process
    } 
    else {
        // Child process
        setsid();
        chdir("/");
        umask(0);
        while (1){
            printf("Hello\n");
    	    sleep(5);		
    		}
    }
    return 0;
}

