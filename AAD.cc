#include "AAD.h"

DualNumber::DualNumber(double value, double derivative, Tape* tape)
    : value(value), derivative(derivative), tape(tape) {
        std::cout << "Constructing Dual Number" << std::endl;
    };

inline DualNumber operator+(const DualNumber& lhs, const DualNumber& rhs){
    DualNumber result(lhs.value + rhs.value, lhs.derivative + rhs.derivative, lhs.tape);
    result.tape->add([&result, &lhs, &rhs](){
        result.derivative += (lhs.derivative + rhs.derivative);
    });

    return result;
}

inline DualNumber operator-(const DualNumber& lhs, const DualNumber& rhs){
    DualNumber result(lhs.value - rhs.value, lhs.derivative - rhs.derivative, lhs.tape);
    result.tape->add([&result, &lhs, &rhs](){
        result.derivative += (lhs.derivative - rhs.derivative);
    });

    return result;
}

inline DualNumber operator*(const DualNumber& lhs, const DualNumber& rhs){
    DualNumber result(lhs.value * rhs.value, lhs.value * rhs.derivative + lhs.derivative * rhs.value, lhs.tape);
    result.tape->add([&result, &lhs, &rhs](){
        result.derivative += (rhs.value * result.derivative + lhs.value * result.derivative);
    });

    return result;
}

inline DualNumber operator/(const DualNumber& lhs, const DualNumber& rhs){
    if (rhs.value == 0.0) throw std::runtime_error("Division by zero");
    DualNumber result(lhs.value / rhs.value, 
                      (lhs.derivative * rhs.value - lhs.value * rhs.derivative) / (rhs.value * rhs.value), lhs.tape);
    result.tape->add([&result, &lhs, &rhs]() {
        result.derivative += ((result.derivative / rhs.value) - (lhs.value * result.derivative / (rhs.value * rhs.value)));
    });

    return result;
}

inline DualNumber operator+(const DualNumber& lhs, const double& rhs){
    DualNumber result(lhs.value + rhs, lhs.derivative, lhs.tape);
    result.tape->add([&result, &lhs](){
        result.derivative += lhs.derivative;
    });

    return result;
}

inline DualNumber operator-(const DualNumber& lhs, const double& rhs){
    DualNumber result(lhs.value - rhs, lhs.derivative, lhs.tape);
    result.tape->add([&result, &lhs](){
        result.derivative += lhs.derivative;
    });

    return result;
}

inline DualNumber operator*(const DualNumber& lhs, const double& rhs){
    DualNumber result(lhs.value * rhs, lhs.derivative * rhs, lhs.tape);
    result.tape->add([&result, &lhs, rhs](){
        result.derivative += lhs.derivative * rhs;
    });

    return result;
}

inline DualNumber operator/(const DualNumber& lhs, const double& rhs){
    if (rhs == 0.0) throw std::runtime_error("Division by zero");
    DualNumber result(lhs.value / rhs, lhs.derivative / rhs, lhs.tape);
    result.tape->add([&result, &lhs, rhs]() {
        result.derivative += lhs.derivative / rhs;
    });

    return result;    
}

inline DualNumber operator+(const double& lhs, const DualNumber& rhs){
    DualNumber result(lhs + rhs.value, rhs.derivative, rhs.tape);
    result.tape->add([&result, &rhs](){
        result.derivative += rhs.derivative;
    });

    return result;
}

inline DualNumber operator-(const double& lhs, const DualNumber& rhs){
    DualNumber result(lhs -rhs.value, -rhs.derivative, rhs.tape);
    result.tape->add([&result, &rhs](){
        result.derivative -= rhs.derivative;
    });

    return result;
}

inline DualNumber operator*(const double& lhs, const DualNumber& rhs){
    DualNumber result(lhs * rhs.value, lhs * rhs.derivative, rhs.tape);
    result.tape->add([&result, &rhs, lhs](){
        result.derivative += lhs * rhs.derivative;
    });

    return result;
}

inline DualNumber operator/(const double& lhs, const DualNumber& rhs){
    if (rhs.value == 0.0) throw std::runtime_error("Division by zero");
    DualNumber result(lhs / rhs.value, -lhs * rhs.derivative / (rhs.value * rhs.value), rhs.tape);
    result.tape->add([&result, &rhs, lhs]() {
        result.derivative -= -lhs * rhs.derivative / rhs.value;
    });

    return result;

}

inline DualNumber operator-(const DualNumber& number) {
    return DualNumber(-number.value, -number.derivative, number.tape);
}

