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

#include "datastructures.hpp"
#include "parser.hpp"


using namespace std;

/*
 * Recursively evaluate the function chain of the almanac
 * for a single input value n.
 * Starting with initial value n from category k.
 */
unsigned int doLookup(almanac_t a, string k, unsigned int n){
  Function f = a[k].second;
  unsigned int val = f(n);

  if(!a[k].first.empty())
    return doLookup(a, a[k].first, val);

  return val;
}


/*
 * Recursively evaluate the function chain of the almanac
 * for an input domain d. starting with category k.
 */
domain_t doLookup(almanac_t a, string k, domain_t d){
  Function f = a[k].second;
  domain_t val = f(d);

  if(!a[k].first.empty())
    return doLookup(a, a[k].first, val);

  return val;
}


int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;

  ifstream infile(argv[1]);

  almanac_t almanac;

  // read first line and parse seed numbers into vector
  // Contains seed info
  // I.e.,
  //   Seeds: xx xxx xx xxx
  string sSeeds;
  getline(infile, sSeeds);
  vector<unsigned int> seeds = parseSeeds(sSeeds);

  // read remaining input into string
  std::stringstream sBuf;
  sBuf << infile.rdbuf();
  // parse almanac string into evaluatable functions
  almanac = parseAlmanacMappings(sBuf.str());

  unsigned int location = 0;

  // Build the evaluation domain for problem 2
  // For pairs of 2 seed numbers a and b, an interval [a, a+b-1]
  // must be considered.
  domain_t d;
  for(auto it = seeds.begin(); it != seeds.end(); ++it){
    d.insert(interval::closed(*it, *it + *next(it++) -1));
  }

  // find the minimum by evaluating all possible seeds
  location = std::transform_reduce(
      seeds.begin(), seeds.end(),
      numeric_limits<unsigned int>::max(),
      [](const auto& x, const auto& y) { return min(x, y); },
      [&almanac](unsigned int num){
        // recursively traverse the almanac chain
        unsigned int res = doLookup(almanac, "seed", num);
        return res;
      }
  );

  cout << "prob 1: " << location << endl;

  // find the minimum by evaluating an input domain
  d = doLookup(almanac, "seed", d);

  // d contains an ordered set if disjoint intervals
  // the first element will also be the lowest.
  cout << "prob 2: " << boost::icl::first(d) << endl;

  return EXIT_SUCCESS;
}
