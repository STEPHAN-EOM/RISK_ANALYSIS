#ifndef PARALLEL_AAD_H
#define PARALLEL_AAD_H

#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include <cmath>
#include <pthread.h>
#include "Number_Pthread.h"

template <class T>
class Parallel_AAD {
public:
    Parallel_AAD(T& spot_p, T& strike_p, T& risk_neutral, T& vol, T& maturity, double& r_dom, int& num_step, int& num_sim);
    ~Parallel_AAD(){};

    T Parallel_Simulation();

private:
    T spot_p, strike_p, risk_neutral, vol, maturity;
    double r_dom;
    int num_step;
    int num_sim;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;

    // Define the structure for thread data
    struct ThreadData {
        T spot_p;
        T strike_p;
        T risk_neutral;
        T vol;
        T maturity;
        double r_dom;
        int num_step;
        int num_sim_per_thread;
        T local_sum;
        std::default_random_engine generator;
        std::normal_distribution<double> distribution;

         // Default constructor
         ThreadData() : 
            spot_p(0.0), strike_p(0.0), risk_neutral(0.0), vol(0.0), maturity(0.0),
            r_dom(0.0), num_step(0), local_sum(0.0), distribution(0.0, 1.0) {}
    };

    static void* ThreadFunction(void* arg);
};

extern template class Parallel_AAD<Number>;

#endif // PARALLEL_AAD_H