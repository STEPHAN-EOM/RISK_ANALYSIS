#include <iostream>
#include <algorithm>
#include "mc_simul.h"
#include "aad.h"


int main(){

double fx_initial = 1295.0;	// Initial Forward FX spot price (Korea Won per US dollar)
double fx_drift = 0.05;		// Foreign exchange drift parameter
double fx_vol = 0.15; 		// Foreign exchange volatility parameter
double r_dom = 0.035;		// Domestic interest rate (Korea, 8-June-2023)
double r_foreign = 0.0525; 	// Foreign interest rate(USA, 8-June-2023)
double investment = 1000000.0;	// Initial investment amount in Korea Won
double num_scenario = 10000; 	// Number of scenarios

// Random Number Generator
std::random_device rd;
std::mt19937 gen(rd());
std::normal_distribution<> dist(0.0, 1.0);	// N(0, 1) 


// MC simulation
std::vector<double> portfolio_values;
std::vector<double> sensitivities;

for (int i = 0; i < num_scenario; ++i){

double fx_rv = dist(gen);
double fx_rate = generate_Scenario(fx_initial, fx_drift, fx_vol, 1.0, fx_rv);
double portfolio_value = calculate_Portfolio(fx_rate, r_dom, r_foreign, investment);
double sensitivity = cal_sensitivity(r_dom, r_foreign, investment);

portfolio_values.push_back(portfolio_value);
sensitivities.push_back(sensitivity);
}


// Risk Analysis
double confi_level = 0.95;
int index = static_cast<int> (num_scenario * (1 - confi_level));

std::sort(portfolio_values.begin(), portfolio_values.end());
double portfolio_var = portfolio_values[index];

std::cout << "Portfolio VaR at " << (1 - confi_level) * 100 << "% confidence level: " << portfolio_var << std::endl; 

// Calculate the average sensitivity
double ave_sensitivity = cal_ave_sensitivity(sensitivities);

std::cout << "Average sensitivity: " << ave_sensitivity << std::endl;

return 0;

}
