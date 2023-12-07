#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <set>

#include "datastructures.hpp"


namespace prob_1 {
TEST_CASE( "Hand ordering 1", "[datastructures]" ) {
  Hand<Cards> a{HighCard, {N2, N3, N4, N5, N6}, 1};
  Hand<Cards> b{HighCard, {N3, N4, N5, N6, N7}, 1};

  REQUIRE(a < b);
}


TEST_CASE( "Hand ordering 2", "[datastructures]" ) {
  Hand<Cards> a{FullHouse, {N3, N3, N3, N5, N5}, 1};
  Hand<Cards> b{HighCard, {N3, N4, N5, N6, N7}, 1};

  REQUIRE(a > b);
}

TEST_CASE( "Hand ordering 3", "[datastructures]" ) {
  Hand<Cards> a{FullHouse, {N2, N2, N2, N6, N6}, 1};
  Hand<Cards> b{FiveOfAKind, {N4, N4, N4, N4, N4}, 1};

  REQUIRE(a < b);
}
} // namespace prob_1
