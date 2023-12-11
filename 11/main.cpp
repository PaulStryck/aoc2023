#include <iostream>
#include <fstream>

#include "datastructures.hpp"
#include "parser.hpp"


using namespace std;

/*
 * Compute the sum of the distances between all n choose 2 combinations in galaxies
 * Uses the Manhatten Distance
 */
long long distances(const vector<coord_t> &galaxies){
  long long sum = 0;
  // compute all n choose 2 combinations using a nested for loop
  for(auto itFirst = galaxies.begin(); itFirst != galaxies.end(); ++itFirst){
    for(auto itSecond = next(itFirst); itSecond != galaxies.end(); ++itSecond){
      // compute manhatten distance between two galaxies.
      sum += abs(itSecond->first - itFirst->first) + abs(itSecond->second - itFirst->second);
    }
  }
  return sum;
}


int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;


  // Parse Galaxy Image and expand immediately.
  // Parser consumes input stream and only requires one line at a time.
  // Expansion in Y direction is done during parsing
  // Expansion in X direction is done after, as it requires all lines to be read.
  { // Problem 1
    ifstream infile(argv[1]);
    vector<coord_t> galaxies = parseGalaxies(infile, 1);
    cout << "Problem 1: " << distances(galaxies) << endl;
  }

  { // Problem 2
    ifstream infile(argv[1]);
    vector<coord_t> galaxies = parseGalaxies(infile, 999999);
    cout << "Problem 2: " << distances(galaxies) << endl;
  }

  return 0;
}
