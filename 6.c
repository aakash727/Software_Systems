/*
============================================================================
Name : 6.c
Author : Aakash Bhardwaj
Description : Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
Date: 12th Aug, 2023.
============================================================================
*/
#include<stdio.h>
#include <unistd.h>
int main() {
    char buffer[100];
    int count = 0;
    int bytes_read;
 printf("If 1st character is 'X', Program will halt\n");

    while ((bytes_read=read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
            if (buffer[0] == 'X') {
                printf("Encountered 'X' at 0th index. Stopping...\n");
                break;  
            }
            printf("Output is:-  ");
            fflush(stdout);
            count=count+bytes_read-1;
            write(STDOUT_FILENO, buffer,bytes_read);
    }
     // Print the total count of input characters
    printf("Total count of input characters: %d\n", count);

    return 0;
}


