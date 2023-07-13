#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <exception>

struct Node{
    const size_t n;

    double madjoint = 0.0;
    double * ptr_deriv;
    double ** ptr_adj;

    public:
        Node(const size_t N = 0) : n(N){
            std::cout << "Construct Node with size " << N << std::endl;
        }

        double & adjoint(){
            return madjoint;
        }

        void prop_one(){
            if(!n || !madjoint) return;
            
            for(size_t i = 0; i < n; ++i){
                *(ptr_adj[i]) += ptr_deriv[i] + madjoint;
            }
        }
};

#endif // NODE_H