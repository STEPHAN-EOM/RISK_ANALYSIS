#include "Parallel_aad.h"

// Constructor
template <class T>
Parallel_AAD<T>::Parallel_AAD(T& spot_p, T& strike_p, T& risk_neutral, T& vol, T& maturity, double& r_dom, int& num_step, int& num_sim)
    : spot_p(spot_p), strike_p(strike_p), risk_neutral(risk_neutral), vol(vol), 
    maturity(maturity), r_dom(r_dom), num_step(num_step), num_sim(num_sim), distribution(0.0, 1.0) {
}

// Thread function
template <class T>
void* Parallel_AAD<T>::ThreadFunction(void* arg) {
    ThreadData * data = static_cast<ThreadData*>(arg);

    // Mark the beginning of the local thread's tape
    Number::Mark_tape();

    T dt = data->maturity / (365.0 * data->num_step);
    T first = (data->risk_neutral - (0.5 * data->vol * data->vol)) * dt;
    T second = data->vol * sqrt(dt);

    for(int i = 0; i < data->num_sim_per_thread; ++i){
        T fx_rate = data->spot_p;
        for (int j = 0; j < data->num_step; ++j){
            double rand = data->distribution(data->generator); 
            fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        T payoff = max(fx_rate - data->strike_p, static_cast<T>(0.0));
     
        data->local_sum += payoff;
    }

    // Combine the local thread's tape into the global tape
    std::cout << "Start Data" << std::endl;
    for (const auto& node : Number::local_tape) {
        std::cout << "Result is of "<< std::this_thread::get_id() << "is " << node->Get_result() << " <<==" << std::endl;
    }

    std::lock_guard<std::mutex> lock(Number::tape_mutex);

    for(auto& node : Number::local_tape) {
        Number::tape.push_back(std::move(node));
    }
    Number::local_tape.clear();
    
    return nullptr;
}

template <class T>
T Parallel_AAD<T>::Parallel_Simulation() {
    const int NUM_THREADS = 4;
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
   
    for (int i = 0; i < NUM_THREADS; ++i) {
        /*threadData[i] = {spot_p, strike_p, risk_neutral, vol, maturity, r_dom, num_step, num_sim / NUM_THREADS, 
                        0.0, std::default_random_engine(i), std::normal_distribution<double>(0.0, 1.0)
        };
      */
        threadData[i].spot_p = spot_p;
        threadData[i].strike_p = strike_p;
        threadData[i].risk_neutral = risk_neutral;
        threadData[i].vol = vol;
        threadData[i].maturity = maturity;
        threadData[i].r_dom = r_dom;
        threadData[i].num_step = num_step;
        threadData[i].num_sim_per_thread = num_sim / NUM_THREADS;
        threadData[i].local_sum = 0.0;
        threadData[i].generator = std::default_random_engine(i);
        threadData[i].distribution = std::normal_distribution<double>(0.0, 1.0);

        int CreateResult = pthread_create(&threads[i], nullptr, &ThreadFunction, &threadData[i]);

        if (CreateResult != 0) {
            std::cerr << "Failed to create thread " << i << ". Error code: " << CreateResult << std::endl;
            threads[i] = 0;
        }
    }
/*
    T total_sum = 0.0;

    for (int i = 0; i < NUM_THREADS; ++i) {
        std::cout << "Tread <" << i << "> join" << std::endl; 
        pthread_join(threads[i], nullptr);
        total_sum += threadData[i].local_sum;
        std::cout << "Tread <" << i << "> is done" << std::endl; 
    }
    
    T average = total_sum / num_sim;
    T discount = exp(-r_dom);
    T result = average * discount;
*/

    
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], nullptr);
    }

    T total_sum = 0.0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        total_sum += threadData[i].local_sum;
    }

    T average = total_sum / (num_sim / NUM_THREADS);
    T discount = exp(-r_dom);
    T result = average * discount;
 
    return result;
}

template class Parallel_AAD<Number>;