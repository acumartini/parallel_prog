all: stencil_serial stencil_openmp stencil_cilk stencil_tbb

stencil_serial: stencil_serial.cpp
	icpc -std=c++11 -o stencil_serial stencil_serial.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm -fopenmp

stencil_openmp: 
	icpc -std=c++11 -o stencil_mp stencil_mp.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm -fopenmp

stencil_tbb:
	icpc -std=c++11 -o stencil_tbb stencil_tbb.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm -fopenmp -ltbb

stencil_cilk:
	icpc -std=c++11 -o stencil_cilk stencil_cilk.cpp -lcilkrts -Wall -Wextra -lopencv_core -lopencv_highgui -lm -fopenmp

clean:
	rm -f *.o stencil_serial stencil_mp stencil_cilk stencil_tbb
	
.PHONY: clean
