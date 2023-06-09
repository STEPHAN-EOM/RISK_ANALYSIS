#include "mc_simul.h"

double generate_Scenario(double init, double drift, double vol, double time, double rv){

double value;
value = init * exp((drift - 0.5 * pow(vol, 2)) * time + vol * sqrt(time) * rv);

return value;
}

double calculate_Portfolio(double fx, double r_dom, double r_foreign, double invest){

double portfolio;
portfolio = fx * invest * exp((r_dom - r_foreign));

return portfolio;
}

