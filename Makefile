CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SOURCE = BSModel.h MCSimulation.h AAD.h  AAD_Tape.h

all: main main_aad main_mcaad main_pthread main_mcaad_pthread main_omp
#######################################################
MCSimulation.o: MCSimulation.cc MCSimulation.h 
	$(CXX) $(CXXFLAGS) -o MCSimulation.o -c MCSimulation.cc

BSModel.o: BSModel.cc BSModel.h
	$(CXX) $(CXXFLAGS) -o BSModel.o -c BSModel.cc	

AAD.o: AAD.cc AAD.h AAD_Tape.h
	$(CXX) $(CXXFLAGS) -o AAD.o -c AAD.cc

MCSimulation_aad.o: MCSimulation_aad.cc MCSimulation_aad.h Number_v1.h 
	$(CXX) $(CXXFLAGS) -o MCSimulation_aad.o -c MCSimulation_aad.cc

MCSimulation_pthread.o: MCSimulation_pthread.cc MCSimulation_pthread.h
	$(CXX) $(CXXFLAGS) -pthread -o MCSimulation_pthread.o -c MCSimulation_pthread.cc

MCSimulation_omp.o: MCSimulation_omp.cc MCSimulation_omp.h
	$(CXX) $(CXXFLAGS) -fopenmp -o MCSimulation_omp.o -c MCSimulation_omp.cc

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

main_pthread.o: main_pthread.cc MCSimulation_pthread.h
	$(CXX) $(CXXFLAGS) -pthread -o main_pthread.o -c main_pthread.cc

main_pthread: main_pthread.o MCSimulation_pthread.o
	$(CXX) $(CXXFLAGS) -pthread -o main_pthread main_pthread.o MCSimulation_pthread.o

main_mcaad_pthread.o: main_mcaad_pthread.cc Node_v1.h Number_Pthread.h 
	$(CXX) $(CXXFLAGS) -pthread -o main_mcaad_pthread.o -c main_mcaad_pthread.cc

main_mcaad_pthread: main_mcaad_pthread.o  
	$(CXX) $(CXXFLAGS) -pthread -o main_mcaad_pthread main_mcaad_pthread.o

main_omp.o: main_omp.cc MCSimulation_omp.h
	$(CXX) $(CXXFLAGS) -fopenmp -o main_omp.o -c main_omp.cc 

main_omp: main_omp.o MCSimulation_omp.o
	$(CXX) $(CXXFLAGS) -fopenmp -o main_omp main_omp.o MCSimulation_omp.o

#######################################################
.Phony: all clean

clean:
	rm -f *.o main main_aad main_mcaad main_pthread main_mcaad_pthread