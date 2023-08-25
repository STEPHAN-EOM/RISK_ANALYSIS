#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include <chrono>
#include <sys/resource.h>
#include <sys/time.h>
#include "Number_v1.h"
//#include "Number_v2.h"
//#include "MCSimulation_aad.h"

template <class T>
T f(T spot_p, T strike_p, T risk_neutral, T vol, T maturity, double r_dom, int num_sim, int num_step){
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    T dt = maturity / (365.0 * num_step);
    T first = (risk_neutral - (0.5 * vol * vol)) * dt;
    T second = vol * sqrt(dt);

    //T sum_fx = 0.0;
    T sum_op = 0.0;
    T payoff = 0.0;


    for (int i = 0; i < num_sim; ++i){
        T fx_rate = spot_p;

        for (int j = 0; j < num_step; ++j){
            double rand = distribution(generator);
            fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        payoff = max(fx_rate - strike_p, 0.0);

        sum_op += payoff;
    }

    T average_op = sum_op / num_sim;
    T discount = exp(-r_dom);          // Discount by Domestic Interest Rate

    T result_op = average_op * discount;

/*
    // Second Version of back-propagation
    result_op.Propagate_adj();

    // Mark the tape after adjoint initialization
    Number::Mark_tape();

    const int repeat_count = 10; 
    for (int i = 0; i < repeat_count; ++i) {
        Number::Rewind_Mark();
        result_op.Propagate_adj();
    }
*/

    // Record the intermediate results in the local tape
    return result_op;
}

int main(){
   
   struct rusage usage;

    // Declare the initial parameters as Number
    Number spot_p = 1295.0;
    Number strike_p = 1300.0;
    //Number r_dom = 0.035;		                // Domestic interest rate (Korea, 8-June-2023)
    //Number r_foreign = 0.0525; 	                // Foreign interest rate(USA, 8-June-2023)
    Number risk_neutral = 0.0525 - 0.035;           // r_foreign -r_dom;
    Number vol = 0.15;
    Number maturity = 1.0;
    double r_dom = 0.035;
    int num_sim = 100000;
    int num_step = 5;

    auto start = std::chrono::high_resolution_clock::now();
    // Templated function for the Black-Scholes Fomula
    Number MC_Simulation = f(spot_p, strike_p, risk_neutral, vol, maturity, r_dom, num_sim, num_step);

    // Implement the Adjoint Differentiation
    MC_Simulation.Propagate_adj();


    Number::Mark_tape();

    const int repeat_count = 9; // or whatever number of repetitions you need

    for (int i = 0; i < repeat_count; ++i) {
        // Rewind the tape to the state just after adjoint initialization
        Number::Rewind_Mark();

        // Execute back-propagation
        MC_Simulation.Propagate_adj();    
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time taken by MC Simulation with AAD: " << duration << " milliseconds" << std::endl;

    // Templated function for the Black-Scholes Fomula
    //MCSimulation<Number> Cal_Sensitivities(spot_p, r_dom, risk_neutral, vol, maturity, num_sim);
    //Number result = Cal_Sensitivities.MC_Simulation(strike_p, num_step);

    // Implement the Adjoint Differentiation
    //result.Propagate_adj();

    // Print out the result of AAD
    std::cout << "\n========== Print out the result of AAD(The Greek Letters) ==========" << std::endl;
    //std::cout << "The result of the simulation" << MC_Simulation.Get_value() << std::endl;
    std::cout << "Delta_Derivative with respect to spot price: " << spot_p.Get_adjoint() << std::endl;
    std::cout << "Rho_Derivative with respect to risk-free rate: " << risk_neutral.Get_adjoint() << std::endl;
    std::cout << "Vega_Derivative with respect to volatility(): " << vol.Get_adjoint() << std::endl;
    std::cout << "Theta_Derivative with respect to maturity: " << maturity.Get_adjoint() << std::endl;


    int t = Number::tape.size();

    // Print out the values recorded on the tape
    std::cout << "\n========== Print out the values saved onto the tape(whose size = " << t << ") ==========" << std::endl;
    std::cout << "Result is " << Number::tape[t-1]->Get_result() << std::endl;

    for (auto i = 0; i < 30; ++i){
        std::cout << "tape(" << i << ") = " << Number::tape[i]->Get_result() << std::endl;
    }


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