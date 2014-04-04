CC=icc
CXX=icpc

mp: openmpNotQuiteRight.c
	$(CC) -o openmpNotQuiteRight openmpNotQuiteRight.c -fopenmp

cilk: cilkNotQuiteRight.c
	$(CC) -o cilkNotQuiteRight cilkNotQuiteRight.c -lcilkrts

tbb: tbbNotQuiteRight.cpp
	$(CXX) -o tbbNotQuiteRight tbbNotQuiteRight.cpp -ltbb

all: mp cilk tbb

clean: 
	rm tbbNotQuiteRight cilkNotQuiteRight openmpNotQuiteRight
