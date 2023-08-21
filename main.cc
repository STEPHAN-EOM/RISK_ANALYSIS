#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <sys/resource.h>
#include <sys/time.h>
#include "BSModel.h"
#include "MCSimulation.h"
#include "AAD.h"


int main(){

    struct rusage usage;

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

    // Step.4 Black-Scholes Fomular for FX Option
    double spot_p = 1295.0;
    double strike_p = 1300.0;
    double risk_neutral = r_foreign - r_dom;
    double vol = 0.15;
    double maturity = 1.0; 

    BSModel bsmodel(spot_p, strike_p, risk_neutral, vol, maturity);
    double fx_forward = bsmodel.calculateFXForward();
    double call_op = bsmodel.calculateCallOptionPrice();
    double put_op = bsmodel.calculatePutOptionPrice();

    std::cout << "\n======== Option Price(S(T0): "<< spot_p << ", K: " << strike_p << ") ========\n";
    std::cout << "Forward Price: " << fx_forward << "\n";
    std::cout << "Call Option Price: " << call_op << "\n";
    std::cout << "Put Option Price: " << put_op << "\n";

    double delta = bsmodel.calculateDelta();
    double gamma = bsmodel.calculateGamma();
    double theta = bsmodel.calculateTheta();
    double vega = bsmodel.calculateVega();
    double rho = bsmodel.calculateRho();

    std::cout << "\n======== Sensitivities(S(T0): "<< spot_p << ", K:" << strike_p << ") ========\n";
    std::cout << "Delta: " << delta << "\n";
    std::cout << "Gamma: " << gamma << "\n";
    std::cout << "Theta: " << theta << "\n";
    std::cout << "Vega: " << vega << "\n";
    std::cout << "Rho: " << rho << "\n";


    double spot_p1 = 1295.0;
    double strike_p1 = 1290.0;
    double risk_neutral1 = r_foreign - r_dom;
    double vol1 = 0.15;
    double maturity1 = 1.0; 

    BSModel bsmodel1(spot_p1, strike_p1, risk_neutral1, vol1, maturity1);
    //double fx_forward1 = bsmodel1.calculateFXForward();
    double call_op1 = bsmodel1.calculateCallOptionPrice();
    double put_op1 = bsmodel1.calculatePutOptionPrice();

    std::cout << "======== Option Price(S(t0): "<< spot_p1 << ", K: " << strike_p1 << ") ========\n";
    //std::cout << "Forward Price: " << fx_forward1 << "\n";
    std::cout << "Call Option Price: " << call_op1 << "\n";
    std::cout << "Put Option Price: " << put_op1 << "\n";

    double delta1 = bsmodel1.calculateDelta();
    double gamma1 = bsmodel1.calculateGamma();
    double theta1 = bsmodel1.calculateTheta();
    double vega1 = bsmodel1.calculateVega();
    double rho1 = bsmodel1.calculateRho();

    std::cout << "\n======== Sensitivities(S(t0): "<< spot_p1 << ", K: " << strike_p1 << ") ========\n";
    std::cout << "Delta: " << delta1 << "\n";
    std::cout << "Gamma: " << gamma1 << "\n";
    std::cout << "Theta: " << theta1 << "\n";
    std::cout << "Vega: " << vega1 << "\n";
    std::cout << "Rho: " << rho1 << "\n";
/*
    // Step.4 AAD for Black-Scholes Formula
    std::cout << "\n======== Checking the tape is working well ========\n";
    Tape tape;

    tape.add([]() { std::cout << "This is the first function\n"; });
    tape.add([]() { std::cout << "This is the second function\n"; });
    tape.add([]() { std::cout << "This is the third function\n"; });
    
    tape.rewind(); 
    */

    /*
    DualNumber S(spot_p, 1.0, &tape); 
    DualNumber K(strike_p, 0.0, &tape);  
    DualNumber MT(maturity, 0.0, &tape);
    DualNumber r(risk_neutral, 0.0, &tape);
    DualNumber sigma(vol, 0.0, &tape);

    // Calculate the Black-Scholes call price
    DualNumber call_price = BS_Call(S, K, MT, r, sigma);

    std::cout << "Call price: " << call_price.value << std::endl;

    //tape.rewind();

    // Compute Greeks
    std::cout << "Start Calling Greeks Function" << std::endl; 
    Greeks greeks = computeGreeks(spot_p, strike_p, maturity, risk_neutral, vol, tape);
    std::cout << "Finish Computing Greeks" << std::endl; 

    //tape.rewind();  

    std::cout << "Delta: " << greeks.delta << std::endl;
    std::cout << "Vega: " << greeks.vega << std::endl;
    std::cout << "Theta: " << greeks.theta << std::endl;
    std::cout << "Rho: " << greeks.rho << std::endl;
    std::cout << "Gamma: " << greeks.gamma << std::endl;
    std::cout << "Vanna: " << greeks.vanna << std::endl;
    */

    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        
        // Calculate user and system CPU times
        double userTime = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1e6;
        double systemTime = (double)usage.ru_stime.tv_sec + (double)usage.ru_stime.tv_usec / 1e6;

        std::cout << "User CPU time: " << userTime << " seconds" << std::endl;
        std::cout << "System CPU time: " << systemTime << " seconds" << std::endl;

        // If you want to print memory as well
        std::cout << "Max memory used (KB): " << usage.ru_maxrss << std::endl;

    } else {
        std::cerr << "Error retrieving usage information." << std::endl;
    }

    return 0;
}
