#include <iostream>
#include "BSModel.h"

int main(){
    double spot_p = 1295.0;
    double strike_p = 1300.0;
    double risk_neutral = 0.05;
    double vol = 0.2;
    double maturity = 1.0; 

    BSModel bsmodel(spot_p, strike_p, risk_neutral, vol, maturity);
    double call_op = bsmodel.calculateCallOptionPrice();
    double put_op = bsmodel.calculatePutOptionPrice();

    std::cout << "\n======== Option Price(S(T0): "<< spot_p << ", K: " << strike_p << ") ========\n";
    std::cout << "Call Option Price: " << call_op << "\n";
    std::cout << "Put Option Price: " << put_op << "\n";

    double delta = bsmodel.calculateDelta();
    double gamma = bsmodel.calculateGamma();
    double theta = bsmodel.calculateTheta();
    double vega = bsmodel.calculateVega();
    double rho = bsmodel.calculateRho();

    std::cout << "\n======== Sensitivities(S(T0): "<< spot_p << ", K:" << strike_p << ") ========\n";
    std::cout << "Delta: " << delta << "\n";
    std::cout << "Gamma: " << gamma << "\n";
    std::cout << "Theta: " << theta << "\n";
    std::cout << "Vega: " << vega << "\n";
    std::cout << "Rho: " << rho << "\n";


    double spot_p1 = 1295.0;
    double strike_p1 = 1290.0;
    double risk_neutral1 = 0.05;
    double vol1 = 0.2;
    double maturity1 = 1.0; 

    BSModel bsmodel1(spot_p1, strike_p1, risk_neutral1, vol1, maturity1);
    double call_op1 = bsmodel1.calculateCallOptionPrice();
    double put_op1 = bsmodel1.calculatePutOptionPrice();

    std::cout << "\n======== Option Price(S(t0): "<< spot_p1 << ", K: " << strike_p1 << ") ========\n";
    std::cout << "Call Option Price: " << call_op1 << "\n";
    std::cout << "Put Option Price: " << put_op1 << "\n";

    double delta1 = bsmodel1.calculateDelta();
    double gamma1 = bsmodel1.calculateGamma();
    double theta1 = bsmodel1.calculateTheta();
    double vega1 = bsmodel1.calculateVega();
    double rho1 = bsmodel1.calculateRho();

    std::cout << "\n======== Sensitivities(S(t0): "<< spot_p1 << ", K: " << strike_p1 << ") ========\n";
    std::cout << "Delta: " << delta1 << "\n";
    std::cout << "Gamma: " << gamma1 << "\n";
    std::cout << "Theta: " << theta1 << "\n";
    std::cout << "Vega: " << vega1 << "\n";
    std::cout << "Rho: " << rho1 << "\n";

    return 0;
}