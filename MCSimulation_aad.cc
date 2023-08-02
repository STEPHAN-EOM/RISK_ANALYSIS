#include "MCSimulation_aad.h"

MCSimulation::MCSimulation(Number fx_initial, Number fx_vol, Number r_dom, Number r_foreign, Number T, int num_sim) 
        : fx_initial(fx_initial), fx_vol(fx_vol), r_dom(r_dom), r_foreign(r_foreign),
          maturity(T), num_sim(num_sim), distribution(0.0, 1.0) {
          std::cout << "Constructing Monte-Carlo Simulation" << std::endl;
        };
        
//std::pair<double, double> MCSimulation::MC_Simulation(double K)
std::pair<Number, Number> MCSimulation::MC_Simulation(Number K, int num_step){
    
    // Initialise elementary parameters for the simulation
    //double dt = maturity / 365.0;
    Number dt = maturity / (365.0 * num_step);
    Number ir_rate = r_foreign +(-r_dom);
    Number first = (ir_rate + (-(0.5 * fx_vol * fx_vol))) * dt;
    Number second = fx_vol * sqrt(dt);

    Number sum_fx = 0.0;
    Number sum_op = 0.0;
    Number payoff = 0.0;

    // Monte-Carlo Simulation
    for(int i = 0; i < num_sim; ++i){
        Number fx_rate = fx_initial;

        for (int j = 0; j < num_step; ++j){
          Number rand = distribution(generator);
          fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        payoff = max(fx_rate + (- K), 0.0); 

        sum_fx += fx_rate;
        sum_op += payoff;
    }

    Number average_fx = sum_fx / num_sim;
    Number average_op = sum_op / num_sim;
    Number discount = exp(-r_dom);          // Discount by Domestic Interest Rate

    Number result_fx = average_fx * discount;
    Number result_op = average_op * discount;

    return std::make_pair(result_fx, result_op);
}