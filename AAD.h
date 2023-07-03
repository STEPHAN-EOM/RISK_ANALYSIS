#ifndef AAD_H
#define AAD_H

#include <iostream>
#include <cmath>

class DualNumber {
    public:
        double value;
        double derivative;
        double second_derivative;

        DualNumber(double value, double derivative, double second_derivative);
        ~DualNumber(){
            std::cout << "Destroying Dual Number" << std::endl;
        };

        DualNumber operator+(const DualNumber& other);
        DualNumber operator-(const DualNumber& other);
        DualNumber operator*(const DualNumber& other);
        DualNumber operator/(const DualNumber& other);
};

DualNumber N(DualNumber X);
DualNumber BS_Call(DualNumber S, DualNumber K, DualNumber T, DualNumber r, DualNumber sigma);


struct Greeks {
    double delta;
    double gamma;
    double vega;
    double theta;
    double rho;
    double vanna;
};

Greeks computeGreeks(double S_val, double K_val, double T_val, double r_val, double sigma_val);

#endif  // AAD_H