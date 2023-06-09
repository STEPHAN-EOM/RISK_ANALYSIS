#include "aad.h"

double cal_sensitivity(double r_dom, double r_foreign, double invest){
double sensitivity;
sensitivity = invest * exp((r_dom - r_foreign));

return sensitivity;
}

double cal_ave_sensitivity(const std::vector<double>& sensitivities){

double average = 0.0;

for (double sensitivity : sensitivities){
average += sensitivity;
}

average /= sensitivities.size();

return average;
}
