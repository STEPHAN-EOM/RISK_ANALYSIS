#ifndef MCSIMULATION_H
#define MCSIMULATION_H

#include "IRModel.h"
#include "FXModel.h"
#include <iostream>
#include <random>
#include <vector>   

class MCSimulation{
    FXModel fx_model;
    IRModel ir_model;
    std::mt19937_64 rng;

    public:
        MCSimulation(double fx_initial, double fx_drift, double fx_vol, double r_dom, double r_foreign);
        ~MCSimulation(){
            std::cout << "Destroying Monte-Carlo Simulation" << "\n";
        };

        double simulate_Path(double T);
        double cal_sensitivity(size_t param_index);

    private:
        double generate_RN();
        double get_param(size_t param_index) const;
        void set_param(size_t param_index, double value);
};


#endif	// MCSIMULATION_H
