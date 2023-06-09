CXX = g++
CXXFLAGS = -W -Wall -std=c++17 

all: main
######################################

mc_simul.o: mc_simul.cpp mc_simul.h
	$(CXX) $(CXXFLAGS) -c mc_simul.cpp -o mc_simul.o

aad.o: aad.cpp aad.h
	$(CXX) $(CXXFLAGS) -c aad.cpp -o aad.o

main.o: main.cpp mc_simul.h aad.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

main: main.o mc_simul.o aad.o
	$(CXX) $(CXXFLAGS) -o main main.o mc_simul.o aad.o


######################################
.Phony: all clean

clean:
	rm -f *.o main
