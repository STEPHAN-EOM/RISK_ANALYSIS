#ifndef NUMBER_V2_H
#define NUMBER_V2_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <optional>
#include "Node_v2.h"

class Number{
    Node* mynode;

    public:

        static std::vector<std::unique_ptr<Node>> tape;

        Number(double value) : mynode(new Leaf(value)) {
            tape.push_back(std::unique_ptr<Node>(mynode));

            //std::cout << "Constructing class for Number with Leaf" << std::endl;
        };

        Number(Node* node) : mynode(node) {
            //std::cout << "Constructing class for Number with Node" << std::endl;
        };

        ~Number(){
            //std::cout << "Destroying class for Numbers" << std::endl;
        };

        Node* node(){
            return mynode;
        }

        void Set_value(double value){
            dynamic_cast<Leaf*>(mynode) -> Set_value(value);
        }

        double Get_value() {
            return dynamic_cast<Leaf*>(mynode) -> Get_value();
        }

        double& Get_adjoint(){
            return mynode -> Get_adjoint();
        }

        void Propagate_adj(){
            mynode -> Reset_adjoint();
            mynode -> Get_adjoint() = 1.0;

            auto it = tape.rbegin();
            while (it-> get() != mynode){
                ++it;
            }

            while (it != tape.rend()){
                (*it) -> Propagate_adj();
                ++it;
            }
        }

        static void Mark_tape(){
            tapeMark = tape.size();
        }

        static void Rewind_Mark(){
            if(tapeMark.has_value()){
                //tape.erase(tape.begin() + tapeMark.value(), tape.end());
                tape.resize(tapeMark.value());
                tapeMark.reset();
            }
        }

    private:
    
        static std::optional<size_t> tapeMark;
        
};

std::vector<std::unique_ptr<Node>> Number::tape;
std::optional<size_t> Number::tapeMark = std::nullopt;

Number operator+(Number lhs, Number rhs){
    Node* n = new AddNode(lhs.node(), rhs.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number operator-(Number lhs, Number rhs){
    Node* n = new SubNode(lhs.node(), rhs.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number operator*(Number lhs, Number rhs){
    Node* n = new MulNode(lhs.node(), rhs.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number operator*(Number lhs, double rhs){
    Node* n = new MulDoubleNode(lhs.node(), rhs);
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number operator/(Number lhs, Number rhs){
    Node*n = new DivNode(lhs.node(), rhs.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number operator/(Number lhs, double rhs){
    Node*n = new DivDoubleNode(lhs.node(), rhs);
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number operator-(Number arg){
    Node* n = new MinusNode(arg.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number log(Number arg){
    Node* n = new LogNode(arg.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number exp(Number arg){
    Node* n = new ExpNode(arg.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number sqrt(Number arg){
    Node* n = new SqrtNode(arg.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number N(Number arg){
    Node* n = new NormalNode(arg.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number max(Number lhs, Number rhs) {
    Node* n = new MaxNode(lhs.node(), rhs.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));

    return n;
}

Number& operator+=(Number& lhs, Number rhs) {
    Node* n = new AddNode(lhs.node(), rhs.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));
    lhs = n;
    return lhs;
}

Number& operator*=(Number& lhs, Number rhs) {
    Node* n = new MulNode(lhs.node(), rhs.node());
    Number::tape.push_back(std::unique_ptr<Node>(n));
    lhs = n;
    return lhs;
}

#endif // NUMBER_V2_H