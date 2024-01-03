#include <iostream>
#include <random> 
#include <functional>
#include <chrono>
#include <future>
#include <sys/resource.h>
#include <sys/time.h>
#include "Number_v2.h"

std::mutex Number::tape_mutex;
std::vector<std::unique_ptr<Node>> Number::global_tape;
thread_local std::vector<std::unique_ptr<Node>> Number::tape;

std::mutex rng_mutex; // Mutex to protect the random number generator
std::default_random_engine generator; // Global random number generator

template <class T>
T Simulation(T spot_p, T strike_p, T risk_neutral, T vol, T maturity, int num_step, int sim_thread){
    //std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    T dt = maturity / (365.0 * num_step);
    T first = (risk_neutral - (0.5 * vol * vol)) * dt;
    T second = vol * sqrt(dt);

    T local_sum = 0.0;
    T payoff = 0.0;
    for (int i = 0; i < sim_thread; ++i){
        T fx_rate = spot_p *1;

        for (int j = 0; j < num_step; ++j){
            double rand;
            {
                std::lock_guard<std::mutex> lock(rng_mutex); // Lock the mutex before accessing the generator
                rand = distribution(generator); // Generate random number
            } // Mutex is unlocked here when lock goes out of scope

            fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        payoff = max(fx_rate - strike_p, static_cast<T>(0.0));
        local_sum += payoff;
    } 

    std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;

//    std::cout << "Start Data Transferring" << std::endl;
/*
{
std::lock_guard<std::mutex> lock(Number::tape_mutex);

for (auto& node : Number::tape) {
// Move the data from the local tape to the global tape
//std::cout << "Adding mynode to global_tape, address: " << node.get() << std::endl;
Number::global_tape.push_back(std::move(node));
}

}
std::cout << "Data Transfer is done" << std::endl;
Number::tape.clear();  // Clear the thread-local tape
*/
    //return result;
    return local_sum;


}

template <class T>
T f(T spot_p, T strike_p, T risk_neutral, T vol, T maturity, double r_dom, int num_sim, int num_step){
    int num_threads = 8;
    std::vector<std::future<T>> futures;

    int sim_thread = num_sim / num_threads;
    for(int i = 0; i < num_threads; ++i) {
        futures.push_back(std::async(std::launch::async, Simulation<T>, spot_p, strike_p, risk_neutral, vol, maturity, num_step, sim_thread));
    }

    //Number::global_tape.clear();  
    //std::cout << "START Combining" << std::endl;

    T total_sum = 0.0;
    for(auto& fut : futures) {
        total_sum += fut.get();
    }

    T average = total_sum / static_cast<int>(num_sim);
    T discount = exp(-r_dom);          
    T result = average * discount;

    //std::cout << "The number of Global tape size is " << Number::global_tape.size() << std::endl;

    return result;
}

int main(){

    struct rusage usage;
    Number::global_tape.clear();


    // Declare the initial parameters as Number
    Number spot_p = 1295.0;
    Number strike_p = 1300.0;
    //Number r_dom = 0.035;		                // Domestic interest rate (Korea, 8-June-2023)
    //Number r_foreign = 0.0525; 	                // Foreign interest rate(USA, 8-June-2023)
    Number risk_neutral = 0.0525 - 0.035;           // r_foreign -r_dom;
    Number vol = 0.15;
    Number maturity = 1.0;
    double r_dom = 0.035;

    int num_sim = 50000;
    int num_step = 5;

    //Number local_sum = 0.0;
    auto start = std::chrono::high_resolution_clock::now();
    Number result  = f(spot_p, strike_p, risk_neutral, vol, maturity, r_dom, num_sim, num_step); 
    
    Number::Mark_tape();
    result.Propagate_adj();

   

   const int repeat_count = 2; // or whatever number of repetitions you need

    for (int i = 0; i < repeat_count; ++i) {
        // Rewind the tape to the state just after adjoint initialization
        Number::Rewind_Mark();

        // Execute back-propagation
        result.Propagate_adj();    
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time taken by MC Simulation with AAD: " << duration << " milliseconds" << std::endl;

    //int size =  Number::global_tape.size();
/*   
    // Print out the values recorded on the tape
    std::cout << "\n========== Print out the values saved onto the tape(whose size = " << Number::global_tape.size() << ") ==========" << std::endl;
   for (int i = 0; i < 40; i++){
    //for (int i = 0; i < size; i++){
    std::cout << "Global Tape[" << i  <<"] = " << Number::global_tape[i]->Get_result() << std::endl;
    }
*/

    // Print out the result of AAD
    std::cout << "\n========== Print out the result of AAD(The Greek Letters) ==========" << std::endl;
    std::cout << "Delta_Derivative with respect to spot price: " << spot_p.Get_adjoint() << std::endl;
    std::cout << "Rho_Derivative with respect to risk-free rate: " << risk_neutral.Get_adjoint() << std::endl;
    std::cout << "Vega_Derivative with respect to volatility(): " << vol.Get_adjoint() << std::endl;
    std::cout << "Theta_Derivative with respect to maturity: " << maturity.Get_adjoint() << std::endl;


    int tape_size = Number::global_tape.size();

    // Print out the values recorded on the tape
    std::cout << "\n========== Print out the values saved onto the tape(whose size = " << tape_size << ") ==========" << std::endl;
    if (!Number::global_tape.empty()) {
   std::cout << "Result is " << Number::global_tape[tape_size-1]->Get_result() << std::endl;
} else {
   std::cout << "Tape is empty." << std::endl;
}

std::cout << "Time taken by MC Simulation with AAD: " << duration << " milliseconds" << std::endl;

   if (getrusage(RUSAGE_SELF, &usage) == 0) {
        
        // Calculate user and system CPU times
        double userTime = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1e6;
        double systemTime = (double)usage.ru_stime.tv_sec + (double)usage.ru_stime.tv_usec / 1e6;

        std::cout << "User CPU time: " << userTime << " seconds" << std::endl;
        std::cout << "System CPU time: " << systemTime << " seconds" << std::endl;

        // If you want to print memory as well
        std::cout << "Max memory used (KB): " << usage.ru_maxrss / 1024 << std::endl;

    } else {
        std::cerr << "Error retrieving usage information." << std::endl;
    }

    return 0;
}