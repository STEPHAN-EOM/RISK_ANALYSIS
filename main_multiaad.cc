#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include <chrono>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "Parallel_aad.h"

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

    //const int NUM_THREADS = 4;
    int num_sim = 100;
    int num_step = 1;

    //Number local_sum = 0.0;

    //std::cout << "Done1" << std::endl;
    Parallel_AAD<Number> Simulation(spot_p, strike_p, risk_neutral, vol, maturity, r_dom, num_step, num_sim); 

    auto start = std::chrono::high_resolution_clock::now();
    //std::cout << "Done2" << std::endl;
    Number result = Simulation.Parallel_Simulation();
    //std::cout << "Done3" << std::endl;
    // Implement the Adjoint Differentiation

    int t = Number::tape.size();

    std::cout << "The number of tape size is " << t << std::endl;

    result.Propagate_adj();
    std::cout << "Done4" << std::endl;

    Number::Mark_tape();

    const int repeat_count = 1; // or whatever number of repetitions you need

    for (int i = 0; i < repeat_count; ++i) {
        // Rewind the tape to the state just after adjoint initialization
        Number::Rewind_Mark();

        // Execute back-propagation
        result.Propagate_adj();    
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time taken by MC Simulation with AAD: " << duration << " milliseconds" << std::endl;


    // Print out the result of AAD
    std::cout << "\n========== Print out the result of AAD(The Greek Letters) ==========" << std::endl;
    std::cout << "Delta_Derivative with respect to spot price: " << spot_p.Get_adjoint() << std::endl;
    std::cout << "Rho_Derivative with respect to risk-free rate: " << risk_neutral.Get_adjoint() << std::endl;
    std::cout << "Vega_Derivative with respect to volatility(): " << vol.Get_adjoint() << std::endl;
    std::cout << "Theta_Derivative with respect to maturity: " << maturity.Get_adjoint() << std::endl;


    int tape_size = Number::tape.size();


    // Print out the values recorded on the tape
    std::cout << "\n========== Print out the values saved onto the tape(whose size = " << tape_size << ") ==========" << std::endl;
    std::cout << "Result is " << Number::tape[tape_size-1]->Get_result() << std::endl;
/*

    // Clear up
    for (int t = 0; t < NUM_THREADS; ++t) {
        if (ThreadData[t]) {
            delete ThreadData[t];
            ThreadData[t] = nullptr;  // Set the pointer to nullptr after deletion
        }
    }

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