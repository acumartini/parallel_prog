all: crypto_serial crypto_mp crypto_tbb crypto_cilk

default: all

crypto_mp:

crypto_tbb:

crypto_cilk:

crypto_serial: serial.c key.c
	icc -std=c99 -o crypto_serial serial.c key.c

clean:
	rm -f *.o crypto_serial crypto_mp crypto_tbb crypto_cilk decryptedOut encryptedOut
