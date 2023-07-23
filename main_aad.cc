#include <iostream>
#include "Number_v1.h"

template <class T>
T f(T x[5]){
    
    auto d1 = (log(x[0] / x[1]) + (x[2] + 0.5 * x[3] * x[3]) * x[4]) / (x[3] * sqrt(x[4])); 
    auto d2 = d1 + (- x[3]) * sqrt(x[4]);
    auto y = x[0] * N(d1) + (- x[1]) * exp((-x[2]) * x[4]) * N(d2);  

    return y;
}

int main(){

    double spot_p = 1295.0;
    double strike_p = 1300.0;
    double r_dom = 0.035;		                // Domestic interest rate (Korea, 8-June-2023)
    double r_foreign = 0.0525; 	                // Foreign interest rate(USA, 8-June-2023)
    double risk_neutral = r_foreign - r_dom;
    double vol = 0.15;
    double maturity = 1.0; 
   
    Number initial[5] = {spot_p, strike_p, risk_neutral, vol, maturity};
    Number BS_Call = f(initial);

    BS_Call.Propagate_adj();


    for (size_t i = 0; i < 5; ++i){
        std::cout << "a" << i << " = " << initial[i].Get_adjoint() << std::endl;
    }
    

    return 0;
}