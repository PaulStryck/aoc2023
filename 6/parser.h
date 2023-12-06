#ifndef parser_HEADER
#define parser_HEADER

#include <iostream>
#include <string>
#include <functional>

#include "datastructures.hpp"

void parseNumbers(std::string_view s, std::function <void (unsigned int num)> fn) {
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


races_t parseRaces1(std::string_view sTime, std::string_view sRecord){
  races_t races;
  parseNumbers(sTime, [&races](unsigned int num){
      races.push_back(std::pair{num, 0});
  });

  size_t i = 0;
  parseNumbers(sRecord, [&races, &i](unsigned int num){
      races[i++].second = num;
  });

  return races;
}

race_t parseRaces2(std::string sTime, std::string sRecord){
  // remove whitespace and prefix
  std::erase_if(sTime, [](char x){return !std::isdigit(x);});
  std::erase_if(sRecord, [](char x){return !std::isdigit(x);});

  return race_t{std::stoul(sTime.c_str()), std::stoul(sRecord.c_str())};
}

#endif
