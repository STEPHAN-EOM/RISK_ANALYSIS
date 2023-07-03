#include "AAD.h"

DualNumber::DualNumber(double value, double derivative, double second_derivative)
    : value(value), derivative(derivative), second_derivative(second_derivative){
        std::cout << "Constructing Dual Number" << std::endl;
    };

DualNumber DualNumber::operator+(const DualNumber& other){
    return DualNumber(this-> value + other.value,
                      this-> derivative + other.derivative,
                      this-> second_derivative + other.second_derivative);
}

DualNumber DualNumber::operator-(const DualNumber& other){
    return DualNumber(this-> value - other.value,
                      this-> derivative - other.derivative,
                      this-> second_derivative - other.second_derivative);
}

DualNumber DualNumber::operator*(const DualNumber& other){
    return DualNumber(this-> value * other.value,
                      this-> value * other.derivative + this->derivative * other.value,
                      this-> second_derivative * other.value + 2 * this->derivative * other.derivative + this->value * other.second_derivative);
}

DualNumber DualNumber::operator/(const DualNumber& other){
    return DualNumber(this-> value / other.value,
                      (this-> derivative * other.value - this->value * other.derivative) / (other.value * other.value),
                      (this-> second_derivative * other.value - 2 * this->derivative * other.derivative + this->value * other.second_derivative) / (other.value * other.value));
}

DualNumber N(DualNumber X){
    static const double inv_sqrt_2pi = 0.3989422804014327;
    double value = 0.5 * erfc(-X.value * M_SQRT1_2);
    double derivative = X.derivative * inv_sqrt_2pi * exp(-0.5 * X.value * X.value);
    double second_derivative = X.second_derivative * inv_sqrt_2pi * exp(-0.5 * X.value * X.value)
        - derivative * X.value;

    return DualNumber(value, derivative, second_derivative);
}

DualNumber BS_Call(DualNumber S, DualNumber K, DualNumber T, DualNumber r, DualNumber sigma){
    DualNumber d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    DualNumber d2 = d1 - sigma * sqrt(T);

    return S * N(d1) - K * exp(-r * T) * N(d2);
}

Greeks computeGreeks(double S_val, double K_val, double T_val, double r_val, double sigma_val){
    DualNumber S(S_val, 1.0, 0.0);
    DualNumber K(K_val, 1.0, 0.0);
    DualNumber T(T_val, 1.0, 0.0);
    DualNumber r(r_val, 1.0, 0.0);
    DualNumber sigma(sigma_val, 1.0, 0.0);

    Greeks greeks;
    greeks.delta = BS_Call(S, K, T, r, sigma).derivative;



}
