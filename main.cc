#include <iostream>
#include <algorithm>
#include <vector>
#include "BSModel.h"
#include "RiskAnalysis.h"


int main(){

double fx_initial = 1295.0;	    // Initial Forward FX spot price (Korea Won per US dollar)
double fx_drift = 0.05;		    // Foreign exchange drift parameter
double fx_vol = 0.15; 		    // Foreign exchange volatility parameter
double r_dom = 0.035;		    // Domestic interest rate (Korea, 8-June-2023)
double r_foreign = 0.0525; 	    // Foreign interest rate(USA, 8-June-2023)
// double investment = 1000000.0;	// Initial investment amount in Korea Won
// double num_scenario = 10000; 	// Number of scenarios

double T = 1.0;
double conf_level = 0.95;
RiskAnalysis risk_analysis(fx_initial, fx_drift, fx_vol, r_dom, r_foreign);
double var = risk_analysis.cal_Risk(T, conf_level);

//std::vector<double> sensitivity = risk_analysis.cal_sensitivity();

std::cout << "Value at Risk(VaR) at " << (1.0 - conf_level) * 100 << "% Confidence level: " << var << std::endl;
std::cout << "======== Sensitivity ======== " << std::endl;
//std::cout << "FX Initial: " << sensitivity[0] << std::endl;
//std::cout << "FX Drift: " << sensitivity[1] << std::endl;
//std::cout << "FX Volatility: " << sensitivity[2] << std::endl;

return 0;
}
