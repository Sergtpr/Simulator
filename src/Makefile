CC = g++
LDFLAGS = `pkg-config --libs ibsimu-1.0.6dev`
CXXFLAGS = -Wall -g `pkg-config --cflags ibsimu-1.0.6dev`
all: fileman.o bfieldbldr.o confbldr.o geombldr.o parameters.o simulator.o solidbldr.o

fileman.o: fileman.cpp fileman.hpp
	$(CC) fileman.cpp -c -o fileman.o  $(CXXFLAGS)

bfieldbldr.o: bfieldbldr.cpp bfieldbldr.hpp
	$(CC) bfieldbldr.cpp -c -o bfieldbldr.o $(CXXFLAGS)

confbldr.o: confbldr.cpp confbldr.hpp
	$(CC) confbldr.cpp -c -o confbldr.o $(CXXFLAGS)

geombldr.o: geombldr.cpp geombldr.hpp
	$(CC) geombldr.cpp -c -o geombldr.o $(CXXFLAGS)

parameters.o: parameters.cpp parameters.hpp
	$(CC) parameters.cpp -c -o parameters.o $(CXXFLAGS)

simulator.o: simulator.cpp simulator.hpp
	$(CC) simulator.cpp -c -o simulator.o $(CXXFLAGS)

solidbldr.o: solidbldr.cpp solidbldr.hpp
	$(CC) solidbldr.cpp -c -o solidbldr.o $(CXXFLAGS)

clean:
	$(RM) *~ *.o 