CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SOURCE = BSModel.h FXModel.h IRModel.h MCSimulation.h RiskAnalysis.h

all: main main_bs
#######################################################
FXModel.o: FXModel.cc FXModel.h
	$(CXX) $(CXXFLAGS) -o FXModel.o -c FXModel.cc

IRModel.o: IRModel.cc IRModel.h
	$(CXX) $(CXXFLAGS) -o IRModel.o -c IRModel.cc

MCSimulation.o: MCSimulation.cc MCSimulation.h FXModel.h IRModel.h
	$(CXX) $(CXXFLAGS) -o MCSimulation.o -c MCSimulation.cc

RiskAnalysis.o: RiskAnalysis.cc RiskAnalysis.h MCSimulation.h
	$(CXX) $(CXXFLAGS) -o RiskAnalysis.o -c RiskAnalysis.cc

main.o: main.cc $(SOURCE)
	$(CXX) $(CXXFLAGS) -o main.o -c main.cc 

main: main.o BSModel.o FXModel.o IRModel.o MCSimulation.o RiskAnalysis.o 
	$(CXX) $(CXXFLAGS) -o main main.o BSModel.o FXModel.o IRModel.o MCSimulation.o RiskAnalysis.o

BSModel.o: BSModel.cc BSModel.h
	$(CXX) $(CXXFLAGS) -o BSModel.o -c BSModel.cc

main_bs.o: main_bs.cc BSModel.h
	$(CXX) $(CXXFLAGS) -o main_bs.o -c main_bs.cc 

main_bs: main_bs.o
	$(CXX) $(CXXFLAGS) -o main_bs main_bs.o BSModel.o

#######################################################
.Phony: all clean

clean:
	rm -f *.o main main_bs

