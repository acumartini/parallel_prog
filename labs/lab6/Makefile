all: crypto_serial crypto_mp crypto_tbb crypto_cilk

default: all

crypto_mp:
	icc -std=c99 -o mp_crypto mp_serial.c key.c -fopenmp

crypto_tbb:

crypto_cilk:
	icpc -o cilk_crypto cilk_serial.c key.c -lcilkrts

crypto_serial: serial.c key.c
	icc -std=c99 -o crypto_serial serial.c key.c

clean:
	rm -f *.o crypto_serial crypto_mp crypto_tbb crypto_cilk decryptedOut encryptedOut
