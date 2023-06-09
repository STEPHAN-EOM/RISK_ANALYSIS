#ifndef MC_SIMUL_H
#define MC_SIMUL_H


#include <cmath>
#include <random>
#include <vector>

double generate_Scenario(double init, double drift, double vol, double time, double rv);
double calculate_Portfolio(double fx, double int_dom, double int_foreign, double invest);




#endif	// MC_SIMUL_H
