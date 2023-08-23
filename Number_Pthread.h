#ifndef NUMBER_PTHREAD_H
#define NUMBER_PTHREAD_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <optional>
#include <thread>
#include <mutex>
#include "Node_v2.h"

class Number{
    std::shared_ptr<Node> mynode;

    public:

        static std::vector<std::shared_ptr<Node>> tape;
        thread_local static std::vector<std::shared_ptr<Node>> local_tape;
        static std::mutex tape_mutex;

        Number(double value) : mynode(std::make_shared<Leaf>(value)) {
            tape.push_back(mynode);

            //std::cout << "Constructing class for Number with Leaf" << std::endl;
        };

        Number(const std::shared_ptr<Node>& node) : mynode(node) {
            //std::cout << "Constructing class for Number with Node" << std::endl;
        };

        ~Number(){
            //std::cout << "Destroying class for Numbers" << std::endl;
        };

       std::shared_ptr<Node> node(){
            return mynode;
        }

        void Set_value(double value){
             auto leafNode = std::dynamic_pointer_cast<Leaf>(mynode);
            if (leafNode) {
                leafNode->Set_value(value);
            }
        }

        double Get_value() {
             auto leafNode = std::dynamic_pointer_cast<Leaf>(mynode);
            if (leafNode) {
                return leafNode->Get_value();
            }

            return 0.0;
        }

        double& Get_adjoint(){
            return mynode -> Get_adjoint();
        }

        void Propagate_adj(){
            mynode -> Reset_adjoint();
            mynode -> Get_adjoint() = 1.0;

            auto it = tape.rbegin();
            while (it != tape.rend() && *it != mynode){
                ++it;
            }

            while (it != tape.rend()){
                (*it) -> Propagate_adj();
                ++it;
            }
        }

        void setNode(const std::shared_ptr<Node>& newNode) {
            mynode = newNode;
        }

        static void Mark_tape();
        static void Rewind_Mark();
        static void CombineLocalTape();

    private:
    
        static std::optional<size_t> tapeMark;
        
};

extern std::vector<std::unique_ptr<Node>> tape;
extern thread_local std::vector<std::unique_ptr<Node>> local_tape;
extern std::optional<size_t> tapeMark;

inline Number operator+(Number lhs, Number rhs){
    std::shared_ptr<Node> n = std::make_shared<AddNode>(lhs.node(), rhs.node());
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number operator-(Number lhs, Number rhs){
    std::shared_ptr<Node> n = std::make_shared<SubNode>(lhs.node(), rhs.node());
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number operator*(Number lhs, Number rhs){
    std::shared_ptr<Node> n = std::make_shared<MulNode>(lhs.node(), rhs.node());
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number operator*(Number lhs, double rhs){
    std::shared_ptr<Node> n = std::make_shared<MulDoubleNode>(lhs.node(), rhs);
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number operator/(Number lhs, Number rhs){
    std::shared_ptr<Node> n = std::make_shared<DivNode>(lhs.node(), rhs.node());
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number operator/(Number lhs, double rhs){
    std::shared_ptr<Node> n = std::make_shared<DivDoubleNode>(lhs.node(), rhs);
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number operator-(Number arg){
    std::shared_ptr<Node> n = std::make_shared<MinusNode>(arg.node());
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number log(Number arg){
    std::shared_ptr<Node> n = std::make_shared<LogNode>(arg.node());
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number exp(Number arg){
    std::shared_ptr<Node> n = std::make_shared<ExpNode>(arg.node());
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number sqrt(Number arg){
    std::shared_ptr<Node> n = std::make_shared<SqrtNode>(arg.node());
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number N(Number arg){
    std::shared_ptr<Node> n = std::make_shared<NormalNode>(arg.node());
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number max(Number lhs, Number rhs) {
    std::shared_ptr<Node> n = std::make_shared<MaxNode>(lhs.node(), rhs.node());
    Number::local_tape.push_back(n);

    return Number(n);
}

inline Number& operator+=(Number& lhs, Number rhs) {
    std::shared_ptr<Node> n = std::make_shared<AddNode>(lhs.node(), rhs.node());
    Number::local_tape.push_back(n);
    lhs.setNode(n);

    return lhs;
}

inline Number& operator*=(Number& lhs, Number rhs) {
    std::shared_ptr<Node> n = std::make_shared<MulNode>(lhs.node(), rhs.node());
    Number::local_tape.push_back(n);
    lhs.setNode(n);
    
    return lhs;
}

#endif // NUMBER_PTHREAD_H