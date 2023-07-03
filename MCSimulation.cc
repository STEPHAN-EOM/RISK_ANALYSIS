#include "MCSimulation.h"

MCSimulation::MCSimulation(double fx_initial, double fx_vol, double r_dom, double r_foreign, int num_sim, double T) 
        : fx_initial(fx_initial), fx_vol(fx_vol), r_dom(r_dom), r_foreign(r_foreign),
          num_sim(num_sim), maturity(T),
          distribution(0.0, 1.0) {
          std::cout << "Constructing Monte-Carlo Simulation" << std::endl;
        };
        

double MCSimulation::MC_Simulation(){
    
    // Initialise elementary parameters for the simulation
    double dt = maturity / 365.0;
    double ir_rate = r_foreign - r_dom;
    double first = (ir_rate - (0.5 * fx_vol * fx_vol)) * dt;
    double second = fx_vol * sqrt(dt);

    double sum = 0.0;

    for(int i = 0; i < num_sim; ++i){
        double rand = distribution(generator);
        double fx_rate = fx_initial * exp(first * 365) * exp(second * sqrt(365) * rand);

        sum += fx_rate;
    }

    double average = sum / num_sim;
    double discount = exp(-ir_rate);
    double result = average * discount;

    return result;
}

double MCSimulation::MC_Simulation_Option(double K){

    // Initialise elementary parameters for the simulation
    double dt = maturity / 365.0;
    double ir_rate = r_foreign - r_dom;
    double first = (ir_rate - (0.5 * fx_vol * fx_vol)) * dt;
    double second = fx_vol * sqrt(dt);

    double sum = 0.0;
    double payoff;
    for(int i = 0; i < num_sim; ++i){
        double rand = distribution(generator);
        double fx_rate = fx_initial * exp(first * 365) * exp(second * sqrt(365) * rand);

        payoff = std::max(fx_rate - K, 0.0);
        sum += payoff;
    }

    double average = sum / num_sim;
    double discount = exp(-ir_rate);
    double result = average * discount;

    return result;
}