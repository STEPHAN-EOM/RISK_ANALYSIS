#ifndef FX_MODEL_H
#define FX_MODEL_H

#include <iostream>
#include <random>
#include <cmath>

struct FXModel {
    double fx_initial;     // Initial Forward FX spot price (Korea Won per US dollar)
    double fx_drift;       // Foreign exchange drift parameter
    double fx_vol;         // Foreign exchange volatility parameter

public:
    FXModel(double initial, double drift, double vol);
    ~FXModel(){
        std::cout << "Destroying FX Model" << std::endl;
    };

    double simulate(double dt, double rand_num) const;
    double calculateSensitivity(size_t parameter_index, double dt, double rand_num);

    double get_param(size_t parameter_index) const;
    void set_param(size_t parameter_index, double value);
};

#endif // FX_MODEL_H
