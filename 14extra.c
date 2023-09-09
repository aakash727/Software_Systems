/*
============================================================================
Name : 18a.c
Author : Boppana Venkatesh
Description : Write a C, Ansi-style program to perform Record locking.
                       a. Implement write lock
Date: 28th Aug, 2023.
============================================================================
*/
#include <stdio.h>
#include<string.h>

int main(int argc, char *argv[]) {

    char *c = argv[1];
    int i = 0;

    while (*(c + i) != '.') {
        i++;
    }

    i++;

    if (*(c + i) == 'c') {
        printf("C file\n");
    } 
    else if (strcmp(c+i,"png")==0) {
        printf("png file\n");
    }
    else if (strcmp(c+i,"jpeg")==0) {
        printf("jpeg file\n");
    }
    else {
        printf("unidentified file\n");
    }

    return 0;
}

