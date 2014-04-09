#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#define INTENSITY 100000000
#define THREADS 4

//Waste processor cycles proportional to the passed parameter
int doComputation(int numberOfComputations)
{
   int value=0;
   int i=0;
   for(i=0;i<numberOfComputations;i++){
      value+=i/50000000;
   }
   return value;
}


int main()
{
   int i=0;
   omp_set_num_threads(THREADS);
   int* values=(int*)malloc(sizeof(int)*omp_get_num_threads());
   int threadNum=0;
   //do this in parallel. Give each thread a threadNum, but they all get the same array
   #pragma omp parallel private(threadNum) shared(values) 
   {
      //give me your threadNum
      threadNum=omp_get_thread_num();
      printf("Thread %d\n",threadNum);
      //INTENSITY is just a scaling factor to make this not run in a microsecond
      //Each thread does work proportional to its threadNum      
      values[threadNum]=doComputation((1+threadNum)*INTENSITY);
   }
   //prove that you actually did something
   for(i=0;i<THREADS;i++)
   {
       printf("Thread %d calculated %d\n",i,values[i]);
   }
}
