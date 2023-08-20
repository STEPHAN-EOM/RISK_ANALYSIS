#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include "MCSimulation.h"

int main() {
    
     // Step.1 Monte-Carlo Simulation for FX rate
    double fx_initial = 1295.0;	    // Initial Forward FX spot price (Korea Won per US dollar)
    double fx_vol = 0.15; 		    // Foreign exchange volatility parameter
    double r_dom = 0.035;		    // Domestic interest rate (Korea, 8-June-2023)
    double r_foreign = 0.0525; 	    // Foreign interest rate(USA, 8-June-2023)

    int num_sims = 1000000;          // Number of Simulations
    double T = 1.0;                 // Maturity(Year)
    double eps = 0.001;             // Epsilon
    
    auto start = std::chrono::high_resolution_clock::now();

    // Create Monte-Carlo simulation Object
    MCSimulation FX_simulator(fx_initial, fx_vol, r_dom, r_foreign, num_sims, T);           // Base Model
    MCSimulation FX_simulator1(fx_initial + eps, fx_vol, r_dom, r_foreign, num_sims, T);    // For Sensitivity, Delta
    MCSimulation FX_simulator2(fx_initial, fx_vol + eps, r_dom, r_foreign, num_sims, T);    // For Sensitivity, Vega
    MCSimulation FX_simulator3(fx_initial, fx_vol, r_dom, r_foreign + eps, num_sims, T);
    MCSimulation FX_simulator4(fx_initial, fx_vol + eps, r_dom, r_foreign, num_sims, T + eps);
    // Calculate Simulation for FX rate
    // Step.2 Pricing an FX forward and FX Option using this framework
    double Strike = 1300.0;
    int num_step = 5;
    std::pair<double, double> FX_result = FX_simulator.MC_Simulation(Strike, num_step);

    // Display Results
    std::cout << "\nSimulation Result: " << FX_result.first << std::endl;
    std::cout << "Simulation Result(FX Option): " << FX_result.second << std::endl;

    
    // Step.3 Use bump and reval to compute options on these instruments
    std::pair<double, double> Option_result1 = FX_simulator1.MC_Simulation(Strike, num_step);
    std::pair<double, double> Option_result2 = FX_simulator2.MC_Simulation(Strike, num_step);
    std::pair<double, double> Option_result3 = FX_simulator3.MC_Simulation(Strike, num_step);
    std::pair<double, double> Option_result4 = FX_simulator4.MC_Simulation(Strike, num_step);
    std::pair<double, double> Option_result5 = FX_simulator1.MC_Simulation(Strike + eps, num_step);
    double delta_bump = (Option_result1.second - FX_result.second) / eps;
    double vega_bump = (Option_result2.second - FX_result.second) / eps;
    double rho_bump = (Option_result3.second - FX_result.second) / eps;
    double theta_bump = (Option_result4.second - FX_result.second) / eps;
    double strike_bump = (Option_result5.second - Option_result1.second) / eps;

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Display Results 
    std::cout << "Simulation Result(FX Option1): " << Option_result1.second << std::endl;
    std::cout << "Simulation Result(FX Option2): " << Option_result2.second << std::endl;

    std::cout << "\n======== Sensitivities by bump and reval with (S(T0): "<< fx_initial << ", K:" << Strike << ") ========\n"; 
    std::cout << "Simulation Result(Option_delta): " << delta_bump << std::endl;
    std::cout << "Simulation Result(Option_vega): " << vega_bump << std::endl;
    std::cout << "Simulation Result(Option_rho): " << rho_bump << std::endl;
    std::cout << "Simulation Result(Option_theta): " << theta_bump << std::endl;
    std::cout << "Simulation Result(Option_wrt Strike Price): " << strike_bump << std::endl;

    std::cout << "\nTime taken by MC Simulation for 5 sensitivities: " << duration << " milliseconds" << std::endl;
    
    return 0;
}
