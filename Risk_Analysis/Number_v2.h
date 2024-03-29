#ifndef NUMBER_V2_H
#define NUMBER_V2_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <optional>
#include <mutex>
#include <thread>
#include "Node_v1.h"

class Number{
    Node* mynode;

    public:

        static std::vector<std::unique_ptr<Node>> global_tape;
        thread_local static std::vector<std::unique_ptr<Node>> tape;
        static std::mutex tape_mutex;

        Number(double value) : mynode(new Leaf(value)) {
            std::lock_guard<std::mutex> lock(tape_mutex);
            //tape.push_back(std::unique_ptr<Node>(mynode));
            global_tape.push_back(std::unique_ptr<Node>(mynode));

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

        double Get_result() {
            return dynamic_cast<Leaf*>(mynode) -> Get_result();
        }

        void Set_adjoint(double value) {
            mynode -> Set_adjoint(value);
        }

        double& Get_adjoint(){
            return mynode -> Get_adjoint();
        }

        void Propagate_adj(){
            std::lock_guard<std::mutex> lock(tape_mutex);
            
            //std::cout << "Address of mynode: " << mynode << std::endl;
            //std::cout << "Nodes in global_tape: ";
            /*
            for (auto& node : global_tape) {
                std::cout << node.get() << " ";
            }
            */
            //std::cout << std::endl;

            //std::cout << "Back-Propagation is starting" <<std::endl;
            mynode -> Reset_adjoint();
            mynode -> Get_adjoint() = 1.0;

            //std::cout << "Back-Propagation is working1" <<std::endl;
            auto it = global_tape.rbegin();
            //std::cout << "Back-Propagation is working2" <<std::endl;
            //auto it = tape.rbegin(); 
            while (it-> get() != mynode){
                ++it;
                if (it == global_tape.rend()) {
                    std::cerr << "mynode not found in global_tape. Aborting.\n";
                    return;
                }
            }

            //std::cout << "Back-Propagation is working3" <<std::endl;
            while (it != global_tape.rend()){
            //while (it != tape.rend()){    
                (*it) -> Propagate_adj();
                //std::cout << "Propagated adjoint for node: " << it->get() << ", New adjoint value: " << (*it)->Get_adjoint() << '\n';
                ++it;
            }
           // std::cout << "Back-Propagation is done" <<std::endl;
        }

        static void Mark_tape(){
            tapeMark = global_tape.size();
        }

        static void Rewind_Mark(){
            if(tapeMark.has_value()){
                //tape.erase(tape.begin() + tapeMark.value(), tape.end());
                global_tape.resize(tapeMark.value());
                tapeMark.reset();
            }
        }

    private:
    
        static std::optional<size_t> tapeMark;
        
};

std::vector<std::unique_ptr<Node>> global_tape;
thread_local std::vector<std::unique_ptr<Node>> tape;
std::mutex tape_mutex;
std::optional<size_t> Number::tapeMark = std::nullopt;

Number operator+(Number lhs, Number rhs){
    Node* n = new AddNode(lhs.node(), rhs.node());
    //Number::tape.push_back(std::unique_ptr<Node>(n));
    std::lock_guard<std::mutex> lock(Number::tape_mutex);
    //Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));

    //std::cout << "Added new MulNode to tape" << lhs.Get_result() << " & "<< rhs.Get_result() <<std::endl;

    return n;
}

Number operator-(Number lhs, Number rhs){
    Node* n = new SubNode(lhs.node(), rhs.node());
    //Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
    //Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    return n;
}

Number operator*(Number lhs, Number rhs){
    Node* n = new MulNode(lhs.node(), rhs.node());
   // Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
    //Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    return n;
}

Number operator*(Number lhs, double rhs){
    Node* n = new MulDoubleNode(lhs.node(), rhs);
   // Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
    //Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    //std::cout << "Added new MulNode to tape" << &lhs << " & "<< rhs <<std::endl;

    return n;
}

Number operator/(Number lhs, Number rhs){
    Node*n = new DivNode(lhs.node(), rhs.node());
   // Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
    //Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    return n;
}

Number operator/(Number lhs, double rhs){
    Node*n = new DivDoubleNode(lhs.node(), rhs);
    //Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
   // Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    return n;
}

Number operator-(Number arg){
    Node* n = new MinusNode(arg.node());
   // Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
   // Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    return n;
}

Number log(Number arg){
    Node* n = new LogNode(arg.node());
    //Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
   // Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    return n;
}

Number exp(Number arg){
    Node* n = new ExpNode(arg.node());
   // Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
   // Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    return n;
}

Number sqrt(Number arg){
    Node* n = new SqrtNode(arg.node());
    //Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
   //Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    return n;
}

Number N(Number arg){
    Node* n = new NormalNode(arg.node());
    //Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
   // Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    return n;
}

Number max(Number lhs, Number rhs) {
    Node* n = new MaxNode(lhs.node(), rhs.node());
   // Number::tape.push_back(std::unique_ptr<Node>(n));
 std::lock_guard<std::mutex> lock(Number::tape_mutex);
   // Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    return n;
}

Number operator+=(Number& lhs, Number rhs) {
    Node* n = new AddAssignNode(lhs.node(), rhs.node());
    //Number::tape.push_back(std::unique_ptr<Node>(n));
     std::lock_guard<std::mutex> lock(Number::tape_mutex);
    //Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    lhs = n;
    return lhs;
}

Number operator*=(Number& lhs, Number rhs) {
    Node* n = new MulAssignNode(lhs.node(), rhs.node());
    //Number::tape.push_back(std::unique_ptr<Node>(n));
    std::lock_guard<std::mutex> lock(Number::tape_mutex);
   // Number::tape.push_back(std::unique_ptr<Node>(n));
    Number::global_tape.push_back(std::unique_ptr<Node>(n));
    lhs = n;
    return lhs;
}

#endif // NUMBER_V2_H