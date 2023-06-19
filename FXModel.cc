#include "FXModel.h"


FXModel:: FXModel(double initial, double drift, double vol) : fx_initial(initial), fx_drift(drift), fx_vol(vol) {
    std::cout << "Constructing FXModel" << std::endl;
}


double FXModel::simulate(double dt, double rand_num) const {
    double fx_rate = fx_initial * exp((fx_drift - 0.5 * fx_vol * fx_vol) * dt + fx_vol * sqrt(dt) * rand_num);    
    return fx_rate;
}


double FXModel::calculateSensitivity(size_t parameter_index, double dt, double rand_num) {
    double epsilon = 1e-8;                      // Small epsilon for finite difference approximation

    double fx_rate = simulate(dt, rand_num);    // Base simulation

    // Perturb the model parameter by epsilon and simulate the path
    double perturbed_fx_rate = 0.0;
    double original_param_value = get_param(parameter_index);
    double perturbed_param_value = original_param_value + epsilon;
    set_param(parameter_index, perturbed_param_value);
    perturbed_fx_rate = simulate(dt, rand_num);
    set_param(parameter_index, original_param_value);

    // Calculate the sensitivity using finite difference approximation
    double sensitivity = (perturbed_fx_rate - fx_rate) / epsilon;

    return sensitivity;
}


double FXModel::get_param(size_t parameter_index) const {
    switch (parameter_index) {
        case 0:
            return fx_initial;
        case 1:
            return fx_drift;
        case 2:
            return fx_vol;
        default:
            return 0.0;
    }
}


void FXModel::set_param(size_t parameter_index, double value) {
    switch (parameter_index) {
        case 0:
            fx_initial = value;
            break;
        case 1:
            fx_drift = value;
            break;
        case 2:
            fx_vol = value;
            break;
    }
}
