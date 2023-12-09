#ifndef parser_HEADER
#define parser_HEADER

#include <string>
#include <ranges>

#include "datastructures.hpp"


[[nodiscard]] oasis_t parseOasisReading(std::string_view s){
  oasis_t res;

  auto splits = std::views::split(s, ' ');
  for(auto sNum: splits){
    int foo = std::stoi ( std::string( std::string_view(sNum) ) );
    res.emplace_back(foo);
  }

  return res;
}


#endif
