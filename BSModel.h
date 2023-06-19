#ifndef BSMODEL_H
#define BSMODEL_H

#include <iostream>
#include <cmath>

class BSModel {
    double spotPrice;
    double strikePrice;
    double riskFreeRate;
    double volatility;
    double timeToMaturity;

public:
    BSModel(double spotPrice, double strikePrice, double riskFreeRate, double volatility, double timeToMaturity);
    ~BSModel(){
        std::cout << "Destroying BSModel" << "\n";
    };

    double calculateCallOptionPrice() const;
    double calculatePutOptionPrice() const;
    
    double calculateDelta() const;
    double calculateGamma() const;
    double calculateTheta() const;
    double calculateVega() const;
    double calculateRho() const;

private:
    double calculateOptionPrice(double d1, double d2) const;
    double normcdf(double x) const;
};

#endif	// BSMODEL_H