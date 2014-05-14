all: mpi_ring mpi_hello

mpi_ring: mpi_ring.c
	mpicc -o mpi_ring mpi_ring.c

mpi_hello: mpi_hello.c
	mpicc -o mpi_hello mpi_hello.c

clean:
	rm -f *.o mpi_ring mpi_hello 	
.PHONY: clean
