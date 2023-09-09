/*
============================================================================
Name     : 22.c
Author   : Aakash Bhardwaj
Description : 
    Write a program to open a file, create a child process using fork, 
    and then write to the file from both the child and parent processes. 
    Check the output of the file.
Date: 5th september, 2023.
============================================================================
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h> // Include the necessary header for open() and O_RDWR

int main() {
    int fd = open("22file.txt", O_RDWR | O_CREAT, 0777); // Open the file with read and write permissions

    pid_t returnedPid = fork(); // Create a child process

    if (returnedPid != 0) {
        // This is the parent process
        write(fd, "Written by parent  ", 19); // Write to the file
    } else {
        // This is the child process
        write(fd, "Written by child", 16); // Write to the file
    }

    close(fd); // Close the file
    return 0;
}

