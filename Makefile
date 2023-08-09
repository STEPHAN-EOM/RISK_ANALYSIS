CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SOURCE = BSModel.h MCSimulation.h AAD.h  AAD_Tape.h

all: main main_aad main_mcaad
#######################################################
MCSimulation.o: MCSimulation.cc MCSimulation.h 
	$(CXX) $(CXXFLAGS) -o MCSimulation.o -c MCSimulation.cc

BSModel.o: BSModel.cc BSModel.h
	$(CXX) $(CXXFLAGS) -o BSModel.o -c BSModel.cc	

AAD.o: AAD.cc AAD.h AAD_Tape.h
	$(CXX) $(CXXFLAGS) -o AAD.o -c AAD.cc

MCSimulation_aad.o: MCSimulation_aad.cc MCSimulation_aad.h Number_v1.h 
	$(CXX) $(CXXFLAGS) -o MCSimulation_aad.o -c MCSimulation_aad.cc

main.o: main.cc $(SOURCE)
	$(CXX) $(CXXFLAGS) -o main.o -c main.cc 

main: main.o BSModel.o  MCSimulation.o  AAD.o
	$(CXX) $(CXXFLAGS) -o main main.o BSModel.o  MCSimulation.o AAD.o

#main_mcaad.o: main_mcaad.cc Node_v1.h Number_v1.h MCSimulation_aad.h
main_mcaad.o: main_mcaad.cc Node_v1.h Number_v1.h 
	$(CXX) $(CXXFLAGS) -o main_mcaad.o -c main_mcaad.cc

#main_mcaad: main_mcaad.o MCSimulation_aad.o 
#	$(CXX) $(CXXFLAGS) -o main_mcaad main_mcaad.o MCSimulation_aad.o

main_mcaad: main_mcaad.o  
	$(CXX) $(CXXFLAGS) -o main_mcaad main_mcaad.o 

main_aad.o: main_aad.cc Node_v1.h Number_v1.h 
	$(CXX) $(CXXFLAGS) -o main_aad.o -c main_aad.cc 

main_aad: main_aad.o 
	$(CXX) $(CXXFLAGS) -o main_aad main_aad.o 

#######################################################
.Phony: all clean

clean:
	rm -f *.o main main_aad main_mcaad

