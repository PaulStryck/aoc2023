#include <iostream>
#include <fstream>
#include <set>
#include "datastructures.hpp"
#include "parser.hpp"


using namespace std;

int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;

  // set is ordered automatically using a strong ordering on Hand<T>
  set<Hand<prob_1::Cards>> hands_1;
  set<Hand<prob_2::Cards>> hands_2;

  // parse each hand
  // difference between prob_1 and prob_2 is a different strong_ordering
  // on the cards
  ifstream infile(argv[1]);
  string line;
  while(getline(infile, line)){
    hands_1.insert(parseHand<prob_1::Cards>(line));
    hands_2.insert(parseHand<prob_2::Cards>(line));
  }

  // the sets is already ordered by the ordering defined on the Hand<T> struct
  {
    // problem 1
    unsigned int rnk = 1;
    unsigned int result = std::transform_reduce(
        hands_1.begin(), hands_1.end(),
        (unsigned int)0, std::plus{},
        [&rnk](const Hand<prob_1::Cards> &h){
          return h.bid * rnk++;
        }
    );
    cout << "prob 1: " << result << endl;
  }

  {
    // problem 2
    unsigned int rnk = 1;
    unsigned int result = std::transform_reduce(
        hands_2.begin(), hands_2.end(),
        (unsigned int)0, std::plus{},
        [&rnk](const Hand<prob_2::Cards> &h){
          return h.bid * rnk++;
        }
    );
    cout << "prob 2: " << result << endl;
  }


  return EXIT_SUCCESS;
}
