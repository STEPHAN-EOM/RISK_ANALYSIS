CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SOURCE = BSModel.h MCSimulation.h 

all: main main_aad main_mcaad main_pthread main_multiaad
#######################################################
MCSimulation.o: MCSimulation.cc MCSimulation.h 
	$(CXX) $(CXXFLAGS) -o MCSimulation.o -c MCSimulation.cc

BSModel.o: BSModel.cc BSModel.h
	$(CXX) $(CXXFLAGS) -o BSModel.o -c BSModel.cc	

MCSimulation_aad.o: MCSimulation_aad.cc MCSimulation_aad.h Number_v1.h 
	$(CXX) $(CXXFLAGS) -o MCSimulation_aad.o -c MCSimulation_aad.cc

MCSimulation_pthread.o: MCSimulation_pthread.cc MCSimulation_pthread.h
	$(CXX) $(CXXFLAGS) -pthread -o MCSimulation_pthread.o -c MCSimulation_pthread.cc

main.o: main.cc $(SOURCE)
	$(CXX) $(CXXFLAGS) -o main.o -c main.cc 

main: main.o BSModel.o  MCSimulation.o 
	$(CXX) $(CXXFLAGS) -o main main.o BSModel.o  MCSimulation.o

main_mcaad.o: main_mcaad.cc Node_v1.h Number_v1.h 
	$(CXX) $(CXXFLAGS) -o main_mcaad.o -c main_mcaad.cc

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

main_multiaad.o: main_multiaad.cc Node_v1.h Number_v2.h
	$(CXX) $(CXXFLAGS) -o main_multiaad.o -c main_multiaad.cc -fsanitize=address

main_multiaad: main_multiaad.o
	$(CXX) $(CXXFLAGS) -o main_multiaad main_multiaad.o -fsanitize=address

#######################################################
.Phony: all clean

clean:
	rm -f *.o main main_aad main_mcaad main_pthread main_multiaad 