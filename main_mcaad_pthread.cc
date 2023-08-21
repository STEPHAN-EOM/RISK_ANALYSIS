#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include <chrono>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "Number_v1.h"

// Define a structure to pass data to each thread
template <class T>
struct ThreadData {
    T spot_p, strike_p, risk_neutral, vol, maturity;
    double r_dom;
    int start_sim, end_sim, num_step;
    T *sum_op;
    pthread_mutex_t *mutex;
};

// The function executed by each thread
template <class T>
void* MonteCarloThread(void* arg) {
    ThreadData<T>* data = (ThreadData<T>*)arg;
    
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    T dt = data->maturity / (365.0 * data->num_step);
    T first = (data->risk_neutral - (0.5 * data->vol * data->vol)) * dt;
    T second = data->vol * sqrt(dt);

    for (int i = data->start_sim; i < data->end_sim; ++i){
        T fx_rate = data->spot_p;
        for (int j = 0; j < data->num_step; ++j){
            double rand = distribution(generator);
            fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        T payoff = max(fx_rate - data -> strike_p, 0.0);
        
        // Synchronize update to shared data
        pthread_mutex_lock(data->mutex);
        *data->sum_op += payoff; // Assuming Get_value() gives the double value
        pthread_mutex_unlock(data->mutex);
    }
    
    return NULL;
}

void* Helper_Function(void* arg) {
    return MonteCarloThread<Number>(arg);
}

int main(){

    struct rusage usage;
   
    // Declare the initial parameters as Number
    Number spot_p = 1295.0;
    Number strike_p = 1300.0;
    //Number r_dom = 0.035;		                // Domestic interest rate (Korea, 8-June-2023)
    //Number r_foreign = 0.0525; 	                // Foreign interest rate(USA, 8-June-2023)
    Number risk_neutral = 0.0525 - 0.035;           // r_foreign -r_dom;
    Number vol = 0.15;
    Number maturity = 1.0;
    double r_dom = 0.035;

    const int NUM_THREADS = 4;
    int num_sim = 100;
    int num_step = 5;

    // Define number of threads and initialize thread attributes
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Initialize mutex for shared data access
    pthread_mutex_t sum_mutex;
    pthread_mutex_init(&sum_mutex, NULL);

    Number sum_op = 0.0;

    // Split the simulations across threads
    int sims_per_thread = num_sim / NUM_THREADS;


    ThreadData<Number>* threadData[NUM_THREADS];
    for (int t = 0; t < NUM_THREADS; ++t) {
        threadData[t] = new ThreadData<Number>{spot_p, strike_p, risk_neutral, vol, maturity, r_dom, t * sims_per_thread, (t + 1) * sims_per_thread, num_step, &sum_op, &sum_mutex};

        if (!threadData[t]) {
           std::cerr << "Memory allocation failed for threadData[" << t << "]" << std::endl;
            exit(EXIT_FAILURE);
        }

        int createResult = pthread_create(&threads[t], &attr, Helper_Function, (void *)threadData[t]);
        if (createResult) {
            std::cerr << "Failed to create thread " << t << ". Error code: " << createResult << std::endl;
            threads[t] = 0;  // Mark thread as not created
        }
    }

    std::cout << "The following part causes Segmentation Fault" << std::endl;
    // Wait for all threads to complete
    for (int t = 0; t < NUM_THREADS; ++t) {
       if (threads[t] != 0) {  // Join thread only if it was successfully created
            pthread_join(threads[t], NULL);
            threads[t] = 0;  // Mark thread as joined
        }
    }

    std::cout << "Done" << std::endl;
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&sum_mutex);

    Number average_op = sum_op / num_sim;
    Number discount = exp(-r_dom);
    Number result_op = average_op * discount;
    auto start = std::chrono::high_resolution_clock::now();
    
    // Implement the Adjoint Differentiation
    result_op.Propagate_adj();

    Number::Mark_tape();

    const int repeat_count = 1; // or whatever number of repetitions you need

    for (int i = 0; i < repeat_count; ++i) {
        // Rewind the tape to the state just after adjoint initialization
        Number::Rewind_Mark();

        // Execute back-propagation
        result_op.Propagate_adj();    
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time taken by MC Simulation with AAD: " << duration << " milliseconds" << std::endl;


    // Print out the result of AAD
    std::cout << "\n========== Print out the result of AAD(The Greek Letters) ==========" << std::endl;
    std::cout << "Delta_Derivative with respect to spot price: " << spot_p.Get_adjoint() << std::endl;
    std::cout << "Rho_Derivative with respect to risk-free rate: " << risk_neutral.Get_adjoint() << std::endl;
    std::cout << "Vega_Derivative with respect to volatility(): " << vol.Get_adjoint() << std::endl;
    std::cout << "Theta_Derivative with respect to maturity: " << maturity.Get_adjoint() << std::endl;


    int tape_size = Number::tape.size();


    // Print out the values recorded on the tape
    std::cout << "\n========== Print out the values saved onto the tape(whose size = " << tape_size << ") ==========" << std::endl;
    std::cout << "Result is " << Number::tape[tape_size-1]->Get_result() << std::endl;


    // Clear up
    for (int t = 0; t < NUM_THREADS; ++t) {
        if (threadData[t]) {
            delete threadData[t];
            threadData[t] = nullptr;  // Set the pointer to nullptr after deletion
        }
    }


   if (getrusage(RUSAGE_SELF, &usage) == 0) {
        
        // Calculate user and system CPU times
        double userTime = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1e6;
        double systemTime = (double)usage.ru_stime.tv_sec + (double)usage.ru_stime.tv_usec / 1e6;

        std::cout << "User CPU time: " << userTime << " seconds" << std::endl;
        std::cout << "System CPU time: " << systemTime << " seconds" << std::endl;

        // If you want to print memory as well
        std::cout << "Max memory used (KB): " << usage.ru_maxrss << std::endl;

    } else {
        std::cerr << "Error retrieving usage information." << std::endl;
    }

    return 0;
}