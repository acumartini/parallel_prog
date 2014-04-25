all: stencil_serial stencil_openmp

stencil_serial: stencil_serial.cpp
	icpc -std=c++11 -o stencil_serial stencil_serial.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm

stencil_openmp: 

clean:
	rm -f *.o stencil_serial
	
.PHONY: clean
