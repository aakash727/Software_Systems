/*
============================================================================
Name : 27a.c
Author : Aakash Bhardwaj
Description : Execute ls -Rl cmd by execl.
Date: 5th september, 2023.
============================================================================
*/
#include <unistd.h>
#include <stdio.h>

int main() {
  char *args[] = {"ls", "-RL", NULL};

  int ret = execl("/bin/ls", args[0], args[1], NULL);
 printf("hello");
  if (ret < 0) {
    perror("execl");
    return 1;
  }

  return 0;
}

