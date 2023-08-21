#include "MCSimulation_pthread.h"

// Constructor
template <class T>
MCSimulation<T>::MCSimulation(T fx_initial, T fx_vol, T r_dom, T r_foreign, int num_sim, T maturity) 
    : fx_initial(fx_initial), fx_vol(fx_vol), r_dom(r_dom), r_foreign(r_foreign),
       maturity(maturity), num_sim(num_sim), distribution(0.0, 1.0) {
}

// Thread function
template <class T>
void* MCSimulation<T>::threadFunction(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);

    T dt = data->maturity / (365.0 * data->num_step);
    T ir_rate = data->r_foreign - data->r_dom;
    T first = (ir_rate - (0.5 * data->fx_vol * data->fx_vol)) * dt;
    T second = data->fx_vol * sqrt(dt);

    for(int i = 0; i < data->num_sim_per_thread; ++i){
        T fx_rate = data->fx_initial;
        for (int j = 0; j < data->num_step; ++j){
            double rand = data->distribution(data->generator); 
            fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        T payoff = std::max(fx_rate - data->K, static_cast<T>(0.0));
        data->sum_fx += fx_rate;
        data->sum_op += payoff;
    }
    return nullptr;
}

// MC_Simulation method
template <class T>
std::pair<T, T> MCSimulation<T>::MC_Simulation(T K, int num_step) {
    const int NUM_THREADS = 4;
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
         threadData[i] = {fx_initial, fx_vol, r_dom, r_foreign, maturity, K, num_step, num_sim / NUM_THREADS, 
                        0.0, 0.0, std::default_random_engine(i),std::normal_distribution<double>(0.0, 1.0)
        };
        pthread_create(&threads[i], nullptr, threadFunction, &threadData[i]);
    }

    T total_sum_fx = 0.0, total_sum_op = 0.0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
        total_sum_fx += threadData[i].sum_fx;
        total_sum_op += threadData[i].sum_op;
    }

    T average_fx = total_sum_fx / num_sim;
    T average_op = total_sum_op / num_sim;
    T discount = exp(-r_dom * maturity);

    T result_fx = average_fx * discount;
    T result_op = average_op * discount;

    return std::make_pair(result_fx, result_op);
}

template class MCSimulation<double>;
template class MCSimulation<float>;