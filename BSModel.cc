#include "BSModel.h"


BSModel::BSModel(double spotPrice, double strikePrice, double riskFreeRate, double volatility,
                           double timeToMaturity)
    : spotPrice(spotPrice),
      strikePrice(strikePrice),
      riskFreeRate(riskFreeRate),
      volatility(volatility),
      timeToMaturity(timeToMaturity) {
        std::cout << "\nConstructing BSModel for S(t0): " << spotPrice << " , K: " << strikePrice << std::endl;
      };

double BSModel::calculateFXForward() const {
    //return spotPrice * exp(-riskFreeRate * timeToMaturity) - strikePrice;
    return spotPrice * exp(-riskFreeRate * timeToMaturity);
}

double BSModel::calculateCallOptionPrice() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));
    double d2 = d1 - volatility * std::sqrt(timeToMaturity);

    //std::cout << "Progress: BS_CalCallOp" << std::endl;
    return calculateOptionPrice(d1, d2);
}

double BSModel::calculatePutOptionPrice() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));
    double d2 = d1 - volatility * std::sqrt(timeToMaturity);

    //std::cout << "Progress: BS_CalPutOp" << std::endl;
    return calculateOptionPrice(-d2, -d1);
}

double BSModel::calculateDelta() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));

    //std::cout << "Progress: BS_CalDelta" << std::endl;
    return normcdf(d1);
}

double BSModel::calculateGamma() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));

    double pdf = std::exp(-0.5 * d1 * d1) / std::sqrt(2.0 * M_PI);

    //std::cout << "Progress: BS_CalGamma" << std::endl;
    return pdf / (spotPrice * volatility * std::sqrt(timeToMaturity));
}

double BSModel::calculateTheta() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));
    double d2 = d1 - volatility * std::sqrt(timeToMaturity);

    double term1 = -spotPrice * volatility * std::exp(-riskFreeRate * timeToMaturity) * normcdf(d1) / (2.0 * std::sqrt(timeToMaturity));
    double term2 = riskFreeRate * strikePrice * std::exp(-riskFreeRate * timeToMaturity) * normcdf(d2);

    //std::cout << "Progress: BS_CalTheta" << std::endl;
    return term1 - term2;
}

double BSModel::calculateVega() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));

    double pdf = std::exp(-0.5 * d1 * d1) / std::sqrt(2.0 * M_PI);

    //std::cout << "Progress: BS_CalVega" << std::endl;
    return spotPrice * std::exp(-riskFreeRate * timeToMaturity) * pdf * std::sqrt(timeToMaturity);
}

double BSModel::calculateRho() const {
    double d2 = (std::log(spotPrice / strikePrice) + (riskFreeRate - 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));

    //std::cout << "Progress: BS_CalRho" << std::endl;
    return strikePrice * timeToMaturity * std::exp(-riskFreeRate * timeToMaturity) * normcdf(d2);
}

double BSModel::calculateOptionPrice(double d1, double d2) const {
    double OptionPrice =
        spotPrice * normcdf(d1) - strikePrice * std::exp(-riskFreeRate * timeToMaturity) * normcdf(d2);
    
    //std::cout << "Progress: BS_CalOP" << std::endl;
    return OptionPrice;
}

double BSModel::normcdf(double x) const {
    //std::cout << "Progress: BS_Normcdf" << std::endl;
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}
