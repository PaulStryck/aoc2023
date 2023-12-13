#ifndef parser_HEADER
#define parser_HEADER

#include <fstream>
#include <set>
#include <vector>
#include <string>


/*
 * Store the given map of mirrors as row major matrix.
 * Where the first argument is the number of columns.
 */
using MirrorMap = std::pair<size_t, std::vector<char>>;

/*
 * Parse the starting block of infile until the first empty line
 */
[[nodiscard]] MirrorMap parseMap(std::ifstream& infile){
  MirrorMap m{};
  std::string sLineBuf{};

  while(getline(infile, sLineBuf)){
    if(sLineBuf.empty())
      break;

    if(m.first == 0)
      m.first = sLineBuf.length();
    else if(m.first != sLineBuf.length())
      throw;

    // append the read row
    m.second.insert(m.second.end(), sLineBuf.cbegin(), sLineBuf.cend());
  }

  return m;
}


#endif
