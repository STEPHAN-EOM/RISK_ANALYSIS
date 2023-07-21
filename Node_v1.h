#ifndef NODE_v1_H
#define NODE_v1_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Node {
    protected:
        std::vector<Node*> arguments;

        double result;
        double adjoint = 0.0;

    public:

        Node(){
            std::cout << "Constructing the Node" << std::endl;
        }

        virtual ~Node(){
            std::cout << "Destroying the Node" << std::endl;
        };

      double Get_result(){
            return result;
        }

       double & Get_adjoint(){
            return adjoint;
        }

        void Reset_adjoint(){
            for (auto i : arguments) i -> Reset_adjoint();
            adjoint = 0.0;
        }

        virtual void Propagate_adj() = 0;

};

class AddNode : public Node{
    public:
        AddNode(Node* lhs, Node* rhs){
            arguments.resize(2);
            arguments[0] = lhs;
            arguments[1] = rhs;

            result = lhs->Get_result() + rhs->Get_result();

            std::cout << "Constructing class for Adding Nodes" << std::endl;
        };

        ~AddNode(){
            std::cout << "Destroying class for Adding Nodes" << std::endl;
        };

        void Propagate_adj() override{
            arguments[0] -> Get_adjoint() += adjoint;
            arguments[1] -> Get_adjoint() += adjoint;
        }

};

class MulNode : public Node{
    public:
        MulNode(Node* lhs, Node* rhs){
            arguments.resize(2);
            arguments[0] = lhs;
            arguments[1] = rhs;

            result = lhs->Get_result() * rhs->Get_result();

            std::cout << "Constructing class for time nodes" << std::endl;
        };

        ~MulNode(){
            std::cout << "Destroying class for time nodes" << std::endl;
        };


        void Propagate_adj() override{
            arguments[0] -> Get_adjoint() += adjoint * arguments[1] -> Get_result();
            arguments[1] -> Get_adjoint() += adjoint * arguments[0] -> Get_result();
        }
};

class LogNode : public Node{
    public:
        LogNode(Node* arg){
            arguments.reserve(1);
            arguments[0] = arg;

            result = log(arg->Get_result());

            std::cout << "Constructing class for log nodes" << std::endl;
        };

        ~LogNode(){
            std::cout << "Destroying class for log Nodes" << std::endl;
        };

        void Propagate_adj() override{
                arguments[0] -> Get_adjoint() += adjoint / arguments[0] -> Get_result();
            }
};

class Leaf : public Node{
    public:
        Leaf(double value){
            result = value;
            std::cout << "Constructing class for Leaves" << std::endl;
        };

        ~Leaf(){
            std::cout << "Destroying class for Leaves" << std::endl;
        }

        double Get_value(){
            return result;
        }

        void Set_value(double val){
            result = val;
        }

        void Propagate_adj() override {}
};


#endif // NODE_v1_H