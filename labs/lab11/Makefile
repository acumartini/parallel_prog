default: all

all: badIndex tbbNotQuiteRight

badIndex: badIndex.c
	icc -g -o badIndex badIndex.c -fopenmp

tbbNotQuiteRight: tbbNotQuiteRight.cpp
	icpc -g -o tbb tbbNotQuiteRight.cpp -ltbb

clean:
	rm tbb badIndex *.o 2>/dev/null
