#ifndef PROB_1_H
#define PROB_1_H

#include <string>

namespace prob_1 {
void consume_forward(std::string_view str, int* x){
    for(std::string_view::const_iterator it = str.begin(); it != str.end(); ++it){
        if(isdigit(*it)){
            *x += *it;
            break;
        }
    }
}

void consume_backward(std::string_view str, int* x){
    for(std::string_view::const_reverse_iterator it = str.rbegin(); it != str.rend(); ++it){
        if(isdigit(*it)){
            *x += *it;
            break;
        }
    }
}
}
#endif
