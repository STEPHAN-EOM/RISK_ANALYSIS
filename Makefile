CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SOURCE = BSModel.h MCSimulation.h AAD.h  AAD_Tape.h

all: main main_bs
#######################################################
MCSimulation.o: MCSimulation.cc MCSimulation.h 
	$(CXX) $(CXXFLAGS) -o MCSimulation.o -c MCSimulation.cc

BSModel.o: BSModel.cc BSModel.h
	$(CXX) $(CXXFLAGS) -o BSModel.o -c BSModel.cc	

AAD.o: AAD.cc AAD.h AAD_Tape.h
	$(CXX) $(CXXFLAGS) -o AAD.o -c AAD.cc

main.o: main.cc $(SOURCE)
	$(CXX) $(CXXFLAGS) -o main.o -c main.cc 

main: main.o BSModel.o  MCSimulation.o  AAD.o
	$(CXX) $(CXXFLAGS) -o main main.o BSModel.o  MCSimulation.o AAD.o

main_bs.o: main_bs.cc BSModel.h
	$(CXX) $(CXXFLAGS) -o main_bs.o -c main_bs.cc 

main_bs: main_bs.o
	$(CXX) $(CXXFLAGS) -o main_bs main_bs.o BSModel.o

#######################################################
.Phony: all clean

clean:
	rm -f *.o main main_bs

