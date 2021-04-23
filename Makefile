CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

all: toSketchThreads

toSketchThreads: toSketchThreads.cpp
	gcc toSketchThreads.cpp -o toSketchThreads -std=c++11 $(CFLAGS) $(LIBS) -lstdc++ -lm -pthread

toSketch: toSketch.cpp
	gcc toSketch.cpp -o toSketch -std=c++11 $(CFLAGS) $(LIBS) -lstdc++ -lm -pthread