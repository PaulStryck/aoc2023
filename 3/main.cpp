#include <iostream>
#include <fstream>
#include <string>

#include "utils.h"
#include "prob_1.h"
#include "prob_2.h"

using namespace std;


int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;

  ifstream infile(argv[1]);

  // buffer to hold 3 lines at once
  Ringbuffer<string, 3> r;

  // buffer for a single line
  string l;

  // Result values
  int sum_1 = 0;
  int sum_2 = 0;

  // To check if a number is adjacent to a symbol or vice versa,
  //   a lookahead of one line is required.
  // Thus: read the first line here without checking
  //   after reading the second line, check the first
  //   and take the scond into account
  getline(infile, l);
  r.insert(l);

  while(getline(infile, l)){
    r.insert(l);

    sum_1 += prob_1::countPartNumbers(&r);
    sum_2 += prob_2::countGearRations(&r);
  }

  // after the file is consumed,
  // enter an empty dummy line to check numbers in
  //   the final line from the file
  r.insert("");
  sum_1 += prob_1::countPartNumbers(&r);
  sum_2 += prob_2::countGearRations(&r);


  cout << sum_1 << endl;
  cout << sum_2 << endl;

  return 0;
}
