#include<time.h>
#include<tbb/tbb.h>
#include<stdio.h>
#include<stdlib.h>
#define ARRAYSIZE 4000
using namespace tbb;
class Init {
   public:
   int * output;
   Init(){
      output=(int*)malloc(sizeof(int)*ARRAYSIZE);
   }
   void operator() ( const blocked_range<int>& range ) const {
      for ( int i = range.begin(); i!=range.end(); ++i){
         output[i]=i;
      }
   }
};
class Scanner {
    public:
    int* input;
    Scanner(int* tinput){
       input = (int*)malloc(sizeof(int)*ARRAYSIZE);
       memcpy(input,tinput,sizeof(int)*ARRAYSIZE);
    }
    void operator() (const blocked_range<int>& range) const{
      int begin=range.begin();
      begin++;
      for ( int i = begin; i<=range.end(); ++i){
         input[i]=input[i-1]+input[i];
      }
    }
};
int main()
{
    Init init;
    parallel_for( blocked_range<int>(0,ARRAYSIZE), init);
    Scanner scanner(init.output);
    parallel_for( blocked_range<int>(0,ARRAYSIZE), scanner);
    for(int i=0;i<ARRAYSIZE;i++){
        printf("%d:%d\n",i,scanner.input[i]);
    }


     
    
}
