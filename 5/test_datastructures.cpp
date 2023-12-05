#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <set>

#include "datastructures.hpp"


TEST_CASE( "Picewise linear function", "[factorial]" ) {
  Function f = Function(set<Function::FnDefInterval>{ {interval::closed(0,1), 1}, {interval::closed(2,3), -1} } );

  REQUIRE(f(0) == 1);
  REQUIRE(f(1) == 2);
  REQUIRE(f(2) == 1);
  REQUIRE(f(3) == 2);
  REQUIRE(f(4) == 4);
}


TEST_CASE( "Evaluate Function with multiple Intervals", "[Function]" ) {
  Function f = Function(set<Function::FnDefInterval>{ {interval::closed(98, 99), -48}, {interval::closed(50, 97), 2} } );

  REQUIRE(f(1) == 1);

  REQUIRE(f(98) == 50);
  REQUIRE(f(99) == 51);

  REQUIRE(f(53) == 55);

  REQUIRE(f(100) == 100);
}

TEST_CASE( "Evaluate Function with multiple Intervals, using insert", "[Function]" ) {
  Function f = Function();
  f.f.insert({interval::closed(98, 99), -48});
  f.f.insert({interval::closed(50, 97),    2});

  REQUIRE(f(1) == 1);

  REQUIRE(f(98) == 50);
  REQUIRE(f(99) == 51);

  REQUIRE(f(53) == 55);

  REQUIRE(f(100) == 100);
}

TEST_CASE( "Evaluate Function on Domain", "[Function]" ){
  Function f = Function(set<Function::FnDefInterval>{ {interval::closed(1, 5), 1}, {interval::closed(10, 12), -1} } );

  domain_t domain;
  domain.insert(interval::closed(1, 2));
  domain.insert(interval::closed(3, 5));
  domain.insert(interval::closed(9, 20));

  domain_t expect;
  expect.insert(interval::closed(2,6)); // first and second interval get joined and incremented
  expect.insert(interval::closed(9,11)); // three items of third interval get decremented
  expect.insert(interval::left_open(12, 20)); // remainder. Fallback to identity
                                              //
  REQUIRE(f(domain) == expect);

  BENCHMARK("almanac evaluation for domains") {
    return f(domain);
  };
}
