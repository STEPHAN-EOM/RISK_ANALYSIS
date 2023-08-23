#include "Number_Pthread.h"

std::vector<std::shared_ptr<Node>> Number::tape;
std::mutex Number::tape_mutex; 
std::optional<size_t> Number::tapeMark = std::nullopt;
thread_local std::vector<std::shared_ptr<Node>> Number::local_tape = {}; 

void Number::Mark_tape(){
            tapeMark = tape.size();
}

void Number::Rewind_Mark(){
    if(tapeMark.has_value()){
        //tape.erase(tape.begin() + tapeMark.value(), tape.end());
        tape.resize(tapeMark.value());
        tapeMark.reset();
    }
}

void Number::CombineLocalTape() {
    std::lock_guard<std::mutex> lock(tape_mutex);
    tape.insert(tape.end(),
                std::make_move_iterator(local_tape.begin()),
                std::make_move_iterator(local_tape.end()));
    local_tape.clear();
}