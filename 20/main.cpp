#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <chrono>
#include <numeric>

#include "grammar/Absyn.H"
#include "grammar/Parser.H"
#include "parser.h"


using namespace std;

int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;

  FILE *input = fopen(argv[1], "r");

  Records *parse_tree = NULL;
  parse_tree = pRecords(input);

  if(!parse_tree)
    return EXIT_FAILURE;

  prob_1::Validator parser{};
  prob_1::Circuit c = parser.validate(parse_tree);
  int sumLow{0}, sumHigh{0};

  for(int i = 0; i < 1000; ++i){
    std::queue<prob_1::Pulse> pulses;
    pulses.push({"btn", "broadcaster", prob_1::PulseT::LOW});
    for (; !pulses.empty(); pulses.pop()){
      auto& p = pulses.front();
      shared_ptr<prob_1::Gate> currG = c[p.dest];

      if(p.p == prob_1::PulseT::LOW)
        ++sumLow;
      else
        ++sumHigh;

      // For part 2, its a bit hacky:
      // Result is given by the least common multiple of the only four paths to reach the rx gate.
      // rx, bs, zb, js
      // Turn outer for loop to infinite loop, execute program 4, compute lcm with wolfram alpha.
      //
      // if(p.dest == "rx" && p.p == prob_1::PulseT::LOW){
      //   std::cout << i << std::endl;
      //   return EXIT_SUCCESS;
      // }

      if(currG == nullptr){
        continue;
      }

      for(auto _p: currG->recv({p.src, p.p})){
        pulses.push(_p);
      }
    }
  }

  delete(parse_tree);
  std::cout << sumLow << " * " << sumHigh << "=" << sumLow * sumHigh<< std::endl;

  return EXIT_SUCCESS;
}
