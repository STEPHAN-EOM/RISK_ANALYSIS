#ifndef MCSIMULATION_H
#define MCSIMULATION_H

#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>

class MCSimulation{

    public: 
        MCSimulation(double fx_initial, double fx_vol, double r_dom, double r_foreign, int num_sim, double T);
        ~MCSimulation(){
            std::cout << "Destroying Monte-Carlo Simulation" << "\n";
        };

        double MC_Simulation();
        double MC_Simulation_FW(double FW_Rate);
        double MC_Simulation_Option(double K);
        

    private:
        double fx_initial;
        double fx_vol;
        double r_dom;
        double r_foreign;
        int num_sim;
        double maturity;
        std::default_random_engine generator;
        std::normal_distribution<double> distribution;
};


#endif	// MCSIMULATION_H