inline DualNumber log(const DualNumber& X){
    if (X.value <= 0.0) throw std::runtime_error("log domain error");
    DualNumber result(log(X.value), 1.0 / X.value, X.tape);
    result.tape->add([&result, &X]() {
        result.derivative += X.derivative / X.value;
    });

    return result;
}

inline DualNumber sqrt(const DualNumber& X){
    if (X.value < 0) throw std::runtime_error("sqrt domain error");
    double sqrt_value = sqrt(X.value);
    DualNumber result(sqrt_value, 0.5 / sqrt_value * X.derivative, X.tape);
    result.tape->add([&result, &X, sqrt_value]() {
        result.derivative += 0.5 * X.derivative / sqrt_value;
    });

    return result;
}

inline DualNumber exp(const DualNumber& X){
    double exp_value = exp(X.value);
    DualNumber result(exp_value, exp_value * X.derivative, X.tape);
    result.tape->add([&result, &X, exp_value]() {
        result.derivative += X.derivative * exp_value;
    });

    return result;
}

inline DualNumber N(DualNumber X){
    static const double inv_sqrt_2pi = 0.3989422804014327;
    double value = 0.5 * erfc(-X.value * M_SQRT1_2);
    double derivative = X.derivative * inv_sqrt_2pi * exp(-0.5 * X.value * X.value);
    DualNumber result(value, derivative, X.tape);

    result.tape->add([&]() {
        result.derivative += derivative;
    });

    return result;
}

DualNumber BS_Call(DualNumber S, DualNumber K, DualNumber T, DualNumber r, DualNumber sigma){
    DualNumber d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    std::cout << "Calculate d1" << std::endl;
    DualNumber d2 = d1 - sigma * sqrt(T);
    std::cout << "Calculate d2" << std::endl;
    DualNumber y = S * N(d1) - K * exp(-r * T) * N(d2);
    std::cout << "Calculate y" << std::endl;
    y.tape->add([&]() {
        y.derivative += d1.derivative - d2.derivative;
    });

    std::cout << "Finished BS Call" << std::endl;
    return y;
}

Greeks computeGreeks(double S_val, double K_val, double T_val, double r_val, double sigma_val, Tape tape){

    std::cout << "Start Computing Greeks" << std::endl; 
    //Tape tape;
    std::cout << "Declared the Tape for Computing Greeks" << std::endl; 

    DualNumber S(S_val, 1.0, &tape);
    DualNumber K(K_val, 0.0, &tape);
    DualNumber T(T_val, 0.0, &tape);
    DualNumber r(r_val, 0.0, &tape);
    DualNumber sigma(sigma_val, 0.0, &tape);
    
    std::cout << "Declared the Initial Greeks" << std::endl; 

    Greeks greeks;
    
    std::cout << "Prepare to computing" << std::endl;
    DualNumber BS_Call_result = BS_Call(S, K, T, r,sigma);

    std::cout << "Finish computing" << std::endl;

    // Delta
    S.derivative = 1.0;
    BS_Call_result.derivative = 0.0;
    std::cout << "Delta Step.2" << std::endl;
    std::cout << "Dealta Tape Rewinding" << std::endl;
    tape.rewind();
    std::cout << "Dealta Tape is rewinded" << std::endl;
    greeks.delta = BS_Call_result.derivative;
    std::cout << "Calculate delta" << std::endl;

    // Vega
    sigma.derivative = 1.0;
    BS_Call_result.derivative = 0.0;
    tape.rewind();
    greeks.vega = BS_Call_result.derivative;
    std::cout << "Calculate vage" << std::endl;

    // Rho
    r.derivative = 1.0;
    BS_Call_result.derivative = 0.0;
    tape.rewind();
    greeks.rho = BS_Call_result.derivative;
    std::cout << "Calculate rho" << std::endl;

    // Theta
    T.derivative = 1.0;
    BS_Call_result.derivative = 0.0;
    tape.rewind();
    greeks.theta = -BS_Call_result.derivative;
    std::cout << "Calculate theta" << std::endl;

    // Gamma
    S.derivative = 1.0;
    BS_Call_result.derivative = 0.0;
    tape.rewind();
    greeks.gamma = BS_Call_result.derivative;
    std::cout << "Calculate gamma" << std::endl;

    // vanna
    S.derivative = 1.0;
    sigma.derivative = 1.0;
    BS_Call_result.derivative = 0.0;
    tape.rewind();
    greeks.vanna = BS_Call_result.derivative;
    std::cout << "Calculate vanna" << std::endl;

    return greeks;
}
