#include <omp.h>
#include <time.h>
#define NUM_ITERS 10
#define NUM_NAMES 8
int main()
{
  char** nameList=(char**)malloc(sizeof(char*)*100);

  int i=0;
  #pragma omp parallel for
  for(i=0;i<NUM_ITERS;i++)
  {
    int foo=omp_get_thread_num();
    printf("Alien #%d says hello, they're  #%d in line.\n",foo,i);
    sleep(1);
  }
}
