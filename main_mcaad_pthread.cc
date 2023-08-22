#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include <chrono>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "Number_Pthread.h"

// Define a structure to pass data to each thread
template <class T>
struct ThreadData {
    T spot_p, strike_p, risk_neutral, vol, maturity;
    double r_dom;
    int start_sim, end_sim, num_step;
    T local_sum;
    std::vector<std::unique_ptr<Node>> local_tape;
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
    
    // T local_sum = 0.0;
    // std::vector<std::unique_ptr<Node>> local_tape;
    for (int i = data->start_sim; i < data->end_sim; ++i){
        T fx_rate = data->spot_p;
        for (int j = 0; j < data->num_step; ++j){
            double rand = distribution(generator);
            fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        T payoff = max(fx_rate - data->strike_p, 0.0);

        // Update the local tape with new nodes
        Node* n = new Leaf(payoff.Get_value());
        data->local_tape.push_back(std::unique_ptr<Node>(n));
        data->local_sum += payoff;
        
        //local_sum += payoff;

        //Node* n = new Leaf(payoff.Get_value());
        //local_tape.push_back(std::unique_ptr<Node>(n));
        // Record operations on the local tape
        //data->local_tape.push_back(std::unique_ptr<Node>(payoff.node()));
        
        // Update local sum
        //data->local_sum_op += payoff;
    }

    // data->local_sum = local_sum;
    // data->local_tape = std::move(local_tape);
    
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

    // Initialise mutex for shared data access
    // pthread_mutex_t sum_mutex;
    // pthread_mutex_init(&sum_mutex, NULL);
 
    // Number sum_op = 0.0;

    // Split the simulations across threads
    int sims_per_thread = num_sim / NUM_THREADS;


    ThreadData<Number>* threadData[NUM_THREADS];
    for (int t = 0; t < NUM_THREADS; ++t) {
        threadData[t] = new ThreadData<Number>{ spot_p, strike_p, risk_neutral, vol, maturity, r_dom,
                                                t * sims_per_thread, (t + 1) * sims_per_thread, num_step,
                                                0.0,{} };

        if (!threadData[t]) {
            std::cerr << "Memory allocation failed for threadData[" << t << "]" << std::endl;
            exit(EXIT_FAILURE);
        }

        int createResult = pthread_create(&threads[t], &attr, Helper_Function, (void *)threadData[t]);
        if (createResult) {
            std::cerr << "Failed to create thread " << t << ". Error code: " << createResult << std::endl;
            threads[t] = 0;
        }
    }

    std::cout << "The following part causes Segmentation Fault" << std::endl;

    Number sum_op = 0.0;

     // Wait for all threads to complete
    for (int t = 0; t < NUM_THREADS; ++t) {
        if (threads[t] != 0) {
            std::cout << "Done2" << std::endl;
            pthread_join(threads[t], NULL);
            std::cout << "Done3" << std::endl;
            sum_op += threadData[t]->local_sum;
            std::cout << "Done4" << std::endl;
            threads[t] = 0;
            std::cout << "Done5" << std::endl;
        }
    }

    std::cout << "Done" << std::endl;

   // pthread_mutex_destroy(&sum_mutex);

    
    // Clean up
    pthread_attr_destroy(&attr);

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