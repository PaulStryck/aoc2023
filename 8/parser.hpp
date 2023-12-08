#ifndef parser_HEADER
#define parser_HEADER

#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <regex>
#include <string>
#include <map>
#include <ranges>
#include <cassert>

#include "datastructures.hpp"


std::pair<vertex_t, std::pair<vertex_t, vertex_t>> parseNode(std::string_view s){
  assert(s.length() == 16);
  vertex_t _a, _b, _c;

  std::string_view a = s.substr(0,  3);
  std::string_view b = s.substr(7,  3);
  std::string_view c = s.substr(12,  3);

  std::copy(a.begin(), a.end(), _a.name.begin());
  std::copy(b.begin(), b.end(), _b.name.begin());
  std::copy(c.begin(), c.end(), _c.name.begin());

  return std::pair{_a, std::pair{_b, _c}};
}


#endif
