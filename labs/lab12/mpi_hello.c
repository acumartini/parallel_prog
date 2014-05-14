#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int err; // var to catch MPI errors... Could be useful if we checked

    err = MPI_Init(&argc, &argv);
  
    int rank;
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int tot;
    err = MPI_Comm_size(MPI_COMM_WORLD, &tot);

    printf("Hello World. I'm %d of %d\n",rank,tot);

    MPI_Finalize();
    return 0;
}
