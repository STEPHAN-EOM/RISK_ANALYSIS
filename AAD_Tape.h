#ifndef AAD_TAPE_H
#define AAD_TAPE_H

#include <iostream>
#include <vector>
#include <functional>

class Tape{
    public:
        struct TapeEntry{
            std::function<void ()> f;


            TapeEntry(std::function<void ()> f)
                : f(f){
                    //std::cout << "Constructing the Tape" << std::endl;
                };
        };

        static std::vector <TapeEntry>& getTape(){
            static std::vector<TapeEntry> tape;
            return tape;
        };

        static void clear(){
            getTape().clear();
        }

        static void add(std::function<void ()> f){
            getTape().push_back(TapeEntry(f));
        }

        static void rewind(){
            auto& tape = getTape();
            for(auto it = tape.rbegin(); it != tape.rend(); ++it){
                it -> f();
            }
        }

};

//std::vector<Tape::TapeEntry> Tape::tape;

#endif // AAD_TAPE_H