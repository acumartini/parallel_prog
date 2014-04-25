default: matmul_mp

all: matmul_mp matmul_cilk matmul_tbb

matmul_mp:
	icc -o matmul_mp matmul_mp.c -fopenmp

matmul_cilk:

matmul_tbb:

clean:
	rm matmul_serial matmul_mp matmul_cilk matmul_tbb 2>/dev/null
