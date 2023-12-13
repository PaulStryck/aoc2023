#include <fstream>
#include <iostream>

#include "parser.hpp"
#include "datastructures.hpp"

int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;

    {  // Part 1
        std::ifstream infile(argv[1]);
        size_t sum{};

        while(!infile.eof()){
            if(infile.peek() == '\n'){
                infile.ignore('\n');
                continue;
            }

            MirrorMap m = parseMap(infile);
            sum += mirrorAxisVal<0>(m);
        }
        std::cout << "Prob 1: " << sum << std::endl;
    }

    {  // Part 2
        std::ifstream infile(argv[1]);
        size_t sum{};

        while(!infile.eof()){
            if(infile.peek() == '\n'){
                infile.ignore('\n');
                continue;
            }

            MirrorMap m = parseMap(infile);
            sum += mirrorAxisVal<1>(m);
        }

        std::cout << "Prob 2: " << sum << std::endl;
    }

    return EXIT_SUCCESS;
}
