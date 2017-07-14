CXX           = g++
CXXFLAGS      = -std=c++11 $(DEFINES) 

all: main.o
	g++ -Ofast -march=native -std=c++11  -o giscup main.o -Wall

main.o: main.cpp freespace.h \
		trajectory.h \
		point.h \
		cell.h \
		input.h \
		RTree.h \
		query.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp
clean:
	$(RM) *.o $(objects) run
