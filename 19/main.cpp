#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <chrono>
#include <numeric>

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
  auto [states, parts] = parser.validate(parse_tree);
  delete(parse_tree);

  { // Prob 1
    int sum = 0;
    for(auto p: parts){  // for each part
      prob_1::State s{"in", 0};  // start at initial state

      // transition through states, until end state is reached
      while(true){
        if(s.first == "A"){
          // add the sum of all category values of this part if an accepted state is reached
          sum += std::accumulate(p.begin(), p.end(), 0);
          break;
        }
        if(s.first == "R"){
          // Discard if rejected
          break;
        }

        s = states[s]->eval(p);
      }
    }

    std::cout << "Prob 1: " << sum << std::endl;
  }

  { // Prob 2
    //
    // Push entire Domains through the automaton
    // Start with the Domain [1, 4000]^4.
    // Each transition will split the input domain and assign a new state to each part.
    // By the problem definition, each domain is a 4 dimensional KCell (Hyperrectangle).
    // Each transition will split it accross a Hyperplane into two 4-Cells.

    // 4-Cells to be handled
    std::queue<std::pair<prob_1::KCell<4>, prob_1::State>> q{};

    // start with [1,4000]^4
    q.emplace(std::make_pair(
      prob_1::KCell<4>{std::make_pair(1, 4000), std::make_pair(1, 4000), std::make_pair(1, 4000), std::make_pair(1, 4000)},
      prob_1::State{"in", 0}
    ));

    long long sum{0};
    for (; !q.empty(); q.pop()){
      auto& [c, s] = q.front();
      for(auto x: states[s]->eval(c)){
        if(x.second.first == "A"){
          // compute all points contained in accepted KCell
          sum += std::accumulate(x.first.intervals.begin(), x.first.intervals.end(), 1LL, [](auto d, auto xs){
            return (xs.second - xs.first + 1LL) * d;
          });
          continue;
        }
        if(x.second.first == "R"){
          // reject entire KCell
          continue;
        }

        // push transitioned 4-Cell which has not reached an end state.
        q.push(x);
      }
    }

    std::cout << "Prob 2: " << sum << std::endl;
  }
  return EXIT_SUCCESS;
}
