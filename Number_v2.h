#ifndef NUMBER_V2_H
#define NUMBER_V2_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <optional>
#include "Node_v1.h"

class Number{
    std::unique_ptr<Node> mynode;
/*
    static void AddToTape(std::unique_ptr<Node> node) {
        tape.push_back(std::move(node));
    }
*/
    public:

        static std::vector<std::unique_ptr<Node>> tape;

        Number(double value) : mynode(std::make_unique<Leaf>(value)) {
            //AddToTape(std::make_unique<Leaf>(value));
            tape.push_back(std::make_unique<Leaf>(value));
        };

        Number(std::unique_ptr<Node> node) : mynode(node.get()) {
            tape.push_back(std::move(node));
             //std::cout << "Constructing class for Number with Node" << std::endl;
        };

        ~Number(){
            //std::cout << "Destroying class for Numbers" << std::endl;
        };

        Node* node(){
            return mynode.get();
        }

        void Set_value(double value){
            dynamic_cast<Leaf*>(mynode.get()) -> Set_value(value);
        }

        double Get_value() {
            return dynamic_cast<Leaf*>(mynode.get()) -> Get_value();
        }

        double& Get_adjoint(){
            return mynode -> Get_adjoint();
        }

        void Propagate_adj(){
            mynode -> Reset_adjoint();
            mynode -> Get_adjoint() = 1.0;

            auto it = tape.rbegin();
            while (it-> get() != mynode.get()){
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
        //Number(std::unique_ptr<Node> node) : mynode(std::move(node)) {};

        static std::optional<size_t> tapeMark;

        //friend Number operator+(Number lhs, Number rhs);
        
};

std::vector<std::unique_ptr<Node>> Number::tape;
std::optional<size_t> Number::tapeMark = std::nullopt;

Number operator+(Number lhs, Number rhs){
    auto n = std::make_unique<AddNode>(lhs.node(), rhs.node());
    return Number(std::move(n));
}

Number operator-(Number lhs, Number rhs){
    auto n = std::make_unique<SubNode>(lhs.node(), rhs.node());
    return Number(std::move(n));
}

Number operator*(Number lhs, Number rhs){
    auto n = std::make_unique<MulNode>(lhs.node(), rhs.node());
    return Number(std::move(n));
}

Number operator*(Number lhs, double rhs){
    auto n = std::make_unique<MulDoubleNode>(lhs.node(), rhs);
    return Number(std::move(n));
}

Number operator/(Number lhs, Number rhs){
    auto n = std::make_unique<DivNode>(lhs.node(), rhs.node());
    return Number(std::move(n));
}

Number operator/(Number lhs, double rhs){
    auto n = std::make_unique<DivDoubleNode>(lhs.node(), rhs);
    return Number(std::move(n));
}

Number operator-(Number arg){
    auto n = std::make_unique<MinusNode>(arg.node());
    return Number(std::move(n));
}

Number log(Number arg){
    auto n = std::make_unique<LogNode>(arg.node());
    return Number(std::move(n));
}

Number exp(Number arg){
    auto n = std::make_unique<ExpNode>(arg.node());
    return Number(std::move(n));
}

Number sqrt(Number arg){
    auto n = std::make_unique<SqrtNode>(arg.node());
    return Number(std::move(n));
}

Number N(Number arg){
    auto n = std::make_unique<NormalNode>(arg.node());
    return Number(std::move(n));
}

Number max(Number lhs, Number rhs) {
    auto n = std::make_unique<MaxNode>(lhs.node(), rhs.node());
    return Number(std::move(n));
}

Number& operator+=(Number& lhs, Number rhs) {
    auto n = std::make_unique<AddNode>(lhs.node(), rhs.node());
    lhs = Number(std::move(n));  // Update the lhs to point to the new node.
    return lhs;
}

Number& operator*=(Number& lhs, Number rhs) {
    auto n = std::make_unique<MulNode>(lhs.node(), rhs.node());
    lhs = Number(std::move(n));  // Update the lhs to point to the new node.
    return lhs;
}

#endif // NUMBER_V2_H