#ifndef parser_HEADER
#define parser_HEADER

#include <fstream>
#include <set>
#include <vector>
#include <string>
#include <iostream>


enum RockT {
  RockX = 0,
  RockO,
  NoRock
};

using MatrixDim = struct {
  size_t nCols;
  size_t nRows;
};

using Rocks = std::pair<std::vector<RockT>, MatrixDim>;

void transpose(Rocks& r)
{
    const size_t n = r.second.nCols;
    const size_t m = r.second.nRows;
    size_t q = n * m - 1;

    for(size_t cnt = 1; cnt < n * m - 2; ++cnt){
        size_t k = n * cnt - q * (cnt / m);
        while(k > cnt)
          k = n * k - q * (k / m);

        if(k == cnt){
          size_t i = k;
          auto tmp = r.first[i];
          while(true){
            size_t j = n * i - q * (i / m);
            if (j == k)
              break;

            // r.first[i] = r.first[j];

            i = j;
          }
          r.first[i] = tmp;
        }
    }

    r.second.nCols = m;
    r.second.nRows = n;
}

#endif
