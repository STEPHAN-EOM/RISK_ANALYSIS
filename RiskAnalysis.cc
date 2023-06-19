#include "RiskAnalysis.h"


RiskAnalysis::RiskAnalysis(double fx_initial, double fx_drift, double fx_vol, double r_dom, double r_foreign)
    : simulation(fx_initial, fx_drift, fx_vol, r_dom, r_foreign){
        std::cout << "Constructing Risk Analysis" << "\n";
    };


double RiskAnalysis::cal_Risk(double T, double confi_level){
    int num_sims = 10;
    std::vector<double> fx_rates(num_sims);

    for (int i = 0; i < num_sims; ++i){
        double fx_rate = simulation.simulate_Path(T);
        fx_rates[i] = fx_rate;
    }

    std::sort(fx_rates.begin(), fx_rates.end());

    int index = static_cast<int> (num_sims * (1.0 - confi_level));
    double var = fx_rates[index];

    return var;
    }

 std::vector<double> RiskAnalysis::cal_sensitivity(){
    std::vector<double> sens(3);

    for (size_t i = 0; i < sens.size(); ++i){
        sens[i] = simulation.cal_sensitivity(i);
    }

    return sens;
}