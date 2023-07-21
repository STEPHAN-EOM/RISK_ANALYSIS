#include <iostream>
#include "Number_v1.h"

template <class T>
T f(T x[5]){
    auto y1 = x[2] * (5.0 * x[0] + x[1]);
    auto y2 = log(y1);
    auto y = (y1 + x[3] * y2) * (y1 + y2);

    return y;
}

/*
Number d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
Number d2 = d1 - sigma * sqrt(T);
Number y = S * N(d1) - K * exp(-r * T) * N(d2);
*/

int main(){

    double spot_p = 1295.0;
    double strike_p = 1300.0;
    double r_dom = 0.035;		                // Domestic interest rate (Korea, 8-June-2023)
    double r_foreign = 0.0525; 	                // Foreign interest rate(USA, 8-June-2023)
    double risk_neutral = r_foreign - r_dom;
    double vol = 0.15;
    double maturity = 1.0; 

    Number x[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    Number y = f(x);

    y.Propagate_adj();

    for (size_t i = 0; i < 5; ++i){
        std::cout << "a" << i << " = " << x[i].Get_adjoint() << std::endl;
    }

    Number initial[5] = {spot_p, strike_p, risk_neutral, vol, maturity};
    Number BS_Call = f(initial);

    BS_Call.Propagate_adj();
    


    return 0;
}