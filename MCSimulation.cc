#include "MCSimulation.h"

MCSimulation::MCSimulation(double fx_initial, double fx_drift, double fx_vol, double r_dom, double r_foreign) 
        : fx_model(fx_initial, fx_drift, fx_vol), ir_model(r_dom, r_foreign), rng(std::random_device()()) {
            std::cout << "Constructing Monte-Carlo Simulation" << "\n";
        };
        
double MCSimulation::simulate_Path(double T){

    double dt = 1 / 365;
    int num_step = static_cast<int> (T / dt);
    double fx_rate = fx_model.get_param(0);

    for (int i = 0; i < num_step; ++i){
        double rand_num = generate_RN();
        double fx_drift = ir_model.simulate(dt, rand_num);
        fx_rate = fx_model.simulate(dt, rand_num);
    }

    return fx_rate;
}

double MCSimulation::cal_sensitivity(size_t param_index){
    double eps = 1e-8;

    double fx_rate = simulate_Path(1.0);

    double perturbed_fx_rate = 0.0;
    double origin_param = get_param(param_index);
    double perturbed_param = origin_param + eps;
    set_param(param_index, perturbed_param);
    perturbed_fx_rate = simulate_Path(1.0);
    set_param(param_index, origin_param);

    double sensitivity = (perturbed_fx_rate - fx_rate) / eps;

    return sensitivity;
}

double MCSimulation::generate_RN(){
    double mu = 0.0;
    double std = 1.0;
    std::normal_distribution<double> dist(mu, std);
    return dist(rng);
}

double MCSimulation::get_param(size_t param_index) const{
    return fx_model.get_param(param_index);
}

void MCSimulation::set_param(size_t param_index, double value){
    fx_model.set_param(param_index, value);
}