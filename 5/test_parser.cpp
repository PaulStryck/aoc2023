#include <catch2/catch_test_macros.hpp>
#include <string>
#include "parser.hpp"
#include "datastructures.hpp"

using namespace std;

TEST_CASE( "Parse single almanac entry", "[parser]" ) {
  string s = "11 10 2";

  Function::FnDefInterval f = parse(s);
  REQUIRE(f.offset == 1);
  REQUIRE(f.domain.lower() == 10);
  REQUIRE(f.domain.upper() == 11);
}

TEST_CASE( "Parse single almanac entry into Function", "[parser]" ) {
  string s = "50 98 2";

  Function::FnDefInterval f = parse(s);

  Function fn = Function(set{f});

  REQUIRE(fn(1) == 1);

  REQUIRE(fn(98) == 50);
  REQUIRE(fn(99) == 51);

  REQUIRE(fn(100) == 100);
}

TEST_CASE( "Parse a space separated string of integers", "[parser]" ) {
  string s = "foo 100 200 300";

  vector<unsigned int> nums;
  parseNumbers(s, [&nums](unsigned int x){
      nums.push_back(x);
  });

  REQUIRE(nums[0] == 100);
  REQUIRE(nums[1] == 200);
  REQUIRE(nums[2] == 300);
}
