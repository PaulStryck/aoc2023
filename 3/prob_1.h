#ifndef P1_HEADER
#define P1_HEADER
#include <string>
#include <iostream>
#include "utils.h"

namespace prob_1 {

/*
 * check if the substring s[from:to] is adjacent to or contains a symbol
 * Examples:
 *   substrAdjacentToSymbol("*..", 1, 1)   -> true
 *   substrAdjacentToSymbol("..*..", 0, 4) -> true
 *   substrAdjacentToSymbol("*..", 2, 2)   -> false
 */
bool substrAdjacentToSymbol(std::string_view s, size_t from, size_t to){
  if(s.empty()) return false;

  if(from > 0) --from;
  if(to   < (s.length()-1)) ++to;

  std::string_view subStr = s.substr(from, to - from + 1);
  for(auto it = subStr.begin(); it != subStr.end(); ++it){
    if((*it) != '.' && !isdigit(*it)){
      return true;
    }
  }

  return false;
}

/*
 * Check if a number contained in the string in the middle
 * of the Ringbuffer is a part Number. I.e., adjacent to a symbol
 * @returns sum of all part numbers
 */
int countPartNumbers(Ringbuffer<std::string, 3> *r){
  // sum of all part numbers
  int sum = 0;

  // central string in the ringbuffer
  const std::string_view s = (*r)[2];

  applyToNumbers(s, [&r, &sum](int num, size_t from, size_t to) {
      if(
          substrAdjacentToSymbol((*r)[0], from, to) ||
          substrAdjacentToSymbol((*r)[1], from, to) ||
          substrAdjacentToSymbol((*r)[2], from, to)
      ){
        // its a part number
        sum += num;
      }
  });

  return sum;
}

}
#endif
