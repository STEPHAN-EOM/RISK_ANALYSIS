#include "IRModel.h"

IRModel::IRModel(double dom, double foreign) : r_dom(dom), r_foreign(foreign){
        std::cout << "Constructing IR Model with (Domestic IR, Foreign IR) = (" << dom << ", " << foreign << ")" << "\n"; 
}

double IRModel::simulate(double dt, double rand_num){
    double dom_rate, foreign_rate;
    dom_rate = r_dom * dt + sqrt(dt) * rand_num;
    foreign_rate = r_foreign * dt + sqrt(dt) * rand_num;

    return dom_rate - foreign_rate;
}