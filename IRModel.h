#ifndef IRMODEL_H
#define IRMODEL_H

#include <iostream>

struct IRModel{

    double r_dom;                           // Domestic interest rate (Korea, 8-June-2023)
    double r_foreign;                        // Foreign interest rate (US, 8-June-2023)

    IRModel(double dom, double foreign);
    ~IRModel(){
        std::cout << "Destroying IR Model" << std::endl;
    };

    double simulate(double dt, double rand_num);

};


#endif	// IRMODEL_H