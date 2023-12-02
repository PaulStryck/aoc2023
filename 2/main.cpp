#include <iostream>
#include <fstream>
#include <string>

#include "grammar/Parser.H"

#include "prob_1.h"
#include "prob_2.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (2 != argc)
        return EXIT_FAILURE;

    FILE *input = fopen(argv[1], "r");

    Records *parse_tree = NULL;
    parse_tree = pRecords(input);

    if (parse_tree) {
      prob_1::Validator *a = new prob_1::Validator();
      prob_2::Validator *b = new prob_2::Validator();

      cout << "ids: " << a->validate(parse_tree) << std::endl;

      cout << "power: " << b->validate(parse_tree) << std::endl;
      delete(parse_tree);

      return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
