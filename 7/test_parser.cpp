#include <catch2/catch_test_macros.hpp>
#include <string>

#include "parser.hpp"
#include "datastructures.hpp"

namespace prob_1{
  TEST_CASE( "Parse single almanac entry", "[parser]" ) {
    std::string s = "32T3K 765";
    Hand<Cards> h = parseHand<Cards>(s);
    REQUIRE(h.bid == 765);
    REQUIRE(h.h == OnePair);
  };

  TEST_CASE( "Compute Hand Type Five of a kind", "[parser]" ) {
    Hand<Cards> h {HighCard, {N2, N2, N2, N2, N2}, 0};

    REQUIRE(checkHand(h) == FiveOfAKind);
  };

  TEST_CASE( "Compute Hand Type Four of a kind", "[parser]" ) {
    Hand<Cards> h {HighCard, {N2, N3, N2, N2, N2}, 0};

    REQUIRE(checkHand(h) == FourOfAKind);
  };

  TEST_CASE( "Compute Hand Type full house", "[parser]" ) {
    Hand<Cards> h {HighCard, {N2, N3, N2, N3, N2}, 0};

    REQUIRE(checkHand(h) == FullHouse);
  };

  TEST_CASE( "Compute Hand Type Three of a kind", "[parser]" ) {
    Hand<Cards> h {HighCard, {N2, N3, N2, N4, N2}, 0};

    REQUIRE(checkHand(h) == ThreeOfAKind);
  };

  TEST_CASE( "Compute Hand Type Two Pair", "[parser]" ) {
    Hand<Cards> h {HighCard, {N2, N3, N3, N5, N2}, 0};

    REQUIRE(checkHand(h) == TwoPair);
  };

  TEST_CASE( "Compute Hand Type One Pair", "[parser]" ) {
    Hand<Cards> h {HighCard, {A, A, N2, N3, N4}, 0};

    REQUIRE(checkHand(h) == OnePair);
  };

  TEST_CASE( "Compute Hand Type High Card", "[parser]" ) {
    Hand<Cards> h {HighCard, {A, K, Q, J, T}, 0};

    REQUIRE(checkHand(h) == HighCard);
  }
}

namespace prob_2 {
  TEST_CASE( "P2 Pair with J", "[parser]" ) {
    Hand<Cards> a {HighCard, {N2, N3, N4, J, N7}, 0};
    Hand<Cards> b {HighCard, {N2, N3, N4, J, N5}, 0};
    a.h = checkHand(a);
    b.h = checkHand(b);

    REQUIRE(checkHand(a) == OnePair);
    REQUIRE(checkHand(b) == OnePair);

    REQUIRE(a > b);
  }

  TEST_CASE( "P 5 Js", "[parser]" ) {
    Hand<Cards> a {HighCard, {J, J, J, J, J}, 0};
    Hand<Cards> b {HighCard, {J, J, J, J, N2}, 0};
    a.h = checkHand(a);
    b.h = checkHand(b);

    REQUIRE(checkHand(a) == FiveOfAKind);
    REQUIRE(checkHand(b) == FiveOfAKind);

    REQUIRE(b > a);
  }

  TEST_CASE( "P FullHouse Js", "[parser]" ) {
    Hand<Cards> a {HighCard, {J, N2, N3, N3, N2}, 0};
    Hand<Cards> b {HighCard, {J, N3, N3, N4, N4}, 0};
    a.h = checkHand(a);
    b.h = checkHand(b);

    REQUIRE(checkHand(a) == FullHouse);
    REQUIRE(checkHand(b) == FullHouse);

    REQUIRE(b > a);
  }

  TEST_CASE( "P FullHouse Js 2", "[parser]" ) {
    Hand<Cards> a {HighCard, {N2, J, N3, N3, N2}, 0};
    Hand<Cards> b {HighCard, {J, N3, N3, N4, N4}, 0};
    a.h = checkHand(a);
    b.h = checkHand(b);

    REQUIRE(checkHand(a) == FullHouse);
    REQUIRE(checkHand(b) == FullHouse);

    REQUIRE(a > b);
  }

  TEST_CASE( "P Three of a Kind Js 2", "[parser]" ) {
    Hand<Cards> a {HighCard, {N3, J, J, K, Q}, 0};
    Hand<Cards> b {HighCard, {K, J, J, N4, N5}, 0};
    a.h = checkHand(a);
    b.h = checkHand(b);

    REQUIRE(a.h == ThreeOfAKind);
    REQUIRE(b.h == ThreeOfAKind);

    REQUIRE(b > a);
  }
}
