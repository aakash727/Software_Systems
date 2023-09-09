/*
============================================================================
Name : 29.c
Author : Aakash Bhardwaj
Description : Write a program to get scheduling policy and modify the scheduling policy(SCHED_FIFO,SCHED_RR).
Date: 5th september, 2023.
============================================================================
*/
#include <sched.h>
#include <stdio.h>
#include <stdlib.h> // Include the necessary header for 'exit' function
#include <sys/types.h>
#include <unistd.h>

int main() {
    // Get the process ID and scheduling policy
    pid_t pid = getpid();
    int sched = sched_getscheduler(pid);

    // Determine and print the scheduling policy
    switch (sched) {
        case SCHED_FIFO:
            printf("FIFO scheduling is implemented\n");
            break;
        
        case SCHED_RR:
            printf("Round Robin scheduling is implemented\n");
            break;
        
        case SCHED_OTHER:
            printf("Other scheduling is implemented\n");
            break;
        
        default:
            printf("Unknown scheduling policy\n");
            break;
    }

    // Set the scheduling policy to SCHED_FIFO with a priority of 10
    struct sched_param shd;
    shd.sched_priority = 10;
    int ret = sched_setscheduler(pid, SCHED_FIFO, &shd);

    if (ret == -1) {
        perror("Error setting scheduling policy");
        exit(EXIT_FAILURE);
    }
    sched = sched_getscheduler(pid);

    // Determine and print the scheduling policy
    switch (sched) {
        case SCHED_FIFO:
            printf("updated with FIFO scheduling\n");
            break;
        
        case SCHED_RR:
            printf("updated with Round Robin scheduling\n");
            break;
        
        case SCHED_OTHER:
            printf("updated with Other scheduling\n");
            break;
        
        default:
            printf("Unknown scheduling policy\n");
            break;
    }

    return 0;
}

