#include "MCSimulation_omp.h"

template <class T>
MCSimulation<T>::MCSimulation(T fx_initial, T fx_vol, T r_dom, T r_foreign, int num_sim, T maturity) 
        : fx_initial(fx_initial), fx_vol(fx_vol), r_dom(r_dom), r_foreign(r_foreign),
          num_sim(num_sim), maturity(maturity),
          distribution(0.0, 1.0) {
          //std::cout << "Constructing Monte-Carlo Simulation" << std::endl;
        };
        
//std::pair<double, double> MCSimulation::MC_Simulation(double K)
template <class T>
std::pair<T, T> MCSimulation<T>::MC_Simulation(T K, int num_step){

    // Initialise elementary parameters for the simulation
    //double dt = maturity / 365.0;
    T dt = maturity / (365.0 * num_step);
    T ir_rate = r_foreign - r_dom;
    T first = (ir_rate - (0.5 * fx_vol * fx_vol)) * dt;
    T second = fx_vol * sqrt(dt);

    T sum_fx = 0.0;
    T sum_op = 0.0;
    T payoff = 0.0;

    // Monte-Carlo Simulation
    for(int i = 0; i < num_sim; ++i){
        T fx_rate = fx_initial;

        for (int j = 0; j < num_step; ++j){
          double rand = distribution(generator);
          fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        payoff = std::max(fx_rate - K, static_cast<T>(0.0)); 

        sum_fx += fx_rate;
        sum_op += payoff;
    }

    T average_fx = sum_fx / num_sim;
    T average_op = sum_op / num_sim;
    T discount = exp(-r_dom);          // Discount by Domestic Interest Rate

    T result_fx = average_fx * discount;
    T result_op = average_op * discount;

    return std::make_pair(result_fx, result_op);
}

template class MCSimulation<double>;
template class MCSimulation<float>;