CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

all: toSketchOMP

toSketchOMP: toSketchOMP.cpp
	gcc toSketchOMP.cpp -o toSketchOMP -std=c++11 $(CFLAGS) $(LIBS) -lstdc++ -lm -fopenmp

toSketchOMP_PAD: toSketchOMP_PAD.cpp
	gcc toSketchOMP_PAD.cpp -o toSketchOMP_PAD -std=c++11 $(CFLAGS) $(LIBS) -lstdc++ -lm -fopenmp

toSketchOMP_test: toSketchOMP_test.cpp
	gcc toSketchOMP_test.cpp -o toSketchOMP_test -std=c++11 $(CFLAGS) $(LIBS) -lstdc++ -lm -fopenmp

toSketchThreads: toSketchThreads.cpp
	gcc toSketchThreads.cpp -o toSketchThreads -std=c++11 $(CFLAGS) $(LIBS) -lstdc++ -lm -pthread

toSketch: toSketch.cpp
	gcc toSketch.cpp -o toSketch -std=c++11 $(CFLAGS) $(LIBS) -lstdc++ -lm -pthread

toSketch_pointers: toSketch_pointers.cpp
	gcc toSketch_pointers.cpp -o toSketch_pointers -std=c++11 $(CFLAGS) $(LIBS) -lstdc++