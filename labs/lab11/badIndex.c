#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#define NUMVECTORS 5
#define NUMELEMENTS 8 
#define THREADS 4
//simple sum scan
void scan(int* scanMe){
   int i=0;
   for(i=0;i<NUMELEMENTS-1;i++)
   {
      scanMe[i+1]+=scanMe[i];
   }  
}
//scan a few vectors
void scanGroup(int** vectorGroup)
{
   int i=0;
   #pragma omp parallel for
   for(i=0;i<=NUMVECTORS;i++)
   {
      scan(vectorGroup[i]);
   }
}
int main(){
   omp_set_num_threads(THREADS);
   
   int **myVecs=(int**)malloc(sizeof(int*)*NUMVECTORS);
   int i=0;
   int j=0;
   for(j=0;j<NUMVECTORS;j++)
   {
      myVecs[j]=(int*)malloc(sizeof(int)*NUMELEMENTS);
      for(i=0;i<NUMELEMENTS;i++){
         myVecs[j][i]=i;
      }
   }
   
   scanGroup(myVecs);
   for(j=0;j<NUMVECTORS;j++)
   {
      for(i=0;i<NUMELEMENTS;i++)
      {
         printf("%3d->%3d\n",i,myVecs[j][i]);
      }
   }
}
