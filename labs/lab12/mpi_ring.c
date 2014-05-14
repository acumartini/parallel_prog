#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int tot;
    MPI_Comm_size(MPI_COMM_WORLD, &tot);

    char text[255];
    char outText[255];
    char allText[255*tot];
    MPI_Status mpistat;

    int sender = (rank-1+tot)%tot;
    int recver = (rank+1)%tot;
    
    if(rank==0) {
        sprintf(text,"Hello from rank %d",rank);
        MPI_Send(text,strlen(text)+1,MPI_CHAR,recver,0,MPI_COMM_WORLD);
        MPI_Recv(text,255,MPI_CHAR,sender,0,MPI_COMM_WORLD,&mpistat);
        sprintf(outText,"Rank %d received: %s",rank,text);
    } else {
        MPI_Recv(text,255,MPI_CHAR,sender,0,MPI_COMM_WORLD,&mpistat);
        sprintf(outText,"Rank %d received: %s",rank,text);
        sprintf(text,"Hello from rank %d",rank);
        MPI_Send(text,strlen(text)+1,MPI_CHAR,recver,0,MPI_COMM_WORLD);
    }
   
    MPI_Gather(outText,255,MPI_CHAR,allText,255,MPI_CHAR,0,MPI_COMM_WORLD);
    if(rank==0) {
        int i=0;
        for(i=0;i<tot;i++) {
            printf("%s\n",allText+i*255);
        }
    }
    
    MPI_Finalize();
    
    return 0;
}
