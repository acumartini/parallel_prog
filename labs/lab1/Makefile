make_mp: helloWorld.c
	icc -o mpWorld helloWorld.c -fopenmp

make_cilk: cilkWorld.c
	icc -o cilkWorld cilkWorld.c -lcilkrts

make_tbb: tbbWorld.cpp
	icpc -o tbbWorld tbbWorld.cpp -ltbb

all: make_mp make_cilk make_tbb

clean: 
	rm tbbWorld cilkWorld mpWorld
