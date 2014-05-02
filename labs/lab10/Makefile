all: pipeline_serial pipeline_tbb

pipeline_tbb: pipeline_tbb.cpp
	icpc -g -std=c++11 -o pipeline_tbb pipeline_tbb.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lopencv_imgproc -ltbb


pipeline_serial: pipeline_serial.cpp
	icpc -g -std=c++11 -o pipeline_serial pipeline_serial.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lopencv_imgproc

clean:
	rm -f *.o pipeline_serial pipeline_tbb	
.PHONY: clean
