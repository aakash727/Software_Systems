
/*
============================================================================
Name : 20.c
Author : Aakash Bhardwaj
Description : Find out the priority of your running program. Modify the priority with nice command.
Date: 5th september, 2023.
============================================================================
*/


#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
int main()
{

int pri=getpriority(PRIO_PROCESS,getpid());
if(pri==-1){
printf("error");
return 0;
}

printf("current priority(nice): %d\n",pri);
int x=nice(5);
int x2=nice(5);
printf("updated priority(nice): %d\n",x2);


int pri1=getpriority(PRIO_PROCESS,getpid());
if(pri1==-1){
printf("error");
return 0;
}
printf("rechecked priority(nice): %d\n",pri1);

while(1);
return 0;

}
