all: pass_init pass_mp pass_tbb pass_cilk

default: pass_init pass_mp pass_tbb pass_cilk

pass_mp: mp_pass.c
	icc -std=c99 -o mp_pass mp_pass.c -lcrypto -fopenmp

pass_tbb: tbb_pass.cpp
	icpc -o tbb_pass tbb_pass.cpp -ltbb -lcrypto

pass_cilk: cilk_pass.c
	icc -std=c99 -o cilk_pass cilk_pass.c -lcilkrts -lcrypto

pass_init: pass_init.c
	icc -std=c99 -o pass_init pass_init.c -lcrypto

clean:
	rm pass_init cilk_pass mp_pass tbb_pass
