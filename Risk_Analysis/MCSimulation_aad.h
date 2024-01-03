#ifndef MCSIMULATION_H
#define MCSIMULATION_H

#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include "Number_v1.h"

template <class T>
class MCSimulation{

    public: 
        MCSimulation(T fx_initial, T fx_vol, T r_dom, T r_foreign, T maturity, int num_sim);
        ~MCSimulation(){
            std::cout << "Destroying Monte-Carlo Simulation" << "\n";
        };

        //std::pair<double, double> MC_Simulation(double K);
        T MC_Simulation(T K, int num_step);
        //double MC_Simulation_FW(double FW_Rate);
        

    private:
        T fx_initial;
        T fx_vol;
        T r_dom;
        T r_foreign;
        T maturity;
        int num_sim;
        std::default_random_engine generator;
        std::normal_distribution<double> distribution;
};


#endif	// MCSIMULATION_H