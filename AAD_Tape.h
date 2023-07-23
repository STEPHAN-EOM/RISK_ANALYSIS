#ifndef AAD_TAPE_H
#define AAD_TAPE_H

#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <functional>

class Tape{
    public:

        Tape(){
            std::cout << "Constructing the Tape" << std::endl;
        };

        ~Tape(){
            std::cout << "Destroying the Tape" << std::endl;
        };

        struct TapeEntry{
            std::function<void ()> f;


            TapeEntry(std::function<void ()> f)
                : f(f){
                    std::cout << "Constructing the Tap Entry" << std::endl;
                };
        };

       
        static constexpr int MaxTapeSize = 50;
        std::array<std::list<TapeEntry>, MaxTapeSize> tape;
        
        
        static int& tapeSize(){
            static int size = 0;
            return size;
        }
        // int tapeSize = 100;

        void clear() {
            std::cout << "Clearing the Tape" << std::endl;
            for (int i = 0; i < tapeSize(); ++i) {
                tape[i].clear();
            }
            tapeSize() = 0;
        }

        void add(std::function<void()> f) {
            std::cout << "Adding the Tape" << std::endl;
            if (tapeSize() < MaxTapeSize) {
                tape[tapeSize()].emplace_back(TapeEntry(f));
                //tape[tapeSize()].push_back(TapeEntry(f)); 
                ++tapeSize();
            } else {
                throw std::runtime_error("Tape size limit exceeded.");
            }
            std::cout << "Current Tape size " << tapeSize() << std::endl;
        }

        void rewind() {
            if (tapeSize() <= 0) {
            throw std::runtime_error("Tape is empty, nothing to rewind.");
            }   
            std::cout << "Rewinding the Tape" << std::endl;
            std::cout << "Tape size = " << tapeSize() << std::endl;
            for (int i = tapeSize() - 1; i >= 0; --i) {
                std::cout << "Tape size which is rewinded -> " << i << std::endl;
                for (auto it = tape[i].rbegin(); it != tape[i].rend(); ++it) {
                    it->f();  
                }
            }
            std::cout << "Rewinding(Itr) is done" << std::endl;
            clear(); 
            std::cout << "Rewinding(Clear) is done" << std::endl;
        }

/*
        std::vector<TapeEntry> tape;

        void clear() {
            std::cout << "Clearing the Tape" << std::endl;
            tape.clear();
        }

        void add(std::function<void()> f) {
            std::cout << "Emplace backing the Tape" << std::endl;
            tape.emplace_back(f);
        }

        void rewind() {
            std::cout << "Rewinding the Tape" << std::endl;
            for (auto it = tape.rbegin(); it != tape.rend(); ++it) {
                it->f();
             }
            tape.clear();
        }

*/
        /*
        static std::vector <TapeEntry>& getTape(){
            static std::vector<TapeEntry> tape;
            return tape;
        };

        void clear(){
            getTape().clear();
        }

        void add(std::function<void ()> f){
            getTape().push_back(TapeEntry(f));
        }

        void rewind(){
            auto& tape = getTape();
            for(auto it = tape.rbegin(); it != tape.rend(); ++it){
                it -> f();
            }
        }

        */

};


#endif // AAD_TAPE_H