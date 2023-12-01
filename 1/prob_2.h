#ifndef PROB_2_H
#define PROB_2_H

#include <string>
#include <vector>

using namespace std;

namespace prob_2 {

vector<string> digit_literals = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

void consume_forward(string_view str, int* x) {
    vector<string::const_iterator> its= {
        digit_literals[0].cbegin(),
        digit_literals[1].cbegin(),
        digit_literals[2].cbegin(),
        digit_literals[3].cbegin(),
        digit_literals[4].cbegin(),
        digit_literals[5].cbegin(),
        digit_literals[6].cbegin(),
        digit_literals[7].cbegin(),
        digit_literals[8].cbegin(),
    };

    for(string_view::const_iterator it = str.cbegin(); it != str.cend(); ++it){
        if(isdigit(*it)){
            *x += (*it) - '0';
            return;
        }
        for(int i = 0; i < 9; ++i){
            if(*it == *(its[i])){
                ++its[i];
                if(its[i] == digit_literals[i].cend()) {
                    *x += i + 1;
                    return;
                }
            }
        }
    }
}

void consume_backward(string_view str, int* x) {
    vector<string::const_reverse_iterator> its = {
        digit_literals[0].crbegin(),
        digit_literals[1].crbegin(),
        digit_literals[2].crbegin(),
        digit_literals[3].crbegin(),
        digit_literals[4].crbegin(),
        digit_literals[5].crbegin(),
        digit_literals[6].crbegin(),
        digit_literals[7].crbegin(),
        digit_literals[8].crbegin(),
    };

    for(string_view::const_reverse_iterator it = str.crbegin(); it != str.crend(); ++it){
        if(isdigit(*it)){
            *x += (*it) - '0';
            return;
        }

        for(int i = 0; i < 9; ++i){
            if(*it == *(its[i])){
                ++its[i];
                if(its[i] == digit_literals[i].crend()) {
                    *x += i + 1;
                    return;
                }
            }
        }
    }
}
}
#endif
