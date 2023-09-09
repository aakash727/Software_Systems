/*
============================================================================
Name : 27a.c
Author : Aakash Bhardwaj
Description : Execute ls -Rl cmd by execvp.
Date: 5th september, 2023.
============================================================================
*/
#include <unistd.h>
#include <stdio.h>

int main() {
    char *args[] = {"ls", "-RL", NULL};

    // Execute the "ls -RL" command using execlp
    int ret = execvp("ls", args	);

    // Check for errors in the execlp call
    if (ret < 0) {
        perror("execlp");
        return 1; // Return an error code
    }

    // The code will not reach this point if execlp is successful

    // The following line will not be executed if execlp succeeds
    printf("hello");

    return 0;
}

