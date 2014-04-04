#include <cilk/cilk.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define NUM_ITERS 10
#define NUM_NAMES 8
int main()
{
  char** nameList=(char**)malloc(sizeof(char*)*100);
  int i=0;
  
  cilk_for(i=0;i<NUM_ITERS;i++)
  {
    printf("Alien #%d says hello, they're  #%d in line.\n",pthread_self(),i);
    sleep(1);
  }
}
