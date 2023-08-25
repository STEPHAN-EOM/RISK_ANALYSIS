#include <iostream>
#include <random> 
#include <functional>
#include <algorithm>
#include <utility>
#include <chrono>
#include <future>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "Number_v2.h"

std::mutex Number::tape_mutex;
std::vector<std::shared_ptr<Node>> Number::global_tape;
thread_local std::vector<std::shared_ptr<Node>> Number::tape;


template <class T>
T Simulation(T spot_p, T strike_p, T risk_neutral, T vol, T maturity, int num_step, int sim_thread){
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    Number::global_tape.clear();

    Number a(10.0);
    Number b(5.0);
    Number d(20.0);
    a + b;
    Number e = a+b;

    //T a = spot_p * strike_p;
    //T b = spot_p / num_step;
    T local_sum = e;
    int t = Number::tape.size();
    std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
    std::cout << "The number of tape size is " << Number::tape.size() << std::endl;

for(const auto& node : Number::tape) {
    std::cout << "Result is of "<< std::this_thread::get_id() << "is " << node->Get_result() << std::endl;
}
    std::lock_guard<std::mutex> lock(Number::tape_mutex);
    //Number::global_tape.insert(Number::global_tape.end(), std::make_move_iterator(tape.begin()), std::make_move_iterator(tape.end()));

    //std::lock_guard<std::mutex> guard(Number::tape_mutex);
    for(auto& node : Number::tape) {
        Number::global_tape.push_back(std::move(node));
    }

    Number::tape.clear();
    
    return local_sum;
}
/*

template <class T>
T Simulation(T spot_p, T strike_p, T risk_neutral, T vol, T maturity, int num_step, int sim_thread){
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    T dt = maturity / (365.0 * num_step);
    T first = (risk_neutral - (0.5 * vol * vol)) * dt;
    T second = vol * sqrt(dt);

    T local_sum = 0.0;
    T payoff = 0.0;
    for (int i = 0; i < sim_thread; ++i){
        T fx_rate = spot_p;
    // std::cout << "Number of sim_thread" << sim_thread << std::endl;
        for (int j = 0; j < num_step; ++j){
            double rand = distribution(generator);
            //std::cout << "random Number = " << rand << std::endl;

            fx_rate *= exp(first * 365) * exp(second * sqrt(365) * rand);
        }

        T payoff = max(fx_rate - strike_p, static_cast<T>(0.0));
        local_sum += payoff;
    } 
    //int t = Number::tape.size();
     std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
    //std::cout << "The number of tape size is " << t << std::endl;
{
    std::lock_guard<std::mutex> lock(Number::tape_mutex);
    //Number::global_tape.insert(Number::global_tape.end(), std::make_move_iterator(tape.begin()), std::make_move_iterator(tape.end()));

    //std::lock_guard<std::mutex> guard(Number::tape_mutex);
    for(auto& node : Number::tape) {
        Number::global_tape.push_back(std::move(node));
    }
}
    Number::tape.clear();
    
    return local_sum;
}
*/
template <class T>
T f(T spot_p, T strike_p, T risk_neutral, T vol, T maturity, double r_dom, int num_sim, int num_step){
    //int num_threads = std::thread::hardware_concurrency();  // optimal number of threads
    int num_threads = 4;
    std::vector<std::future<T>> futures;

    int sim_thread = num_sim / num_threads;
    for(int i = 0; i < num_threads; ++i) {
        futures.push_back(std::async(std::launch::async, Simulation<T>, spot_p, strike_p, risk_neutral, vol, maturity, num_step, sim_thread));
        //futures.push_back(std::async(std::launch::async, Simulation<T>, num_step, sim_thread))
    }

    Number::global_tape.clear();  
   
    T total_sum = 0.0;
    for(auto& fut : futures) {
        total_sum += fut.get();
    }
    T average = total_sum / num_sim;
    T discount = exp(-r_dom);          
    T result = average * discount;

/*
    result.Set_adjoint(1.0);


    int t = Number::global_tape.size();

    std::cout << "The number of global tape size is " << t << std::endl;

    for (int i = global_tape.size() - 1; i >= 0; --i) {
        global_tape[i]->Propagate_adj();
    }

    Number::Mark_tape();

    const int repeat_count = 9; // or whatever number of repetitions you need

    for (int i = 0; i < repeat_count; ++i) {
        // Rewind the tape to the state just after adjoint initialization
        Number::Rewind_Mark();

        // Execute back-propagation
        for (int k = global_tape.size() - 1; k >= 0; --k) {
            global_tape[k]->Propagate_adj();
        }  
    }
*/
    //result.Propagate_adj();

    return result;
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

    //const int NUM_THREADS = 4;
    int num_sim = 100;
    int num_step = 5;

    Number::global_tape.clear();

    //Number local_sum = 0.0;
    auto start = std::chrono::high_resolution_clock::now();
    //std::cout << "Done1" << std::endl;
    Number result  = f(spot_p, strike_p, risk_neutral, vol, maturity, r_dom, num_sim, num_step); 

 

    int t = Number::global_tape.size();

    std::cout << "The number of Global tape size is " << t << std::endl;

    // Print out the values recorded on the tape
    std::cout << "\n========== Print out the values saved onto the tape(whose size = " << t << ") ==========" << std::endl;
    for (int i = 0; i < 13; i++){
    std::cout << "Result is " << Number::tape[i]->Get_result() << std::endl;
    }

    //global_tape.Propagate_adj();

/*
    Number::Mark_tape();

    const int repeat_count = 0; // or whatever number of repetitions you need

    for (int i = 0; i < repeat_count; ++i) {
        // Rewind the tape to the state just after adjoint initialization
        Number::Rewind_Mark();

        // Execute back-propagation
        result.Propagate_adj();    
    }
*/
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

    if (!Number::tape.empty()) {
   std::cout << "Result is " << Number::tape[tape_size-1]->Get_result() << std::endl;
} else {
   std::cout << "Tape is empty." << std::endl;
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