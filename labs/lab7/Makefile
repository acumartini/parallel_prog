default: matmul_serial

all: matmul_serial matmul_mp matmul_cilk matmul_tbb

matmul_serial: matmul.c
	icc -o matmul_serial matmul.c -fopenmp

matmul_mp:

matmul_cilk:

matmul_tbb:

clean:
	rm matmul_serial matmul_mp matmul_cilk matmul_tbb 2>/dev/null
