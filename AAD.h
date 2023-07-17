#ifndef AAD_H
#define AAD_H

#include <iostream>
#include <cmath>
#include "AAD_Tape.h"

class DualNumber {
    public:
        double value;
        double derivative;
        // Tape& tape
        Tape* tape;

        //DualNumber(double value, double derivative, Tape& tape);
        DualNumber(double value, double derivative, Tape* tape);
        ~DualNumber(){
            //std::cout << "Destroying Dual Number" << std::endl;
        };

        inline friend DualNumber operator+(const DualNumber& lhs, const DualNumber& rhs);
        inline friend DualNumber operator-(const DualNumber& lhs, const DualNumber& rhs);
        inline friend DualNumber operator*(const DualNumber& lhs, const DualNumber& rhs);
        inline friend DualNumber operator/(const DualNumber& lhs, const DualNumber& rhs);

        inline friend DualNumber operator+(const DualNumber& lhs, const double& rhs);
        inline friend DualNumber operator-(const DualNumber& lhs, const double& rhs);
        inline friend DualNumber operator*(const DualNumber& lhs, const double& rhs);
        inline friend DualNumber operator/(const DualNumber& lhs, const double& rhs);

        inline friend DualNumber operator+(const double& lhs, const DualNumber& rhs);
        inline friend DualNumber operator-(const double& lhs, const DualNumber& rhs);
        inline friend DualNumber operator*(const double& lhs, const DualNumber& rhs);
        inline friend DualNumber operator/(const double& lhs, const DualNumber& rhs);

        inline friend DualNumber operator-(const DualNumber& number);
        inline friend DualNumber log(const DualNumber& X);
        inline friend DualNumber sqrt(const DualNumber& X);
        inline friend DualNumber exp(const DualNumber& X);
        inline friend DualNumber N(DualNumber X);
};

DualNumber BS_Call(DualNumber S, DualNumber K, DualNumber T, DualNumber r, DualNumber sigma);

struct Greeks {
    double delta;
    double vega;
    double theta;
    double rho;
    double gamma;
    double vanna;
};

Greeks computeGreeks(double S_val, double K_val, double T_val, double r_val, double sigma_val);

#endif  // AAD_H