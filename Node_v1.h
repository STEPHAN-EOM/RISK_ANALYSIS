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
            std::cout << lhs->Get_result() << " + " << rhs->Get_result() << " = " << result << std::endl;
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

            std::cout << "Constructing class for Multiplying Nodes" << std::endl;
            std::cout << lhs->Get_result() << " * " << rhs->Get_result() << " = " << result << std::endl;
        };

        ~MulNode(){
            std::cout << "Destroying class for Multiplying Nodes" << std::endl;
        };


        void Propagate_adj() override{
            arguments[0] -> Get_adjoint() += adjoint * (arguments[1] -> Get_result());
            arguments[1] -> Get_adjoint() += adjoint * (arguments[0] -> Get_result());
        }
};

class DivNode : public Node{
    public:
        DivNode(Node* lhs, Node* rhs){
            arguments.resize(2);
            arguments[0] = lhs;
            arguments[1] = rhs;

            result = lhs->Get_result() / rhs->Get_result();

            std::cout << "Constructing class for Dividing Nodes" << std::endl;
            std::cout << lhs->Get_result() << " / " << rhs->Get_result() << " = " << result << std::endl;
        };

        ~DivNode(){
            std::cout << "Destroying class for Dividing Nodes" << std::endl;
        };


        void Propagate_adj() override{
            arguments[0] -> Get_adjoint() += adjoint / (arguments[1] -> Get_result());
            arguments[1] -> Get_adjoint() += (-1)* (adjoint * (arguments[0] -> Get_result())) / ((arguments[1] -> Get_result()) * (arguments[1] -> Get_result()));
        }
};

class MinusNode : public Node{
    public:
        MinusNode(Node* arg){
            arguments.resize(1);
            arguments[0] = arg;

            result = (-1) * (arg->Get_result());

            std::cout << "Constructing class for Minus Nodes" << std::endl;
            std::cout << arg->Get_result() << " * 1 = " << result << std::endl;
        };

        ~MinusNode(){
            std::cout << "Destroying class for Minus Nodes" << std::endl;
        };


        void Propagate_adj() override{
            arguments[0] -> Get_adjoint() += (-1) * adjoint;  
        }
};


class LogNode : public Node{
    public:
        LogNode(Node* arg){
            arguments.resize(1);
            arguments[0] = arg;

            result = log(arg->Get_result());

            std::cout << "Constructing class for Log Nodes" << std::endl;
            std::cout << "log(" <<arg->Get_result() << ") =  " << result << std::endl;
        };

        ~LogNode(){
            std::cout << "Destroying class for Log Nodes" << std::endl;
        };

        void Propagate_adj() override{
                arguments[0] -> Get_adjoint() += adjoint / (arguments[0] -> Get_result());
            }
};

class ExpNode : public Node{
    public:
        ExpNode(Node* arg){
            arguments.resize(1);
            arguments[0] = arg;

            result = exp(arg->Get_result());

            std::cout << "Constructing class for Exp Nodes" << std::endl;
            std::cout << "exp(" <<arg->Get_result() << ") =  " << result << std::endl;
        };

        ~ExpNode(){
            std::cout << "Destroying class for Exp Nodes" << std::endl;
        }

        void Propagate_adj() override{
            arguments[0] -> Get_adjoint() += adjoint * exp(arguments[0] -> Get_result());
        }
};

class SqrtNode : public Node{
     public:
        SqrtNode(Node* arg){
            arguments.resize(1);
            arguments[0] = arg;

            result = sqrt(arg->Get_result());

            std::cout << "Constructing class for Sqrt Nodes" << std::endl;
            std::cout << "sqrt(" <<arg->Get_result() << ") =  " << result << std::endl;
        };

        ~SqrtNode(){
            std::cout << "Destroying class for Sqrt Nodes" << std::endl;
        }

        void Propagate_adj() override{
            arguments[0] -> Get_adjoint() += (1/2) * adjoint / sqrt(arguments[0] -> Get_result());
        }

};

class NormalNode : public Node{
     public:
        NormalNode(Node* arg){
            arguments.resize(1);
            arguments[0] = arg;
            
            result = 0.5 * erfc(-1 * arg->Get_result() * M_SQRT1_2);

            std::cout << "Constructing class for Normal Nodes" << std::endl;
            std::cout << "Normal(" <<arg->Get_result() << ") =  " << result << std::endl;
        };

        ~NormalNode(){
            std::cout << "Destroying class for Normal Nodes" << std::endl;
        }


        void Propagate_adj() override{
            static const double inv_sqrt_2pi = 0.3989422804014327;
            arguments[0] -> Get_adjoint() += adjoint * inv_sqrt_2pi * exp(-0.5 * result * result);
        }

};

class Leaf : public Node{
    public:
        Leaf(double value){
            result = value;
            std::cout << "Constructing class for Leaves" << std::endl;
            std::cout << "Leaf = " << result << std::endl;
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