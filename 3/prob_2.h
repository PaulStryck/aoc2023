#ifndef P2_HEADER
#define P2_HEADER
#include <string>
#include "utils.h"

namespace prob_2 {
template<typename T>
inline bool adjacent(T from, T to, T check){
  if(from > 0) --from;
  ++to;

  return from <= check && check <= to;

}

/*
 * Multiply all numbers adjacent to a char at pos
 * increment b for each number that is multiplied
 * Returns multiplicative unit (1) if nothing is done
 *
 * Example:
 * multiplicativeScanAt("11.2.", 0, 2) -> 22, b = 2
 * multiplicativeScanAt("112..", 0, 2) -> 112, b = 1
 * multiplicativeScanAt("112..", 0, 4) -> 1, b = 0
 */
int multiplicativeScanAt(std::string_view s, int &b, size_t pos){
   // product, init with multiplicative unit
  int gearRatio = 1;

  applyToNumbers(s, [&gearRatio, &pos, &b](int num, size_t from, size_t to) {
    if(adjacent(from, to, pos)){
      // add to gearRation product
      gearRatio *= num;

      // increment the amout of parsed numbers
      b++;
    }
  });

  return gearRatio;
}

/*
 * scan the middle line of the ringbuffer for gear symbols (*)
 * get the gear ratio for all gear symbols and add them up
 */
int countGearRations(Ringbuffer<std::string, 3> *r){
  // central string in the ringbuffer
  const std::string_view s = (*r)[2];

  // sum of all found gear ratios
  int sum = 0;

  // tracking current index of a character
  size_t i = 0;
  for(auto it = s.begin(); it != s.end(); ++it, ++i){
    // scan for * symbols
    // if a * symbols is found
    //   multiply all adjacent numbers
    //   add them to the sum only if
    //   exactly 2 numbers were adjacent to the * symbol

    int gearRatio = 1;  // init with multiplicative unit
    int adjCount = 0;   // init with additive unit

    if(*it != '*') continue;  // scan only for potential gear symbols

    // scan entire ringbuffer for numbers adjacent to * at position i
    gearRatio *= multiplicativeScanAt((*r)[0], adjCount, i);
    gearRatio *= multiplicativeScanAt((*r)[1], adjCount, i);
    gearRatio *= multiplicativeScanAt((*r)[2], adjCount, i);

    if(adjCount == 2){
      sum += gearRatio;
    }
  }

  return sum;
}

}
#endif
