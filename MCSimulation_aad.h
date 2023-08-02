#ifndef MCSIMULATION_H
#define MCSIMULATION_H

#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include "Number_v1.h"

class MCSimulation{

    public: 
        MCSimulation(Number fx_initial, Number fx_vol, Number r_dom, Number r_foreign, Number T, int num_sim);
        ~MCSimulation(){
            std::cout << "Destroying Monte-Carlo Simulation" << "\n";
        };

        //std::pair<double, double> MC_Simulation(double K);
        std::pair<Number, Number> MC_Simulation(Number K, int num_step);
        //double MC_Simulation_FW(double FW_Rate);
        

    private:
        Number fx_initial;
        Number fx_vol;
        Number r_dom;
        Number r_foreign;
        Number maturity;
        int num_sim;
        std::default_random_engine generator;
        std::normal_distribution<double> distribution;
};


#endif	// MCSIMULATION_H