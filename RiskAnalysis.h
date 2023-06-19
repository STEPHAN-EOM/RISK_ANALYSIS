#ifndef RISKANALYSIS_H
#define RISKANALYSIS_H

#include <vector>
#include <cmath>
#include "MCSimulation.h"

class RiskAnalysis{
    MCSimulation simulation;

    public:
        RiskAnalysis(double fx_initial, double fx_drift, double fx_vol, double r_dom, double r_foreign);

        ~RiskAnalysis(){
            std::cout << "Destroying Risk Analysis" << "\n";
        };

        double cal_Risk(double T, double confi_level);

        std::vector<double> cal_sensitivity();
};


#endif	// RISKANALYSIS_H