/*
============================================================================
Name : 27c.c
Author : Aakash Bhardwaj
Description : Execute ls -Rl cmd by execle.
Date: 5th september, 2023.
============================================================================
*/
#include <unistd.h>
#include<stdio.h>

int main() {
  char *args[] = {"ls", "-Rh", NULL};
  char *envp[] = {"PATH=/bin:/usr/bin", NULL};

  int ret = execle("/bin/ls", args[0], args[1], NULL, envp);

  if (ret < 0) {
    perror("execle");
    return 1;
  }

  return 0;
}

