#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include "Number_v1.h"
// #include "MCSimulation_aad.h"

template <class T>
T f(T spot_p, T strike_p, T r_dom, T risk_neutral, T vol, T maturity, int num_sim, int num_step){
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    Number divide = 365.0 * num_step;
    auto dt = maturity / divide;
    auto first = (risk_neutral + (-(0.5 * vol * vol))) * dt;
    auto second = vol * sqrt(dt);

    T sum_fx = 0.0;
    T sum_op = 0.0;
    T payoff = 0.0;

    for (int i = 0; i < num_sim; ++i){
        T fx_rate = spot_p;

        for (int j = 0; j < num_step; ++j){
            T rand = distribution(generator);
            fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        payoff = max(fx_rate + (-strike_p), 0.0);

        sum_fx += fx_rate;
        sum_op += payoff;
    }

    T average_fx = sum_fx / num_sim;
    T average_op = sum_op / num_sim;
    T discount = exp(-r_dom);          // Discount by Domestic Interest Rate

    T result_fx = average_fx * discount;
    T result_op = average_op * discount;

    return result_fx;

}

int main(){
   
    // Declare the initial parameters as Number
    Number spot_p = 1295.0;
    Number strike_p = 1300.0;
    Number r_dom = 0.035;		                // Domestic interest rate (Korea, 8-June-2023)
    Number r_foreign = 0.0525; 	                // Foreign interest rate(USA, 8-June-2023)
    Number risk_neutral = r_foreign + (-r_dom);
    Number vol = 0.15;
    Number maturity = 1.0; 
    int num_sim = 5;
    int num_step = 10000;

    // Templated function for the Black-Scholes Fomula
    Number MC_Simulation = f(spot_p, strike_p, r_dom, risk_neutral, vol, maturity, num_sim, num_step);

    // Implement the Adjoint Differentiation
    MC_Simulation.Propagate_adj();

    // Print out the result of AAD
    std::cout << "\n========== Print out the result of AAD(The Greek Letters) ==========" << std::endl;
    std::cout << "Delta_Derivative with respect to spot price: " << spot_p.Get_adjoint() << std::endl;
    std::cout << "Rho_Derivative with respect to risk-free rate: " << risk_neutral.Get_adjoint() << std::endl;
    std::cout << "Vega_Derivative with respect to volatility(): " << vol.Get_adjoint() << std::endl;
    std::cout << "Theta_Derivative with respect to maturity: " << maturity.Get_adjoint() << std::endl;


    int t = Number::tape.size();

    // Print out the values recorded on the tape
    std::cout << "\n========== Print out the values saved onto the tape(whose size = " << t << ") ==========" << std::endl;
/*
    for (auto i = 0; i < t; ++i){
        std::cout << "tape(" << i << ") = " << Number::tape[i]->Get_result() << std::endl;
    }
*/


    return 0;
}