#include <iostream>
#include <fstream>
#include <string>

#include "prob_1.h"
#include "prob_2.h"

using namespace std;

/**
 *   sum_0^n ((x_i-48)*10 + (y_1 - 48))
 * = sum_0^n (x_i - 480) + sum_0^n (y_1 - 48)
 * = sum_0^n x_i + sum_0^n y_i - n*528
 *
 */
int main(int argc, char* argv[]) {
    if (2 != argc)
        return EXIT_FAILURE;

    ifstream infile(argv[1]);

    std::string line;
    int p_1[] = {0, 0};
    int p_2[] = {0, 0};
    int n = 0;

    while(getline(infile, line)){
        prob_1::consume_forward( string_view(line), &p_1[0]);
        prob_1::consume_backward(string_view(line), &p_1[1]);
        n++;

        prob_2::consume_forward( string_view(line), &p_2[0]);
        prob_2::consume_backward(string_view(line), &p_2[1]);
    }

    cout << p_1[0] * 10 + p_1[1] - n*528 << endl;
    cout << p_2[0] * 10 + p_2[1] << endl;

    return 0;
}
