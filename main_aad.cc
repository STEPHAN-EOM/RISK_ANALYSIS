#include <iostream>
#include <chrono>
#include <sys/resource.h>
#include <sys/time.h>
#include "Number_v1.h"

template <class T>
T f(T spot_p, T strike_p, T risk_neutral, T vol, T maturity){
    
    auto d1 = (log(spot_p / strike_p) + (risk_neutral + 0.5 * vol * vol) * maturity) / (vol * sqrt(maturity)); 
    //auto d2 = d1 + (-vol) * sqrt(maturity);
    auto d2 = d1 -vol * sqrt(maturity);

    //auto y = spot_p * N(d1) + (-strike_p) * exp((-risk_neutral) * maturity) * N(d2);    
    auto y = spot_p * N(d1) -strike_p * exp((-risk_neutral) * maturity) * N(d2);    

    return y;
}

int main(){
   
    struct rusage usage;

    // Declare the initial parameters as Number
    Number spot_p = 1295.0;
    Number strike_p = 1300.0;
    double r_dom = 0.035;		                // Domestic interest rate (Korea, 8-June-2023)
    double r_foreign = 0.0525; 	                // Foreign interest rate(USA, 8-June-2023)
    Number risk_neutral = r_foreign - r_dom;
    Number vol = 0.15;
    Number maturity = 1.0; 

    auto start = std::chrono::high_resolution_clock::now();

    // Templated function for the Black-Scholes Fomula
    Number BS_Call = f(spot_p, strike_p, risk_neutral, vol, maturity);

    // Implement the Adjoint Differentiation
    BS_Call.Propagate_adj();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Print out the result of AAD
    std::cout << "\n========== Print out the result of AAD(The Greek Letters) ==========" << std::endl;
    std::cout << "Delta_Derivative with respect to spot price: " << spot_p.Get_adjoint() << std::endl;
    std::cout << "Rho_Derivative with respect to risk-free rate: " << risk_neutral.Get_adjoint() << std::endl;
    std::cout << "Vega_Derivative with respect to volatility(): " << vol.Get_adjoint() << std::endl;
    std::cout << "Theta_Derivative with respect to maturity: " << maturity.Get_adjoint() << std::endl;

    std::cout << "\nTime taken by MC Simulation for 5 sensitivities: " << duration << " milliseconds" << std::endl;

    int t = Number::tape.size();

    // Print out the values recorded on the tape
    std::cout << "\n========== Print out the values saved onto the tape(whose size = " << t << ") ==========" << std::endl;

    for (auto i = 0; i < t; ++i){
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