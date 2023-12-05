#ifndef parser_HEADER
#define parser_HEADER

#include <string>
#include <iostream>
#include <functional>
#include <regex>
#include <string>
#include <map>

#include "datastructures.hpp"

using namespace std;

void parseNumbers(string_view s, std::function <void (unsigned int num)> fn) {
  // string buffer beeing filled with digits
  std::string f = "";

  // are we currently parsing a number
  bool parsing = false;

  for(auto it = s.begin(); it != s.end(); ++it){
    // loop the entire string
    if(isdigit(*it)){
      // append any digit to the buffer
      f += *it;

      if(!parsing){
        // we've hit the first digit
        parsing = true;
      }
    }

    if(!f.empty() && ( !isdigit(*it) || (std::next(it) == s.end()) ) ){
      // we've hit the end of a number
      // either because digits were parsed before,
      //   but current char is not a digit
      // or digits were parsed before and we've hit
      //   the end of the string

      unsigned int f_ = stoul(f);

      f = "";  // reset string buffer
      parsing = false;

      fn(f_);
    }
  }
}

Function::FnDefInterval parse(string_view s) {
  Function::FnDefInterval f;
  AlmanacEntry r;
  size_t grp = 0;
  parseNumbers(s, [&grp, &r](unsigned int num){
    if(grp == 0) {
      r.destination = num;
      ++grp;
    } else if(grp == 1) {
      r.source = num;
      ++grp;
    } else if(grp == 2) {
      r.rng = num;
      ++grp;
    }
  });

  f.domain = interval::closed(r.source, r.source + r.rng -1);
  if(r.source > r.destination)
    f.offset = -(int)(r.source - r.destination);
  else
    f.offset = (int)(r.destination - r.source);

  return f;
}

vector<unsigned int> parseSeeds(string_view sSeeds) {
  vector<unsigned int> seeds;

  parseNumbers(sSeeds, [&](unsigned int num){
    seeds.push_back(num);
  });

  return seeds;
}

almanac_t parseAlmanacMappings(string sInput){
  // basic regex parser for the Almanac Input
  // Matches each block, capturing source and
  // destination category and the block of numbers.
  // I.e.,
  // destCat-to-sourceCat map:
  // xx xx xx
  // xx xx xx
  regex r("(?:(?:([a-z]+)-to-([a-z]+) map:\\n))((?:\\d+ \\d+ \\d+\\n?)+)");

  std::map<string, std::pair<string, Function>> lookup;

  // iterate the Almanac mapping blocks
  // parse each entry into an AlmanacEntry struct
  auto maps_begin = std::sregex_iterator(sInput.begin(), sInput.end(), r);
  auto maps_end = std::sregex_iterator();
  for (std::sregex_iterator i = maps_begin; i != maps_end; ++i) {
    // match[0]: full match
    // match[1]: source Category
    // match[2]: destination Category
    // match[3]: string of \n seperated groups of 3 integers
    std::smatch match = *i;

    category src  = match[1].str();
    category dest = match[2].str();
    std::istringstream f(match[3].str());

    // memorize where the source needs to be matched to
    lookup[src] = std::pair(dest, Function());

    string l;
    while(getline(f, l)){
      Function::FnDefInterval r = parse(l);
      lookup[src].second.f.insert(r);
    }
  }

  return lookup;
}
#endif
