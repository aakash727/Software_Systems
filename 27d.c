/*
============================================================================
Name : 27d.c
Author : Aakash Bhardwaj
Description : Execute ls -Rl cmd by execv.
Date: 5th september, 2023.
============================================================================
*/
#include <unistd.h>
#include <stdio.h>

int main() {
  // Define the command to execute
  char *args[] = {"ls", "-Rh", NULL};

  // Execute the specified command with the given environment variables
  int ret = execv("/bin/ls", args);

  // Check for errors in the execle call
  if (ret < 0) {
    perror("execv");
    return 1; // Return an error code
  }

  // The code will not reach this point if execle is successful

  return 0;
}
 
