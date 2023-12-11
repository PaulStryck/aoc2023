#ifndef parser_HEADER
#define parser_HEADER

#include <fstream>
#include <set>
#include <string>
#include <algorithm>

#include "datastructures.hpp"

std::vector<coord_t> parseGalaxies(std::ifstream &in, int expansion){
  // The set of coordinates of known galaxies
  std::vector<coord_t> galaxies;

  // keeping track of all X coordinates where a galaxy was found in the non expanded universe
  // required for later expansion in X direction
  std::set<val_t> cols;

  // current Y position in the expanded universe
  val_t yPos = 0;

  int xs = 0; // maximum columns in the input
  std::string sLineBuff;
  while(std::getline(in, sLineBuff)){
    xs = sLineBuff.length();
    val_t xPos = 0;  // current X position in non expanded universe
    int nGalaxies = 0;  // galaxies found in the current line
    for(char c: sLineBuff){
      if(c == '#'){
        ++nGalaxies;
        cols.insert(xPos);
        galaxies.push_back({xPos, yPos});
      }
      ++xPos;
    }
    ++yPos;

    // expand in Y direction if no galaxies were found on this line
    if(nGalaxies == 0)
      yPos += expansion;
  }

  // Expand in X direction.
  // By sorting the galaxies a long x coordinate,
  // iterating in reverse order and only shift elements which were already visited.
  // This way, the vector remains ordered along x coordinate.
  std::sort(galaxies.begin(), galaxies.end());
  for(int i = xs-1; i >= 0; --i){
    if(!cols.contains(i)){  // if this X coordinate did not contain any galaxies
      // expand all X coordinates of all galaxies to the right of the current X coordinate
      for(auto it = galaxies.rbegin(); it != galaxies.rend(); ++it){
        if(*it < coord_t{i, 0})
          break;

        it->first += expansion;
      }
    }
  }

  return galaxies;
}


#endif
