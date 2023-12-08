#ifndef datastructures_HEADER
#define datastructures_HEADER

#include <compare>
#include <map>
#include <array>

struct vertex_t{
  std::array<char, 3> name;

  auto operator<=>(const vertex_t&) const = default;
};

typedef std::map<vertex_t, std::pair<vertex_t, vertex_t>> graph_t;

#endif
