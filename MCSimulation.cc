#include "MCSimulation.h"

MCSimulation::MCSimulation(double fx_initial, double fx_vol, double r_dom, double r_foreign, int num_sim, double T) 
        : fx_initial(fx_initial), fx_vol(fx_vol), r_dom(r_dom), r_foreign(r_foreign),
          num_sim(num_sim), maturity(T),
          distribution(0.0, 1.0) {
          std::cout << "Constructing Monte-Carlo Simulation" << std::endl;
        };
        
//std::pair<double, double> MCSimulation::MC_Simulation(double K)
std::pair<double, double> MCSimulation::MC_Simulation(double K, int num_step){
    
    // Initialise elementary parameters for the simulation
    //double dt = maturity / 365.0;
    double dt = maturity / (365.0 * num_step);
    double ir_rate = r_foreign - r_dom;
    double first = (ir_rate - (0.5 * fx_vol * fx_vol)) * dt;
    double second = fx_vol * sqrt(dt);

    double sum_fx = 0.0;
    double sum_op = 0.0;
    double payoff = 0.0;

    // Monte-Carlo Simulation
    for(int i = 0; i < num_sim; ++i){
        double fx_rate = fx_initial;

        for (int j = 0; j < num_step; ++j){
          double rand = distribution(generator);
          fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        payoff = std::max(fx_rate - K, 0.0); 

        sum_fx += fx_rate;
        sum_op += payoff;
    }

    double average_fx = sum_fx / num_sim;
    double average_op = sum_op / num_sim;
    double discount = exp(-r_dom);          // Discount by Domestic Interest Rate

    double result_fx = average_fx * discount;
    double result_op = average_op * discount;

    return std::make_pair(result_fx, result_op);
}