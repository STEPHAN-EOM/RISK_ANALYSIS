#ifndef AAD_H
#define AAD_H

#include <vector>
#include <cmath>

double cal_sensitivity(double r_dom, double r_foreign, double invest);
double cal_ave_sensitivity(const std::vector<double>& sensitivities);


#endif	// AAD_H
