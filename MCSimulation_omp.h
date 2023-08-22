#ifndef MCSIMULATION_OMP_H
#define MCSIMULATION_OMP_H

#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include <cmath>
#include <omp.h>

template <class T>
class MCSimulation{

    public: 
        MCSimulation(T fx_initial, T fx_vol, T r_dom, T r_foreign, int num_sim, T maturity);
        ~MCSimulation(){
            //std::cout << "Destroying Monte-Carlo Simulation" << "\n";
        };

        //std::pair<double, double> MC_Simulation(double K);
        //std::pair<double, double> MC_Simulation(double K, int num_step);
        std::pair<T, T> MC_Simulation(T K, int num_step);
        //double MC_Simulation_FW(double FW_Rate);
        

    private:
        T fx_initial;
        T fx_vol;
        T r_dom;
        T r_foreign;
        int num_sim;
        T maturity;
        std::default_random_engine generator;
        std::normal_distribution<double> distribution;
};


#endif	// MCSIMULATION_OMP_H