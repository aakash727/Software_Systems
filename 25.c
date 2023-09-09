
/*
============================================================================
Name : 25.c
Author : Aakash Bhardwaj
Description :Write a program to create three child processes. The parent should wait for a particular child (use
              waitpid system call).
Date: 5th september, 2023.
============================================================================
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t f1 = fork();

    if (f1 == 0) {
        printf("Child 1\n parent id %d\n child id %d\n\n", getppid(), getpid());
    } else {
        pid_t f2 = fork();

        if (f2 == 0) {
            sleep(10);
            printf("Child 2\n parent id %d\n child id %d\n\n", getppid(), getpid());
        } else {
            pid_t f3 = fork();
            if (f3 == 0) {
                printf("Child 3\n parent id %d\n child id %d\n\n", getppid(), getpid());
            } else {
                printf("Leftover part of parent before wait for Child 2\n");
                int status;
                int a = waitpid(f2, &status, WNOHANG);

                if (a == -1) {
                    perror("Waitpid error");
                    return 1;
                }

                if (WIFEXITED(status)) {
                    printf("Child 2 (PID %d) exited with status %d.\n\n", a, WEXITSTATUS(status));
                } else {
                    printf("Child (PID %d) did not exit normally.\n\n", 0);
                }

                printf("Leftover part of parent after wait for Child 2\n");
            }
        }
    }

    return 0;
}

