#ifndef MCSIMULATION_PTHREAD_H
#define MCSIMULATION_PTHREAD_H

#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include <cmath>
#include <pthread.h>

template <class T>
class MCSimulation {
public:
    MCSimulation(T fx_initial, T fx_vol, T r_dom, T r_foreign, int num_sim, T maturity);
    ~MCSimulation(){};

    std::pair<T, T> MC_Simulation(T K, int num_step);

private:
    T fx_initial, fx_vol, r_dom, r_foreign, maturity;
    int num_sim;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;

    // Define the structure for thread data
    struct ThreadData {
        T fx_initial;
        T fx_vol;
        T r_dom;
        T r_foreign;
        T maturity;
        T K;
        int num_step;
        int num_sim_per_thread;
        T sum_fx;
        T sum_op;
        std::default_random_engine generator;
        std::normal_distribution<double> distribution;
    };

    static void* threadFunction(void* arg);
};

// Explicit instantiation declarations
extern template class MCSimulation<double>;
extern template class MCSimulation<float>;

#endif // MCSIMULATION_PTHREAD_H