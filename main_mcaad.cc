#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include <chrono>
#include "Number_v1.h"
//#include "MCSimulation_aad.h"

template <class T>
T f(T spot_p, T strike_p, T r_dom, T risk_neutral, T vol, T maturity, int num_sim, int num_step){
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    //double divide = 365.0 * num_step;
    //T dt = maturity / divide;
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
        //payoff = std::max(fx_rate - strike_p, 0.0);

        //sum_fx += fx_rate;
        sum_op += payoff;
    }

    //T average_fx = sum_fx / num_sim;
    T average_op = sum_op / num_sim;
    T discount = exp(-r_dom);          // Discount by Domestic Interest Rate

    //T result_fx = average_fx * discount;
    T result_op = average_op * discount;

    // Record the intermediate results in the local tape
    //Number::tape.push_back(std::make_unique<Leaf>(result_fx.Get_value()));
    //Number::tape.push_back(std::make_unique<Leaf>(result_op.Get_value()));
    return result_op;

}

int main(){
   
    // Declare the initial parameters as Number
    Number spot_p = 1295.0;
    Number strike_p = 1300.0;
    Number r_dom = 0.035;		                // Domestic interest rate (Korea, 8-June-2023)
    Number r_foreign = 0.0525; 	                // Foreign interest rate(USA, 8-June-2023)
    Number risk_neutral = r_foreign -r_dom;
    Number vol = 0.15;
    Number maturity = 1.0;
    int num_sim = 120000;
    int num_step = 5;

    auto start = std::chrono::high_resolution_clock::now();
    // Templated function for the Black-Scholes Fomula
    Number MC_Simulation = f(spot_p, strike_p, r_dom, risk_neutral, vol, maturity, num_sim, num_step);
    
    // Implement the Adjoint Differentiation
    MC_Simulation.Propagate_adj();
    
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time taken by code: " << duration << " milliseconds" << std::endl;

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
/*
    for (auto i = 0; i < 30; ++i){
        std::cout << "tape(" << i << ") = " << Number::tape[i]->Get_result() << std::endl;
    }
*/


    return 0;
}