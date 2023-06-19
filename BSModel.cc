#include "BSModel.h"


BSModel::BSModel(double spotPrice, double strikePrice, double riskFreeRate, double volatility,
                           double timeToMaturity)
    : spotPrice(spotPrice),
      strikePrice(strikePrice),
      riskFreeRate(riskFreeRate),
      volatility(volatility),
      timeToMaturity(timeToMaturity) {
        std::cout << "Constructing BSModel for S(t0): " << spotPrice << " , K: " << strikePrice << std::endl;
      };

double BSModel::calculateCallOptionPrice() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));
    double d2 = d1 - volatility * std::sqrt(timeToMaturity);

    return calculateOptionPrice(d1, d2);
}

double BSModel::calculatePutOptionPrice() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));
    double d2 = d1 - volatility * std::sqrt(timeToMaturity);

    return calculateOptionPrice(-d2, -d1);
}

double BSModel::calculateDelta() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));

    return normcdf(d1);
}

double BSModel::calculateGamma() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));

    double pdf = std::exp(-0.5 * d1 * d1) / std::sqrt(2.0 * M_PI);

    return pdf / (spotPrice * volatility * std::sqrt(timeToMaturity));
}

double BSModel::calculateTheta() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));
    double d2 = d1 - volatility * std::sqrt(timeToMaturity);

    double term1 = -spotPrice * volatility * std::exp(-riskFreeRate * timeToMaturity) * normcdf(d1) / (2.0 * std::sqrt(timeToMaturity));
    double term2 = riskFreeRate * strikePrice * std::exp(-riskFreeRate * timeToMaturity) * normcdf(d2);

    return term1 - term2;
}

double BSModel::calculateVega() const {
    double d1 = (std::log(spotPrice / strikePrice) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));

    double pdf = std::exp(-0.5 * d1 * d1) / std::sqrt(2.0 * M_PI);

    return spotPrice * std::exp(-riskFreeRate * timeToMaturity) * pdf * std::sqrt(timeToMaturity);
}

double BSModel::calculateRho() const {
    double d2 = (std::log(spotPrice / strikePrice) + (riskFreeRate - 0.5 * volatility * volatility) * timeToMaturity) /
                (volatility * std::sqrt(timeToMaturity));

    return strikePrice * timeToMaturity * std::exp(-riskFreeRate * timeToMaturity) * normcdf(d2);
}

double BSModel::calculateOptionPrice(double d1, double d2) const {
    double callOptionPrice =
        spotPrice * std::exp(-riskFreeRate * timeToMaturity) * normcdf(d1) - strikePrice * std::exp(-riskFreeRate * timeToMaturity) * normcdf(d2);
    return callOptionPrice;
}

double BSModel::normcdf(double x) const {
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}
