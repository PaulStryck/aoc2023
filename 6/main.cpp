#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <functional>
#include <regex>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <ranges>
#include <cctype>

#include "parser.h"
#include "datastructures.hpp"

using namespace std;

/*
 * let f(x) = (a-x)x-b
 * find a root of f(x) rounded down to the next integer.
 * Unsing newton-raphson method by using only integer operations.
 *
 * Might save about 3 or 4 cpu clock ticks as opposed to floor(0.5*(a - sqrt(a*a - 4*b)))
 *
 * Convergence if:
 *   - a, b, x0 integers
 *   - a > 0 and b > 0 and a^2/4 - b > 0 (parabola has 2 real positive roots)
 *   - x0 in [0, a/2) (initial guess is between 0 and global maximum)
 */
inline unsigned int newton(unsigned long a, unsigned long b, unsigned long x0) {
  unsigned long xn = x0;
  {
    unsigned long _x;
    while(true) {
      _x = (b-xn*xn) / (a - 2*xn);

      if(_x == xn)
        break;

      xn = _x;
    }
  }

  // fallback to single linesearch step
  // required to catch when the root is too close to an exact integer
  // which is approached from below by the newton iteration.
  if(((a - (xn+1)) * (xn+1) - b) == 0)
    ++xn;

  return xn;
}


/*
 * Possibilites to win are given by the amout of whole integers
 * between the roots of the function f(x) = (a-x)x - b.
 *
 * Let x_1 and x_2 be the real roots of f(x). I.e., f(x_1) = f(x_2) = 0. Where x_1 < x_2.
 * possibilities to win are thus given by: ⌊x_2⌋ - ⌊x_1⌋ - 1.
 *
 * ⌊x_1⌋ is computed by newton-raphson.
 * ⌊x_2⌋ is given by a/2 + dist(a/2, x_1) = a/2 + a/2-x_1 = a - x_1
 * => possibilites_to_win = a - 2*x_1 - 1.
 */
unsigned int possibilites_to_win(unsigned long a, unsigned long b) {
  unsigned int x = newton(a, b, 0);

  return a - 2*x -1;
}


int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;

  ifstream infile(argv[1]);


  string sTime, sRecord;
  getline(infile, sTime);
  getline(infile, sRecord);

  // prob 1
  races_t races = parseRaces1(sTime, sRecord);
  unsigned int res_1 = std::reduce(races.begin(), races.end(), (unsigned int)1, [](unsigned int a, race_t b){
    return a * possibilites_to_win(b.first, b.second);
  });

  // prob 2
  race_t race_2 = parseRaces2(sTime, sRecord);
  unsigned long res_2 = possibilites_to_win(race_2.first, race_2.second);

  cout << "Problem 1: " << res_1 << endl;
  cout << "Problem 2: " << res_2 << endl;

  return 0;
}
